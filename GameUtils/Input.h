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

	
	inline void AddToJoystickFlag(JOYSTICK_DIR flag)
	{
		joystick = joystick | flag;
		lastJoyState = joystick;
	}
	inline void AddToButtonFlag(unsigned int flag)
	{
		buttons = buttons | flag;
		lastButtonState = buttons;
	}
	inline bool CheckJoystickFlag(JOYSTICK_DIR flag)
	{
		return (bool)(joystick & flag);
	}
	inline bool CheckButtonFlag(unsigned int flag)
	{
		return (bool)(buttons & flag);
	}
	
	inline bool CheckButtonReleaseFlag(unsigned int flag)
	{

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
	inline bool CheckForAllInputFlags()
	{
		return (bool)(joystick > 0) || (buttons > 0);
	}
};


		