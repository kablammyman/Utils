#include "OptionMenu.h"


void OptionsMenu::MenuItem::SetBoundries()
{
	if (value <= min)
		value = min;
	else if (value >= max)
		value = max;
}
void OptionsMenu::MenuItem::SetToggleBoundries()
{
	if (value <= 0)
		value = 0;
	else value = 1;
}

string OptionsMenu::MenuItem::GetItemValueString()
{

	if (value >= 0)
	{
		if (isToggle)
		{
			if (value == 0)
				return "OFF";
			else
				return "ON";
		}
		else
			return to_string(value);
	}
	return "";
}

void OptionsMenu::GetKeyboardInput(int  newkey)
{
    char ASCII    = newkey & 0xff;
    char scancode = newkey >> 8;
 
    // a character key was pressed; add it to the string
    if(ASCII >= 32 && ASCII <= 126)
    {
    // add the new char, inserting or replacing as need be
    if(insert || iter == edittext.end())
        iter = edittext.insert(iter, ASCII);
    else
        edittext.replace(caret, 1, 1, ASCII);
 
    // increment both the caret and the iterator
    caret++;
    iter++;
    }
    // some other, "special" key was pressed; handle it here
    else
    switch(scancode)
    {
	case 83: //KEY_DEL:
            if(iter != edittext.end()) iter = edittext.erase(iter);
        break;
 
	case 14://KEY_BACKSPACE:
            if(iter != edittext.begin())
            {
                caret--;
                iter--;
                iter = edittext.erase(iter);
            }
        break;
 
	case 48://KEY_RIGHT:
            if(iter != edittext.end())   caret++, iter++;
        break;
 
	case 75://KEY_LEFT:
            if(iter != edittext.begin()) caret--, iter--;
        break;
 
	case 82://KEY_INSERT:
            if(insert) insert = 0; else insert = 1;
        break;
 
        default:
 
        break;
    }
}
void OptionsMenu::SetCurMenuOptionPos(MenuItem &newItem)
{
	newItem.option.x = x;
	newItem.option.y = y + level_y * menuItems.size();
}
void OptionsMenu::AddMenuOption(string add_menu_option, int value)
{
	MenuItem newItem;
	newItem.text = add_menu_option;
	newItem.value = value;
	newItem.isToggle = false;
	
	SetCurMenuOptionPos(newItem);

	menuItems.push_back(newItem);
}


void OptionsMenu::AddMenuOption(string add_menu_option,int value, int min, int max)
{
	MenuItem newItem;
	newItem.text = add_menu_option;
	newItem.value = value;
	newItem.isToggle = false;
	newItem.min = min;
	newItem.max = max;
	
	SetCurMenuOptionPos(newItem);

	menuItems.push_back(newItem);
}
void OptionsMenu::ResetMenuPositions()
{
	for (size_t i = 0; i < menuItems.size(); i++)
	{
		menuItems[i].option.x = x;
		menuItems[i].option.y = y + level_y * i;
	}
}
void OptionsMenu::AddMenuToggleOption(string add_menu_option, bool value)
{
	MenuItem newItem;
	newItem.text = add_menu_option;
	newItem.value = (int)value;
	newItem.isToggle = true;

	SetCurMenuOptionPos(newItem);

	menuItems.push_back(newItem);
}


void OptionsMenu::SetMenuPositions(int _x, int _y, int _level_y, int fontSize, RGB text_color1, RGB text_color2)
{
	x = _x;
	y = _y;
	level_y = _level_y;
	if(level_y < fontSize)
		level_y += ( fontSize + 5);
	font1_color = text_color1;
	font2_color = text_color2;
}

void OptionsMenu::SetInputDelay(int time)
{
	delayTimer = time;
}

void OptionsMenu::PrevSelection(void)
{
	if (menuItemIndex > 0)
	{
		if (!wait_flag)
		{
			menuItemIndex--;
			wait_flag = true;
		}
	}
}
void OptionsMenu::NextSelection(void)
{
	if (menuItemIndex < (int)menuItems.size() - 1)
	{
		if (!wait_flag)
		{
			menuItemIndex++;
			wait_flag = true;
		}
	}
}

void OptionsMenu::IncrementValue(void)
{
	if (!wait_flag)
	{
		menuItems[menuItemIndex].value++;
		wait_flag = true;
	}
}
void OptionsMenu::DecrementValue(void)
{
	if (!wait_flag)
	{
		menuItems[menuItemIndex].value--;
		wait_flag = true;
	}
}



void OptionsMenu::Update()
{
	
	if (wait_flag == true)
		delay--;

	if (delay <= 0)
	{
		wait_flag = false;
		delay = delayTimer;
	}

	//i need to use the key proxy...i gotta improve it first tho
	if (menuInput->CheckJoystick(Input::JOYSTICK_DIR::UP))
	{
		PrevSelection();
	}

	else if (menuInput->CheckJoystick(Input::JOYSTICK_DIR::DOWN))
	{
		NextSelection();
	}

	if (menuInput->CheckJoystick(Input::JOYSTICK_DIR::LEFT))
	{
		DecrementValue();
	}

	else if (menuInput->CheckJoystick(Input::JOYSTICK_DIR::RIGHT))
	{
		IncrementValue();
	}
	
	for (size_t i = 0; i < menuItems.size(); i++)
	{
		if(menuItems[i].isToggle)
			menuItems[i].SetToggleBoundries();
		else
			menuItems[i].SetBoundries();
	}
	
}


RGB OptionsMenu::GetMenuItemColorAt(size_t menuPos)
{
	if (menuPos == menuItemIndex)
		menuItems[menuPos].option.color = font1_color;
	else
		menuItems[menuPos].option.color = font2_color;

	return menuItems[menuPos].option.color;
}

ScreenText * OptionsMenu::GetMemuOptionAt(size_t menuPos)
{
	return &menuItems[menuPos].option;
}

void OptionsMenu::Draw(unsigned char *dest)
{
	//show_menu(0, dest, 0);
	/*for (size_t i = 0; i < options->GetNumMenuItems(); i++)
	{
		if (i == menuItemIndex)
			textout(bmp, font, menuItems[i].text.c_str(), x, y + level_y * i, font1_color);
		else
			textout(bmp, font, menuItems[i].text.c_str(), x, y + level_y * i, font2_color);
		if (menuItems[i].second >= 0)
			textprintf(bmp, font, x + (font_size + (font_size*menuItems[i].first.size())), y + level_y * i, font1_color, "%d", menuItems[i].second);
	}
	*/
}

/*int OptionsMenu::show_menu(DATAFILE *the_datafile, PIXMAP *bmp, int da_font)
{
text_mode(text_mode_type);
if (the_datafile != 0 && da_font != -1)
{
for (int i = 0; i < (int)menuItems.size(); i++)
{
if (i == menuItemIndex)
textout(bmp, (FONT*)the_datafile[da_font].dat, menuItems[i].first.c_str(), x, y + level_y * i, font1_color);
else
textout(bmp, (FONT*)the_datafile[da_font].dat, menuItems[i].first.c_str(), x, y + level_y * i, font2_color);
if (menuItems[i].second >= 0)
textprintf(bmp, (FONT*)the_datafile[da_font].dat, x + (font_size + (font_size*menuItems[i].first.size())), y + level_y * i, font1_color, "%d", menuItems[i].second);
}
}
else
{
for (int i = 0; i < (int)menuItems.size(); i++)
{
if (i == menuItemIndex)
textout(bmp, font, menuItems[i].first.c_str(), x, y + level_y * i, font1_color);
else
textout(bmp, font, menuItems[i].first.c_str(), x, y + level_y * i, font2_color);
if (menuItems[i].second >= 0)
textprintf(bmp, font, x + (font_size + (font_size*menuItems[i].first.size())), y + level_y * i, font1_color, "%d", menuItems[i].second);
}
}
return menuItemIndex;
}*/