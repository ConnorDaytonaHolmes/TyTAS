#include "Input.h"

KEYBDINPUT MakeKBInput(KeyboardAction kba, KeyStrokeType kst) {
	return KEYBOARD_INPUT(kba, kst);
}

MOUSEINPUT MakeMouseInput(MouseClickAction clickAction, MouseMoveAction moveAction) {
	return MOUSEINPUT(
		moveAction.x, moveAction.y,
		0, MOUSEMOVEFLAGS(moveAction) | clickAction,
		0, GetMessageExtraInfo()
	);
}

MOUSEINPUT MakeMouseInput(MouseClickAction clickAction) {
	return MOUSEINPUT(
		0, 0,
		0, clickAction,
		0, GetMessageExtraInfo()
	);
}

MOUSEINPUT MakeMouseInput(MouseMoveAction moveAction) {
	return MOUSEINPUT(
		moveAction.x, moveAction.y,
		0, MOUSEMOVEFLAGS(moveAction),
		0, GetMessageExtraInfo()
	);
}
