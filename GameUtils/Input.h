#pragma once

class Input
{
	unsigned int joystick;
	unsigned int buttons;
	unsigned int lastJoyState;
	unsigned int lastButtonState;
public:
	enum JOYSTICK_DIR
	{
		 UP =   1,
		 DOWN = 2,
		 LEFT  = 4,
		 RIGHT= 8,
	};

	
	inline void AddToJoystickState(JOYSTICK_DIR flag)
	{
		joystick = joystick | flag;
		lastJoyState = joystick;
	}
	inline void AddToButtonPresses(int button)
	{
		buttons = buttons | ConvertButtonNumberToFlag(button);
		lastButtonState = buttons;
	}
	inline bool CheckJoystick(JOYSTICK_DIR flag)
	{
		return(joystick & flag) !=0; //!=0 removes warning c4800
	}
	inline bool CheckButton(int button)
	{
		button--;//get back to array index value
		unsigned int flag = ConvertButtonNumberToFlag(button);
		return (buttons & flag) != 0; //!=0 removes warning c4800
	}
	
	inline bool CheckButtonReleaseFlag(int button)
	{
		button--;//get back to array index value
		unsigned int flag = ConvertButtonNumberToFlag(button);
		if (lastButtonState & flag)
		{
			if (!(buttons & flag))
			{
				lastButtonState &= ~flag;
				return true;
			}
		}
		return false;
	}

	inline bool CheckJoystickReleaseFlag(JOYSTICK_DIR flag)
	{

		if (lastJoyState & flag)
		{
			if (!(joystick & flag))
			{
				lastJoyState &= ~flag;
				return true;
			}
		}
		return false;
	}


	inline void ClearFlags()
	{
		joystick = 0;
		buttons = 0;
	}
	inline void ClearAllFlags()
	{
		joystick = 0;
		buttons = 0;
		lastButtonState = 0;
		lastJoyState = 0;
	}
	inline bool CheckForAllInputFlags()
	{
		return (bool)(joystick > 0) || (buttons > 0);
	}
	inline int ConvertFlagToButtonNumber(unsigned int flag)
	{
		return (flag >> 1);
	}
	inline unsigned int ConvertButtonNumberToFlag(int button)
	{
		//button++;//so we dont have a button 0
		return 1 << button;
	}
};


		