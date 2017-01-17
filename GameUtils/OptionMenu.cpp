#include "OptionMenu.h"


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

void OptionsMenu::AddMenuOption(string add_menu_option, int value)
{
	menuItems.push_back(make_pair(add_menu_option,value));
}


void OptionsMenu::SetMenuPositions(int _x, int _y, int _level_y, RGB text_color1, RGB text_color2)
{
	x = _x;
	y = _y;
	level_y = _level_y;
	font1_color = text_color1;
	font2_color = text_color2;
}

void OptionsMenu::SetInputDelay(int time)
{
	if (wait_flag == true)
		delay--;

	if (delay <= 0)
	{
		wait_flag = false;
		delay = time;
	}
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
		menuItems[menuItemIndex].second++;
		wait_flag = true;
	}
}
void OptionsMenu::DecrementValue(void)
{
	if (!wait_flag)
	{
		menuItems[menuItemIndex].second--;
		wait_flag = true;
	}
}
int OptionsMenu::SetBoundries(int index, int low_num, int hi_num)
{
	if (menuItems[index].second <= low_num)
		menuItems[index].second = low_num;
	else if (menuItems[index].second >= hi_num)
		menuItems[index].second = hi_num;

	return menuItems[index].second;
}
int OptionsMenu::SetToggleBoundries(int index)
{
	if (menuItems[index].second <= 0)
		menuItems[index].second = 0;
	else menuItems[index].second = 1;

	return menuItems[index].second;
}


void OptionsMenu::Update()
{
	SetInputDelay(5);
	
	//i need to use the key proxy...i gotta improve it first tho
	if (menuInput->CheckJoystickFlag(Input::JOYSTICK_DIR::UP))
	{
		PrevSelection();
	}

	else if (menuInput->CheckJoystickFlag(Input::JOYSTICK_DIR::DOWN))
	{
		NextSelection();
	}

	if (menuInput->CheckJoystickFlag(Input::JOYSTICK_DIR::LEFT))
	{
		DecrementValue();
	}

	else if (menuInput->CheckJoystickFlag(Input::JOYSTICK_DIR::RIGHT))
	{
		IncrementValue();
	}
	
	/*MainApp::Instance()->viewPathInfo = setToggleBoundries(0);
	MainApp::Instance()->viewClock = setToggleBoundries(1); 
	MainApp::Instance()->viewWorkoutTimer = setToggleBoundries(2);
	MainApp::Instance()->useImageMemory = setToggleBoundries(3);
	MainApp::Instance()->dirSelectionForDisplay = set_boundries(4, 1, 3);
	MainApp::Instance()->imageDisplayTimeLength = set_boundries(5, 3, 99);
	MainApp::Instance()->imageMemAmt = set_boundries(6, 0, 100);*/
}

void OptionsMenu::Draw(unsigned char *dest)
{
	//show_menu(NULL, dest, 0);
}

/*int OptionsMenu::show_menu(DATAFILE *the_datafile, PIXMAP *bmp, int da_font)
{
text_mode(text_mode_type);
if (the_datafile != NULL && da_font != -1)
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