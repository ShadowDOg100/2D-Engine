#ifndef MAINCOREMESSAGE_H_
#define MAINCOREMESSAGE_H_

#define MC_T_REMINDME		100 // Defines a remind me message from the Main Core
#define MC_Q_DRAW			0   // Defines a qualifier for MC_T_REMINDME to redraw
#define MC_Q_PROCESS		1	// Defines a qualifier for MC_T_REMINDME to process

#define MC_T_HUDCHANGE		101 // Defines a message from the game to the HUD
#define MC_Q_SET			0	// Defines a qualifier for MC_T_GAMESCORE to use a SET operation
								// P1: Relevent control classification
								// P2: Value to set to
#define MC_Q_INCREMENT		1   // Defines a qualifier for MC_T_GAMESCORE to use a INCREMENT operation
								// P1: Relevent control classification
								// P2: Amount to increment by
#define MC_Q_INQUIRE		2   // Defines a qualifier for MC_T_GAMESCORE to INQUIRE
								// P1: Relevent control classification
								// P2: Unused
#define MC_Q_SETRANGE		3   // Defines a qualifier for MC_T_GAMESCORE to set the range
								// P1: Relevent control classification
								// P2.w.lo: Minimum Limit
								// P2.w.hi: Maximum Limit
#define MC_Q_START			4
								// P1: Relevent control classification
#define MC_Q_RESET			5
								// P1: Relevent control classification
#define MC_Q_STOP			6
								// P1: Relevent control classification

#endif