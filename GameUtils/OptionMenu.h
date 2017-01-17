#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include "Scene.h"
#include "Input.h"

using namespace std;

//this class is from my ooooooold game coding days. I  made a few updates and improvements since
//im much better at this coding thing than i was when i orignally wrote this,lol
//notice the underscore variable and method names!! So C! 

//since this menu is only for the option screen, ill leave the class in here
//class menu_class : public RenderObject {
class OptionsMenu : public RenderObject {

	int x, y;
	int text_mode_type;

	int level_y;// = 50;
	RGB font1_color;// = 12;
	RGB font2_color;// = 15;
	int font_size;// = 30; //font size*X = amt of letters in a space for text boxes

	bool menu_on; //a flag to see if meu is showing or not
	bool wait_flag;
	short int delay;
	
	typedef pair<string, int> menuItem;
	vector<menuItem> menuItems;

	int menuItemIndex;
	
	int current_menupar_fill;

	Input *menuInput;
	
	//keyboard input
	int     caret;                       // tracks the text caret
	bool    insert;                    // true of should text be inserted
	string  edittext;                         // an empty string for editting
	string::iterator iter; // string iterator
public:

	//im going to draw directly to the screen buffer, so i wont need my own buffer
	OptionsMenu() : RenderObject(0, 0, 0, 0) {
		delay = 0;
		wait_flag = false;
		menuItemIndex = 0;
		menu_on = true;
		current_menupar_fill = 0;
		font_size = 10;
		text_mode_type = 0;//its an allegro4 thing
	};
	void SetInputDelay(int time);
	void PrevSelection(void);
	void NextSelection(void);
	void IncrementValue(void);
	void DecrementValue(void);
	int SetToggleBoundries(int index);
	int SetBoundries(int index, int low_num, int hi_num);
	void AddMenuOption(string add_menu_option, int value = -1);
	void SetMenuPositions(int _x, int _y, int _level_y, RGB text_color1, RGB text_color2);
	void GetKeyboardInput(int  newkey);//newkey = next key in keyboard buffer

	//int ShowMenu(DATAFILE *the_datafile, PIXMAP *bmp, int da_font);

	virtual void Update();
	virtual void Draw(unsigned char *dest);
};




#endif //OPTIONS_H