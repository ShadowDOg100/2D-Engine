#ifndef IOCOREMESSAGE_H_
#define IOCOREMESSAGE_H_

#define IO_T_KEYBOARD           1000	// Defines a keyboard message from the IO Core
#define IO_Q_KEYDOWN			0		// Defines a qualifier for IO_T_KEYBOARD that a key is down
																//P1: The key(s) currently down
																//P2: Used for Shift, Ctrl, or Alt keys being pressed
#define IO_Q_KEYRELEASE			1		// Defines a qualifier for IO_T_KEYBOARD that a key was released
#define IO_Q_KEYPRESS			2

#define IO_T_MOUSE				1001	// Defines a mouse message from the IO Core
#define IO_Q_MBCHANGE			0		// Defines a qualifier for IO_T_MOUSE that a mouse-button is down
                                                                // P1: Mouse button status, and keyboard shift, ctrl, alt status
                                                                // P2: mouse X & Y position
#define IO_Q_MOUSEMOVE			2		// Defines a qualifier for IO_T_MOUSE that the mouse has moved
                                                                // P1: Mouse button status, and keyboard shift, ctrl, alt status
                                                                // P2: mouse X & Y position
#define IO_T_MENU_BUTTON		1002
#define IO_Q_BUTTON_CLICK		0		// p1 contains button id
										// p2 contains user info

#define IO_T_CONTROLCHANGE		1003	// p1 contains value of control
										// p2 contains user info

#endif