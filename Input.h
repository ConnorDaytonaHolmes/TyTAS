#pragma once
#include <windows.h>
#include <unordered_map>

#define HARDWARE_INPUT

#ifndef HARDWARE_INPUT
#define VIRTUAL_INPUT
#endif

#ifdef HARDWARE_INPUT
#define KEYFLAGS(up) KEYEVENTF_SCANCODE | (up * KEYEVENTF_KEYUP)
#define KEYBOARD_INPUT(action, up) KEYBDINPUT(0, hwScanCodes.at(action), KEYFLAGS(up), 0, GetMessageExtraInfo())
#define MOUSE_INPUT

#else #ifdef VIRTUAL_INPUT
#define KEYBOARD_INPUT(action, kst) KEYBDINPUT(virtualKeyCodes.at(action), 0, (kst * KEYEVENTF_KEYUP), 0, GetMessageExtraInfo())
#define KEYFLAGS(up) (up * KEYEVENTF_KEYUP)
#endif

#define MOUSEMOVEFLAGS(ma) MOUSEEVENTF_MOVE | (MOUSEEVENTF_ABSOLUTE * moveAction.isAbsolute)

enum KeyboardAction : int {
	JUMP,
	FORWARD,
	LEFT,
	RIGHT,
	BACKWARDS,
	INTERACT,
	PREVIOUSRANG,
	NEXTRANG,
	WALK,
	AIM,
	MENU,
	ACCEPT,
	MENU_UP,
	MENU_LEFT,
	MENU_DOWN,
	MENU_RIGHT,
};

const std::unordered_map<int, WORD> hwScanCodes = {
	{ KeyboardAction::JUMP, 57 },
	{ KeyboardAction::FORWARD, 17 },
	{ KeyboardAction::LEFT, 30 },
	{ KeyboardAction::RIGHT, 32},
	{ KeyboardAction::BACKWARDS, 31},
	{ KeyboardAction::INTERACT, 19},
	{ KeyboardAction::PREVIOUSRANG, 16},
	{ KeyboardAction::NEXTRANG, 18},
	{ KeyboardAction::WALK, 42},
	{ KeyboardAction::AIM, 29},
	{ KeyboardAction::MENU, 1},
	{ KeyboardAction::ACCEPT, 28},
	{ KeyboardAction::MENU_UP, 72},
	{ KeyboardAction::MENU_LEFT, 75},
	{ KeyboardAction::MENU_DOWN, 80},
	{ KeyboardAction::MENU_RIGHT, 77},
};
const std::unordered_map<int, WORD> virtualKeyCodes = {
	{ KeyboardAction::JUMP, VK_SPACE },
	{ KeyboardAction::FORWARD, 'W' },
	{ KeyboardAction::LEFT, 'A'},
	{ KeyboardAction::RIGHT, 'D'},
	{ KeyboardAction::BACKWARDS, 'S'},
	{ KeyboardAction::INTERACT, 'R'},
	{ KeyboardAction::PREVIOUSRANG, 'Q'},
	{ KeyboardAction::NEXTRANG, 'E'},
	{ KeyboardAction::WALK, VK_LSHIFT},
	{ KeyboardAction::AIM, VK_LCONTROL},
	{ KeyboardAction::MENU, VK_ESCAPE},
	{ KeyboardAction::ACCEPT, VK_RETURN},
	{ KeyboardAction::MENU_UP, VK_UP},
	{ KeyboardAction::MENU_LEFT, VK_LEFT},
	{ KeyboardAction::MENU_DOWN, VK_DOWN},
	{ KeyboardAction::MENU_RIGHT, VK_RIGHT},
};


enum KeyStrokeType {
	DOWN,
	UP
};

struct MouseMoveAction {
	bool isAbsolute;
	LONG x;
	LONG y;
};

enum MouseClickAction : int {
	THROWRANG_DOWN = MOUSEEVENTF_LEFTDOWN,
	THROWRANG_UP = MOUSEEVENTF_LEFTUP,
	BITE_DOWN = MOUSEEVENTF_RIGHTDOWN,
	BITE_UP = MOUSEEVENTF_RIGHTUP,
};

struct TASData {
	INPUT inputs[];
};

KEYBDINPUT MakeKBInput(KeyboardAction kba, KeyStrokeType kst);
MOUSEINPUT MakeMouseInput(MouseClickAction clickAction, MouseMoveAction moveAction);
MOUSEINPUT MakeMouseInput(MouseClickAction clickAction);
MOUSEINPUT MakeMouseInput(MouseMoveAction moveAction);


static INPUT JUMP_DOWN = {
	.type = INPUT_KEYBOARD,
	.ki = MakeKBInput(KeyboardAction::JUMP, KeyStrokeType::DOWN),
};

static INPUT JUMP_UP = {
	.type = INPUT_KEYBOARD,
	.ki = MakeKBInput(KeyboardAction::JUMP, KeyStrokeType::UP),
};
