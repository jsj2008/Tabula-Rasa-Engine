#ifndef __trINPUT_H__
#define __trINPUT_H__

#include "trModule.h"

#include "SDL/include/SDL_gamecontroller.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum trEventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum trKeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum GAMEPAD_STATE
{
	PAD_BUTTON_IDLE = 0,
	PAD_BUTTON_DOWN,
	PAD_BUTTON_REPEAT,
	PAD_BUTTON_KEY_UP
};

struct Gamepad {

	GAMEPAD_STATE A = PAD_BUTTON_IDLE;
	GAMEPAD_STATE B;
	GAMEPAD_STATE Y;
	GAMEPAD_STATE X;

	GAMEPAD_STATE CROSS_UP;
	GAMEPAD_STATE CROSS_DOWN;
	GAMEPAD_STATE CROSS_LEFT;
	GAMEPAD_STATE CROSS_RIGHT;

	GAMEPAD_STATE JOYSTICK_UP;
	GAMEPAD_STATE JOYSTICK_DOWN;
	GAMEPAD_STATE JOYSTICK_LEFT;
	GAMEPAD_STATE JOYSTICK_RIGHT;

	GAMEPAD_STATE START;

};

class trInput : public trModule
{

public:

	trInput();

	// Destructor
	virtual ~trInput();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(trEventWindow ev);

	// Check key states (includes mouse and joy buttons)
	trKeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	trKeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	GAMEPAD_STATE GetGamepadButton(int key)
	{
		if (key == 0)
			return gamepad.A;
		else if (key == 1)
			return gamepad.B;
		else if (key == 2)
			return gamepad.Y;
		else if (key == 3)
			return gamepad.X;
		
	}

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetWorldMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	Gamepad gamepad;
	void ButtonForGamepad();

private:
	bool		windowEvents[WE_COUNT];
	trKeyState*	keyboard = nullptr;
	trKeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x = 0;
	int			mouse_motion_y = 0;
	int			mouse_x = 0;
	int			mouse_y = 0;

	SDL_GameController *controller;
	SDL_Joystick *joystick;

};

#endif // __trINPUT_H__