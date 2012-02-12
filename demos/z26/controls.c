/*
	controls.c -- handle 2600 controllers
*/

/* control key definitions */

//#include <SDL_keyboard.h>
#include "sdlkeys.h"

#define KeyEsc 		SDLK_ESCAPE	/* ESC (Quit) */

#define KeyF1 		SDLK_F1		/* F1  (Reset) */
#define KeyF2 		SDLK_F2		/* F2  (Select) */
#define KeyF3 		SDLK_F3		/* F3  (B/W) */
#define KeyF4 		SDLK_F4		/* F4  (Color) */
#define KeyF5 		SDLK_F5		/* F5  (P0 easy) */
#define KeyF6 		SDLK_F6		/* F6  (P0 hard) */
#define KeyF7 		SDLK_F7		/* F7  (P1 easy) */
#define KeyF8 		SDLK_F8		/* F8  (P1 hard) */
#define KeyF9 		SDLK_F9		/* F9  (unassigned) */
#define KeyF10 		SDLK_F10	/* F10 (unassigned) */
#define KeyF11 		SDLK_F11	/* F11 (trace on) */
#define KeyF12 		SDLK_F12	/* F12 (trace off) */

#define Key0		SDLK_0
#define Key1		SDLK_1
#define Key2		SDLK_2
#define Key3		SDLK_3
#define Key4		SDLK_4
#define Key5		SDLK_5
#define Key6		SDLK_6
#define Key7		SDLK_7
#define Key8		SDLK_8
#define Key9		SDLK_9

#define KeyQ		SDLK_q
#define KeyW		SDLK_w
#define KeyA		SDLK_a
#define KeyZ		SDLK_z
#define KeyX		SDLK_x
#define KeyC		SDLK_c
#define KeyU		SDLK_u
#define KeyI		SDLK_i
#define KeyO		SDLK_o
#define KeyJ		SDLK_j
#define KeyK		SDLK_k
#define KeyL		SDLK_l
#define KeyM		SDLK_m
#define KeyComma 	SDLK_COMMA	/* , */
#define KeyDot 		SDLK_PERIOD	/* . */

#define KeyG		SDLK_g
#define KeyH		SDLK_h
#define KeyP		SDLK_p
#define KeyR		SDLK_r
#define KeyT		SDLK_t
#define KeyY		SDLK_y
#define KeyColon 	SDLK_COLON	/* : */

#define KeyLeft 	SDLK_LEFT	/* Left arrow */
#define KeyRight 	SDLK_RIGHT	/* Right arrow */
#define KeyUp 		SDLK_UP		/* Up arrow */
#define KeyDown 	SDLK_DOWN	/* Down arrow */

#define KeyLCtrl 	SDLK_LCTRL	/* Ctrl */
#define KeyRCtrl 	SDLK_RCTRL	/* Ctrl */

/* Some Linux distros configure the Alt keys as Meta keys, some as
	Alt keys, and some as one Alt, one Meta. Also, in the future, we
	want to be portable to other flavors of UNIX which only have
	Meta keys (Alt keys are pretty PC-specific). */
	
#define KeyLMeta 	SDLK_LMETA
#define KeyRMeta 	SDLK_RMETA
#define KeyLAlt 	SDLK_LALT
#define KeyRAlt 	SDLK_RALT
/* kludges to allow testing for both CTRL (or ALT) keys at the same time */
#define KeyCtrl		SDLK_F14	/* gets overwritten with LCtrl OR RCtrl */
#define	KeyAlt		SDLK_F15	/* gets overwritten with LAlt OR RAlt OR LMeta OR Rmeta */


#define KeyLShift 	SDLK_LSHIFT
#define KeyRShift 	SDLK_RSHIFT
#define KeySlash 	SDLK_SLASH	/* / */
#define KeyBackSlash 	SDLK_BACKSLASH	/* \ */

#define KeyS 		SDLK_s		/* S (P2 left) */
#define KeyF		SDLK_f		/* F (P2 right) */
#define KeyE		SDLK_e		/* E (P2 up) */
#define KeyD		SDLK_d		/* D (P2 down) */
#define KeyN		SDLK_n		/* N (P2 fire) */
#define KeyB		SDLK_b		/* B (P2 booster grip - trigger) */
#define KeyV		SDLK_v		/* V (P2 booster grip - booster) */

#define KeyBackSpace 	SDLK_BACKSPACE	/* Back Space (pause game) */
#define KeyEnter 	SDLK_RETURN	/* Enter (resume game) */

#define KeyEquals 	SDLK_EQUALS	/* = (do a screen dump) */
#define KeyMinus 	SDLK_MINUS	/* switch between palettes */

#define KeyPgUp 	SDLK_PAGEUP	/* scroll up */
#define KeyPgDn 	SDLK_PAGEDOWN	/* scroll down */
#define KeyHome 	SDLK_HOME	/* back to default screen position */
#define KeyTab 		SDLK_TAB	/* switch mouse control in paddle mode */

#define KeyBackTick SDLK_BACKQUOTE

/* define control keys */

#define P1Left 		KeyLeft
#define P1Right 	KeyRight
#define P1Up 		KeyUp
#define P1Down 		KeyDown
#define P1Fire 		KeyCtrl
#define P1TriggerBG 	KeySlash
#define P1BoosterBG 	KeyRShift

#define P2Left 		KeyS
#define P2Right 	KeyF
#define P2Up 		KeyE
#define P2Down 		KeyD
#define P2Fire 		KeyN
#define P2TriggerBG 	KeyB
#define P2BoosterBG 	KeyV

#define P1Pad1 		Key7
#define P1Pad2 		Key8
#define P1Pad3 		Key9
#define P1Pad4 		KeyU
#define P1Pad5 		KeyI
#define P1Pad6 		KeyO
#define P1Pad7 		KeyJ
#define P1Pad8 		KeyK
#define P1Pad9 		KeyL
#define P1PadStar 	KeyM		/* * */
#define P1Pad0 		KeyComma
#define P1PadPound 	KeyDot		/* # */

#define P2Pad1 		Key1
#define P2Pad2 		Key2
#define P2Pad3 		Key3
#define P2Pad4 		KeyQ
#define P2Pad5 		KeyW
#define P2Pad6 		KeyE
#define P2Pad7 		KeyA
#define P2Pad8 		KeyS
#define P2Pad9 		KeyD
#define P2PadStar 	KeyZ		/* * */
#define P2Pad0 		KeyX
#define P2PadPound 	KeyC		/* # */

#define ResetKey 	KeyF1
#define SelectKey 	KeyF2
#define BWKey		KeyF3
#define ColorKey 	KeyF4
#define P0Easy 		KeyF5
#define P0Hard 		KeyF6
#define P1Easy 		KeyF7
#define P1Hard 		KeyF8

/* swap all signals from port 0 with those from port 1 */
void SwapPorts()
{
	unsigned int SwapVal;	/* hold values while swapping left and right port */

	if (SwapPortsFlag)	/* swap left and right controllers ? */
	{
		SwapVal = InputLatch[0];
		InputLatch[0] = InputLatch [1];
		InputLatch[1] = SwapVal;
		SwapVal = ChargeTrigger0[0];
		ChargeTrigger0[0] = ChargeTrigger0[2];
		ChargeTrigger0[2] = SwapVal;
		SwapVal = ChargeTrigger0[1];
		ChargeTrigger0[1] = ChargeTrigger0[3];
		ChargeTrigger0[3] = SwapVal;
		IOPortA = ((IOPortA & 0xf0) >> 4) | ((IOPortA & 0x0f) << 4);
		IOPortA_Controllers = IOPortA;
		IOPortA_UnusedBits = ((IOPortA_UnusedBits & 0xf0) >> 4) | ((IOPortA_UnusedBits & 0x0f) << 4);
	}
}

/*
** specific controller handling functions
*/

void DoBoosterGrip_L()
{
	ChargeTrigger0[0] = CHARGEMAX;	/* will not expire / will be read as 0 */
	ChargeTrigger0[1] = CHARGEMAX;
	if (KeyboardEnabled)
	{
		if (KeyTable[P1TriggerBG]) ChargeTrigger0[0] = 0;
		if (KeyTable[P1BoosterBG]) ChargeTrigger0[1] = 0;
	}
	if (JoystickEnabled)
	{
		if (JoystickButton[0][1]) ChargeTrigger0[0] = 0;
		if (JoystickButton[0][2]) ChargeTrigger0[1] = 0;
	}
}

void DoBoosterGrip_R()
{
	ChargeTrigger0[2] = CHARGEMAX;	/* will not expire / will be read as 0 */
	ChargeTrigger0[3] = CHARGEMAX;
	if (KeyboardEnabled)
	{
		if (KeyTable[P2TriggerBG]) ChargeTrigger0[2] = 0;
		if (KeyTable[P2BoosterBG]) ChargeTrigger0[3] = 0;
	}
	if (JoystickEnabled)
	{
		if (JoystickButton[1][1]) ChargeTrigger0[2] = 0;
		if (JoystickButton[1][2]) ChargeTrigger0[3] = 0;
	}
}

#define MJ_Threshold 2

void DoJoystick_L()
{
	static db MJ_Count = 0;
	static db MJ_Last_IOPortA = 0xff;

	IOPortA = IOPortA | 0xf0;
	InputLatch[0] = 0x80;
#if 0
	if (MouseEnabled)
	{
		if (srv_micky_x || srv_micky_y)
		{
			if ((!(abs(srv_micky_y) > abs(srv_micky_x) * 2)) && (abs(srv_micky_x) >= MJ_Threshold))
			{
				if (srv_micky_x < 0) {
					IOPortA = IOPortA & 0xbf;	/* left */
				} else if (srv_micky_x > 0) {
					IOPortA = IOPortA & 0x7f;	/* right */
				}
			}

			if ((!(abs(srv_micky_x) > abs(srv_micky_y) * 2)) && (abs(srv_micky_y) >= MJ_Threshold))
			{
				if (srv_micky_y < 0) {
					IOPortA = IOPortA & 0xef;	/* up */
				} else if (srv_micky_y > 0) {
					IOPortA = IOPortA & 0xdf;	/* down */
				}
			}

			MJ_Last_IOPortA = IOPortA;
			MJ_Count = 1;
		}
		else if (MJ_Count)
		{
			MJ_Count--;
			IOPortA = MJ_Last_IOPortA;
		}
		if (srv_mouse_button) InputLatch[0] = 0x00;
	}
#endif
	if (KeyboardEnabled)
	{
		if (keys.key2 & KEY_RIGHT) IOPortA = IOPortA & 0x7f;
		if (keys.key2 & KEY_LEFT) IOPortA = IOPortA & 0xbf;
		if (keys.key2 & KEY_DOWN) IOPortA = IOPortA & 0xdf;
		if (keys.key2 & KEY_UP) IOPortA = IOPortA & 0xef;
		if (keys.key2 & KEY_X) InputLatch[0] = 0x00;

/* for OLPC XO-1

		if (KeyTable[SDLK_KP6]) IOPortA = IOPortA & 0x7f;
		if (KeyTable[SDLK_KP4]) IOPortA = IOPortA & 0xbf;
		if (KeyTable[SDLK_KP2]) IOPortA = IOPortA & 0xdf;
		if (KeyTable[SDLK_KP8]) IOPortA = IOPortA & 0xef;
		if (KeyTable[SDLK_KP3]) InputLatch[0] = 0x00;
*/		
	}
#if 0
	if (JoystickEnabled)
	{
		if (JoystickAxis[0][0] > 16384-4096)
		{
			IOPortA = IOPortA & 0x7f;
			/* Stelladaptor sends "half moved right" for L+R pushed together */
			if ((Stelladaptor[0]) && (JoystickAxis[0][0] < 16384+4096))
				IOPortA = IOPortA & 0xbf;
		}
		if (JoystickAxis[0][0] < -16384) IOPortA = IOPortA & 0xbf;
		if (JoystickAxis[0][1] > 16384-4096)
		{
			IOPortA = IOPortA & 0xdf;
			/* Stelladaptor sends "half moved down" for U+D pushed together */
			if ((Stelladaptor[0]) && (JoystickAxis[0][1] < 16384+4096))
				IOPortA = IOPortA & 0xef;
		}
		if (JoystickAxis[0][1] < -16384) IOPortA = IOPortA & 0xef;
		if (JoystickButton[0][0]) InputLatch[0] = 0x00; 
	}

	if ((!AllowAll4) && (!Stelladaptor[0]))
	{
		if ((IOPortA & 0xc0) == 0) IOPortA = IOPortA | 0xc0;
		if ((IOPortA & 0x30) == 0) IOPortA = IOPortA | 0x30;
	}
#endif
}

void DoJoystick_R()
{
#if 0
	IOPortA = IOPortA | 0x0f;
	InputLatch[1] = 0x80;
	if (KeyboardEnabled)
	{
		if (KeyTable[P2Right]) IOPortA = IOPortA & 0xf7;
		if (KeyTable[P2Left]) IOPortA = IOPortA & 0xfb;
		if (KeyTable[P2Down]) IOPortA = IOPortA & 0xfd;
		if (KeyTable[P2Up]) IOPortA = IOPortA & 0xfe;
		if (KeyTable[P2Fire]) InputLatch[1] = 0x00;
	}

	if (JoystickEnabled)
	{
		if (JoystickAxis[1][0] > 16384-4096)
		{
			IOPortA = IOPortA & 0xf7;
			if ((Stelladaptor[1]) && (JoystickAxis[1][0] < 16384+4096))
				IOPortA = IOPortA & 0xfb;
		}
		if (JoystickAxis[1][0] < -16384) IOPortA = IOPortA & 0xfb;
		if (JoystickAxis[1][1] > 16384-4096)
		{
			IOPortA = IOPortA & 0xfd;
			if ((Stelladaptor[1]) && (JoystickAxis[1][1] < 16384+4096))
			IOPortA = IOPortA & 0xfe;
		}
		if (JoystickAxis[1][1] < -16384) IOPortA = IOPortA & 0xfe;
		if (JoystickButton[1][0]) InputLatch[1] = 0x00; 
	}

	if ((!AllowAll4) && (!Stelladaptor[1]))	/* filter out "impossible" joystick motion */
	{
		if ((IOPortA & 0x0c) == 0) IOPortA = IOPortA | 0x0c;	/* left + right ? */
		if ((IOPortA & 0x03) == 0) IOPortA = IOPortA | 0x03;	/* up + down ? */
	}
#endif
}

#if 0
void DoPaddle_L()
{
	static int KeyRepeat0 = 0;
	static unsigned int PaddleRepeat0 = 0;
	static int KeyRepeat1 = 0;
	static unsigned int PaddleRepeat1 = 0;
	static int Charge[2] = {TRIGMAX/2, TRIGMAX/2};
	static int LastCharge[2] = {TRIGMAX/2, TRIGMAX/2};
	static unsigned char Button[2] = {0, 0};
	static int LeftMotion[2] = {0, 0};

	Button[0] = 0;
	Button[1] = 0;

	if (KeyboardEnabled)
	{
		if (KeyRepeat0)
		{
			PaddleRepeat0++;
			if (PaddleRepeat0 > PaddleSensitivity) PaddleRepeat0 = 2;
		}

		if (KeyRepeat1)
		{
			PaddleRepeat1++;
			if (PaddleRepeat1 > PaddleSensitivity) PaddleRepeat1 = 2;
		}

		KeyRepeat0 = 0;
		KeyRepeat1 = 0;

		if (KeyTable[P1Right])
		{
			KeyRepeat0 = 1;
			if (Charge[0] > (PaddleRepeat0 >> 1))
				Charge[0] -= (PaddleRepeat0 >> 1);
		}
		if (KeyTable[P1Left])
		{
			KeyRepeat0 = 1;
			if ((Charge[0] + (PaddleRepeat0 >> 1)) < TRIGMAX)
				Charge[0] += (PaddleRepeat0 >> 1);
		}
		if (KeyTable[P1Up])
		{
			KeyRepeat1 = 1;
			if (Charge[1] > (PaddleRepeat1 >> 1))
				Charge[1] -= (PaddleRepeat1 >> 1);
		}
		if (KeyTable[P1Down])
		{
			KeyRepeat1 = 1;
			if ((Charge[1] + (PaddleRepeat1 >> 1)) < TRIGMAX)
				Charge[1] += (PaddleRepeat1 >> 1);
		}

		if (KeyTable[P1Fire]) Button[0] = 1;
		if (KeyTable[P1BoosterBG]) Button[1] = 1;
	}

	if (MouseEnabled)
	{
		if ((MouseBaseX == 0) || (MouseBaseX == 1))
		{
			if (MPdirection & 0x01) Charge[MouseBaseX] = Charge[MouseBaseX] + srv_micky_x;
			else Charge [MouseBaseX] = Charge[MouseBaseX] - srv_micky_x;
			if (Charge[MouseBaseX] < TRIGMIN) Charge[MouseBaseX] = TRIGMIN;
			if (Charge[MouseBaseX] > TRIGMAX) Charge[MouseBaseX] = TRIGMAX;
			if ((srv_mouse_button) && (MouseBaseY == 0xff)) Button[MouseBaseX] = 1;
			if (srv_mouse_button & 0x01) Button[MouseBaseX] = 1;
		}
		
		if ((MouseBaseY == 0) || (MouseBaseY == 1))
		{
			if (MPdirection & 0x02) Charge[MouseBaseY] = Charge[MouseBaseY] + srv_micky_y;
			else Charge [MouseBaseY] = Charge[MouseBaseY] - srv_micky_y;
			if (Charge[MouseBaseY] < TRIGMIN) Charge[MouseBaseY] = TRIGMIN;
			if (Charge[MouseBaseY] > TRIGMAX) Charge[MouseBaseY] = TRIGMAX;
			if ((srv_mouse_button) && (MouseBaseX == 0xff)) Button[MouseBaseY] = 1;
			if (srv_mouse_button & 0x06) Button[MouseBaseY] = 1;
		}
	}

	if(JoystickEnabled)
	{
		Charge[0] = (((JoystickAxis[0][0] * -1) + 32767) >> 8) & 0xff;
		Charge[1] = (((JoystickAxis[0][1] * -1) + 32767) >> 8) & 0xff;

		/* filter out jitter by not allowing rapid direction changes */
		if (Charge[0] - LastCharge[0] > 0)	/* we are moving left */
		{
			if (!LeftMotion[0])		/* moving right before ? */
			{
				if (Charge[0] - LastCharge[0] <= 4)
				{
					Charge[0] = LastCharge[0];
				}
				else
				{
					Charge[0] = (Charge[0] + LastCharge[0]) >> 1;
					LastCharge[0] = (((JoystickAxis[0][0] * -1) + 32767) >> 8) & 0xff;
					LeftMotion[0] = 1; 
				}
			}
			else
			{
				Charge[0] = (Charge[0] + LastCharge[0]) >> 1;
				LastCharge[0] = (((JoystickAxis[0][0] * -1) + 32767) >> 8) & 0xff;
			}
		}
		/* filter out jitter by not allowing rapid direction changes */
		else if (Charge[0] - LastCharge[0] < 0)	/* we are moving right */
		{
			if (LeftMotion[0])		/* moving left before ? */
			{
				if (LastCharge[0] - Charge[0] <= 4)
				{
					Charge[0] = LastCharge[0];
				}
				else
				{
					Charge[0] = (Charge[0] + LastCharge[0]) >> 1;
					LastCharge[0] = (((JoystickAxis[0][0] * -1) + 32767) >> 8) & 0xff;
					LeftMotion[0] = 0; 
				}
			}
			else
			{
				Charge[0] = (Charge[0] + LastCharge[0]) >> 1;
				LastCharge[0] = (((JoystickAxis[0][0] * -1) + 32767) >> 8) & 0xff;
			}
		}

		/* filter out jitter by not allowing rapid direction changes */
		if (Charge[1] - LastCharge[1] > 0)	/* we are moving left */
		{
			if (!LeftMotion[1])		/* moving right before ? */
			{
				if (Charge[1] - LastCharge[1] <= 4)
				{
					Charge[1] = LastCharge[1];
				}
				else
				{
					Charge[1] = (Charge[1] + LastCharge[1]) >> 1;
					LastCharge[1] = (((JoystickAxis[0][1] * -1) + 32767) >> 8) & 0xff;
					LeftMotion[1] = 1; 
				}
			}
			else
			{
				Charge[1] = (Charge[1] + LastCharge[1]) >> 1;
				LastCharge[1] = (((JoystickAxis[0][1] * -1) + 32767) >> 8) & 0xff;
			}
		}
		/* filter out jitter by not allowing rapid direction changes */
		else if (Charge[1] - LastCharge[1] < 0)	/* we are moving right */
		{
			if (LeftMotion[1])		/* moving left before ? */
			{
				if (LastCharge[1] - Charge[1] <= 4)
				{
					Charge[1] = LastCharge[1];
				}
				else
				{
					Charge[1] = (Charge[1] + LastCharge[1]) >> 1;
					LastCharge[1] = (((JoystickAxis[0][1] * -1) + 32767) >> 8) & 0xff;
					LeftMotion[1] = 0; 
				}
			}
			else
			{
				Charge[1] = (Charge[1] + LastCharge[1]) >> 1;
				LastCharge[1] = (((JoystickAxis[0][1] * -1) + 32767) >> 8) & 0xff;
			}
		}

		if (PaddleAdjust)
		{
			Charge[0] = (Charge[0] >> 1) + PaddleAdjust;
			Charge[1] = (Charge[1] >> 1) + PaddleAdjust;
		}
		if (JoystickButton[0][0]) Button[0] = 1;
		if (JoystickButton[0][1]) Button[1] = 1;
	}

	IOPortA = IOPortA | 0xc0;

	ChargeTrigger0[0] = Charge[0];
	ChargeTrigger0[1] = Charge[1];
	if (Button[0]) IOPortA = IOPortA & 0x7f;
	if (Button[1]) IOPortA = IOPortA & 0xbf;
}

void DoPaddle_R()
{
	static int KeyRepeat0 = 0;
	static unsigned int PaddleRepeat0 = 0;
	static int KeyRepeat1 = 0;
	static unsigned int PaddleRepeat1 = 0;
	static int Charge[2] = {TRIGMAX/2, TRIGMAX/2};
	static int LastCharge[2] = {TRIGMAX/2, TRIGMAX/2};
	static unsigned char Button[2] = {0, 0};
	static int LeftMotion[2] = {0, 0};

	Button[0] = 0;
	Button[1] = 0;

	if (KeyboardEnabled)
	{
		if (KeyRepeat0)
		{
			PaddleRepeat0++;
			if (PaddleRepeat0 > PaddleSensitivity) PaddleRepeat0 = 2;
		}

		if (KeyRepeat1)
		{
			PaddleRepeat1++;
			if (PaddleRepeat1 > PaddleSensitivity) PaddleRepeat1 = 2;
		}

		KeyRepeat0 = 0;
		KeyRepeat1 = 0;

		if (KeyTable[P2Right])
		{
			KeyRepeat0 = 1;
			if (Charge[0] > (PaddleRepeat0 >> 1))
				Charge[0] -= (PaddleRepeat0 >> 1);
		}
		if (KeyTable[P2Left])
		{
			KeyRepeat0 = 1;
			if ((Charge[0] + (PaddleRepeat0 >> 1)) < TRIGMAX)
				Charge[0] += (PaddleRepeat0 >> 1);
		}
		if (KeyTable[P2Up])
		{
			KeyRepeat1 = 1;
			if (Charge[1] > (PaddleRepeat1 >> 1))
				Charge[1] -= (PaddleRepeat1 >> 1);
		}
		if (KeyTable[P2Down])
		{
			KeyRepeat1 = 1;
			if ((Charge[1] + (PaddleRepeat1 >> 1)) < TRIGMAX)
				Charge[1] += (PaddleRepeat1 >> 1);
		}

		if (KeyTable[P2Fire]) Button[0] = 1;
		if (KeyTable[P2BoosterBG]) Button[1] = 1;
	}

	if (MouseEnabled)
	{
		if ((MouseBaseX == 2) || (MouseBaseX == 3))
		{
			if (MPdirection & 0x01) Charge[MouseBaseX - 2] = Charge[MouseBaseX - 2] + srv_micky_x;
			else Charge [MouseBaseX - 2] = Charge[MouseBaseX - 2] - srv_micky_x;
			if (Charge[MouseBaseX - 2] < TRIGMIN) Charge[MouseBaseX - 2] = TRIGMIN;
			if (Charge[MouseBaseX - 2] > TRIGMAX) Charge[MouseBaseX - 2] = TRIGMAX;
			if ((srv_mouse_button) && (MouseBaseY == 0xff)) Button[MouseBaseX - 2] = 1;
			if (srv_mouse_button & 0x01) Button[MouseBaseX - 2] = 1;
		}
		
		if ((MouseBaseY == 2) || (MouseBaseY == 3))
		{
			if (MPdirection & 0x02) Charge[MouseBaseY - 2] = Charge[MouseBaseY - 2] + srv_micky_y;
			else Charge [MouseBaseY - 2] = Charge[MouseBaseY - 2] - srv_micky_y;
			if (Charge[MouseBaseY - 2] < TRIGMIN) Charge[MouseBaseY - 2] = TRIGMIN;
			if (Charge[MouseBaseY - 2] > TRIGMAX) Charge[MouseBaseY - 2] = TRIGMAX;
			if ((srv_mouse_button) && (MouseBaseX == 0xff)) Button[MouseBaseY - 2] = 1;
			if (srv_mouse_button & 0x06) Button[MouseBaseY - 2] = 1;
		}
	}

	if(JoystickEnabled)
	{
		Charge[0] = (((JoystickAxis[1][0] * -1) + 32767) >> 8) & 0xff;
		Charge[1] = (((JoystickAxis[1][1] * -1) + 32767) >> 8) & 0xff;

		/* filter out jitter by not allowing rapid direction changes */
		if (Charge[0] - LastCharge[0] > 0)	/* we are moving left */
		{
			if (!LeftMotion[0])		/* moving right before ? */
			{
				if (Charge[0] - LastCharge[0] <= 4)
				{
					Charge[0] = LastCharge[0];
				}
				else
				{
					Charge[0] = (Charge[0] + LastCharge[0]) >> 1;
					LastCharge[0] = (((JoystickAxis[1][0] * -1) + 32767) >> 8) & 0xff;
					LeftMotion[0] = 1; 
				}
			}
			else
			{
				Charge[0] = (Charge[0] + LastCharge[0]) >> 1;
				LastCharge[0] = (((JoystickAxis[1][0] * -1) + 32767) >> 8) & 0xff;
			}
		}
		/* filter out jitter by not allowing rapid direction changes */
		else if (Charge[0] - LastCharge[0] < 0)	/* we are moving right */
		{
			if (LeftMotion[0])		/* moving left before ? */
			{
				if (LastCharge[0] - Charge[0] <= 4)
				{
					Charge[0] = LastCharge[0];
				}
				else
				{
					Charge[0] = (Charge[0] + LastCharge[0]) >> 1;
					LastCharge[0] = (((JoystickAxis[1][0] * -1) + 32767) >> 8) & 0xff;
					LeftMotion[0] = 0; 
				}
			}
			else
			{
				Charge[0] = (Charge[0] + LastCharge[0]) >> 1;
				LastCharge[0] = (((JoystickAxis[1][0] * -1) + 32767) >> 8) & 0xff;
			}
		}

		/* filter out jitter by not allowing rapid direction changes */
		if (Charge[1] - LastCharge[1] > 0)	/* we are moving left */
		{
			if (!LeftMotion[1])		/* moving right before ? */
			{
				if (Charge[1] - LastCharge[1] <= 4)
				{
					Charge[1] = LastCharge[1];
				}
				else
				{
					Charge[1] = (Charge[1] + LastCharge[1]) >> 1;
					LastCharge[1] = (((JoystickAxis[1][1] * -1) + 32767) >> 8) & 0xff;
					LeftMotion[1] = 1; 
				}
			}
			else
			{
				Charge[1] = (Charge[1] + LastCharge[1]) >> 1;
				LastCharge[1] = (((JoystickAxis[1][1] * -1) + 32767) >> 8) & 0xff;
			}
		}
		/* filter out jitter by not allowing rapid direction changes */
		else if (Charge[1] - LastCharge[1] < 0)	/* we are moving right */
		{
			if (LeftMotion[1])		/* moving left before ? */
			{
				if (LastCharge[1] - Charge[1] <= 4)
				{
					Charge[1] = LastCharge[1];
				}
				else
				{
					Charge[1] = (Charge[1] + LastCharge[1]) >> 1;
					LastCharge[1] = (((JoystickAxis[1][1] * -1) + 32767) >> 8) & 0xff;
					LeftMotion[1] = 0; 
				}
			}
			else
			{
				Charge[1] = (Charge[1] + LastCharge[1]) >> 1;
				LastCharge[1] = (((JoystickAxis[1][1] * -1) + 32767) >> 8) & 0xff;
			}
		}

		if (PaddleAdjust)
		{
			Charge[0] = (Charge[0] >> 1) + PaddleAdjust;
			Charge[1] = (Charge[1] >> 1) + PaddleAdjust;
		}
		if (JoystickButton[1][0]) Button[0] = 1;
		if (JoystickButton[1][1]) Button[1] = 1;
	}

	IOPortA = IOPortA | 0x0c;

	ChargeTrigger0[2] = Charge[0];
	ChargeTrigger0[3] = Charge[1];
	if (Button[0]) IOPortA = IOPortA & 0xf7;
	if (Button[1]) IOPortA = IOPortA & 0xfb;
}

void DoKeypad_L()
{
	InputLatch[0] = 0x80;
	ChargeTrigger0[0] = 0;	/* will expire immediately / will be read as 0x80 */
	ChargeTrigger0[1] = 0;
	
	if (!(IOPortA & 0x80))
	{
		if (KeyTable[P1PadPound]) InputLatch[0] = 0;
		if (KeyTable[P1PadStar]) ChargeTrigger0[0] = CHARGEMAX;
		if (KeyTable[P1Pad0]) ChargeTrigger0[1] = CHARGEMAX;
	}
	if (!(IOPortA & 0x40))
	{
		if (KeyTable[P1Pad9]) InputLatch[0] = 0;
		if (KeyTable[P1Pad7]) ChargeTrigger0[0] = CHARGEMAX;
		if (KeyTable[P1Pad8]) ChargeTrigger0[1] = CHARGEMAX;
	}
	if (!(IOPortA & 0x20))
	{
		if (KeyTable[P1Pad6]) InputLatch[0] = 0;
		if (KeyTable[P1Pad4]) ChargeTrigger0[0] = CHARGEMAX;
		if (KeyTable[P1Pad5]) ChargeTrigger0[1] = CHARGEMAX;
	}
	if (!(IOPortA & 0x10))
	{
		if (KeyTable[P1Pad3]) InputLatch[0] = 0;
		if (KeyTable[P1Pad1]) ChargeTrigger0[0] = CHARGEMAX;
		if (KeyTable[P1Pad2]) ChargeTrigger0[1] = CHARGEMAX;
	}
}

void DoKeypad_R()
{
	InputLatch[1] = 0x80;
	ChargeTrigger0[2] = 0;	/* will expire immediately / will be read as 0x80 */
	ChargeTrigger0[3] = 0;
	
	if (!(IOPortA & 0x08))
	{
		if (KeyTable[P2PadPound]) InputLatch[1] = 0;
		if (KeyTable[P2PadStar]) ChargeTrigger0[2] = CHARGEMAX;
		if (KeyTable[P2Pad0]) ChargeTrigger0[3] = CHARGEMAX;
	}
	if (!(IOPortA & 0x04))
	{
		if (KeyTable[P2Pad9]) InputLatch[1] = 0;
		if (KeyTable[P2Pad7]) ChargeTrigger0[2] = CHARGEMAX;
		if (KeyTable[P2Pad8]) ChargeTrigger0[3] = CHARGEMAX;
	}
	if (!(IOPortA & 0x02))
	{
		if (KeyTable[P2Pad6]) InputLatch[1] = 0;
		if (KeyTable[P2Pad4]) ChargeTrigger0[2] = CHARGEMAX;
		if (KeyTable[P2Pad5]) ChargeTrigger0[3] = CHARGEMAX;
	}
	if (!(IOPortA & 0x01))
	{
		if (KeyTable[P2Pad3]) InputLatch[1] = 0;
		if (KeyTable[P2Pad1]) ChargeTrigger0[2] = CHARGEMAX;
		if (KeyTable[P2Pad2]) ChargeTrigger0[3] = CHARGEMAX;
	}
}

unsigned char DCTable[4] = {0x0f, 0x0d, 0x0c, 0x0e};

void DoDriving_L()
{
	static int count = 0;

	InputLatch[0] = 0x80;

	if (KeyboardEnabled)
	{
		if (KeyTable[P1Right]) count++;
		if (KeyTable[P1Left]) count--;
		if (KeyTable[P1Fire]) InputLatch[0] = 0x00;
	}

	if (MouseEnabled)
	{
		if (!(MPdirection & 1))
		{
			if (srv_micky_x > 2) count++;
			if (srv_micky_x < -2) count--;
			if (srv_mouse_button) InputLatch[0] = 0x00;
		}
	}

	if (JoystickEnabled)
	{
		if (JoystickAxis[0][0] > 16384) count++;
		if (JoystickAxis[0][0] < -16384) count--;
		if (JoystickButton[0][0]) InputLatch[0] = 0x00;


		if (Stelladaptor[0])
		{
			IOPortA = IOPortA | 0xf0;
			if (JoystickAxis[0][1] > 16384-4096)
			{
				IOPortA = IOPortA & 0xdf;
				if (JoystickAxis[0][1] < 16384+4096)
					IOPortA = IOPortA &0xef;
			}
		}
	}

	count = count & 0x0f;
	if (!(Stelladaptor[0]))
		IOPortA = (IOPortA & 0x0f) | (DCTable[count >> 2]) << 4;

}

void DoDriving_R()
{
	static int count = 0;

	InputLatch[1] = 0x80;

	if (KeyboardEnabled)
	{
		if (KeyTable[P2Right]) count++;
		if (KeyTable[P2Left]) count--;
		if (KeyTable[P2Fire]) InputLatch[1] = 0x00;
	}

	if (MouseEnabled)
	{
		if (MPdirection & 1)
		{
			if (srv_micky_x > 2) count++;
			if (srv_micky_x < -2) count--;
			if (srv_mouse_button) InputLatch[1] = 0x00;
		}
	}

	if (JoystickEnabled)
	{
		if (JoystickAxis[1][0] > 16384) count++;
		if (JoystickAxis[1][0] < -16384) count--;
		if (JoystickButton[1][0]) InputLatch[1] = 0x00;

		if (Stelladaptor[1])
		{
			IOPortA = IOPortA | 0x0f;
			if (JoystickAxis[1][1] > 16384-4096)
			{
				IOPortA = IOPortA & 0xfd;
				if (JoystickAxis[1][1] < 16384+4096)
					IOPortA = IOPortA &0xfe;
			}
		}
	}
	count = count & 0x0f;
	if (!(Stelladaptor[1]))
		IOPortA = (IOPortA & 0xf0) | (DCTable[count >> 2]);
}
#endif

db MLG_ShotCycle = 0;	/* cycle and line of hit - gets */
dd MLG_ShotLine = 0;	/*   checked from cpuhand.asm */

/* the function gets called from CPUHAND.ASM before every INPT4 and INPT5 read */
void TestLightgunHit(dd RClock, dd ScanLine)
{
	if (LeftController == LG)
	{
		InputLatch[0] = 0x80;
		if (MLG_ShotLine <= ScanLine)
		{
			if ((MLG_ShotCycle >= LG_WrapLine) && (RClock <= 22))
			{
				if (MLG_ShotCycle <= (RClock + 76))
					InputLatch[0] = 0x00;
			}
			else if (MLG_ShotCycle <= RClock) InputLatch[0] = 0x00;
		}
	}
}

#if 0
void DoLightgun_L()
{
	static int MLG_mouseX = 320;	/* MLG_Xpos * 4 (to make motion less sensitive) */
	static int MLG_mouseY = 200;	/* MLG_YPos * 2 (to make motion less sensitive) */

	static dd MLG_Xpos = 1;		/* horizontal and vertical coordinates */
	static dd MLG_Ypos  = 1;	/*   for LG marker display */
	static db MLG_Colour = 0;	/* colour cycling the marker */

	InputLatch[0] = 0x80;

	MLG_mouseY = MLG_mouseY + srv_micky_y;
//	if (MLG_mouseY < 0) MLG_mouseY = 0;
	if (MLG_mouseY < 2) MLG_mouseY = 2;
//	if (MLG_mouseY >= 560) MLG_mouseY = 559;
	if (MLG_mouseY >= 510) MLG_mouseY = 509;
	MLG_Ypos = MLG_mouseY >> 1;	
	MLG_ShotLine = MLG_Ypos + CFirst + 4 - LGadjust;
	MLG_mouseX = MLG_mouseX + srv_micky_x;
//	if (MLG_mouseX < 0) MLG_mouseX = 0;
	if (MLG_mouseX < 4) MLG_mouseX = 4;
//	if (MLG_mouseX >= 637) MLG_mouseX = 636;
	if (MLG_mouseX >= 1276) MLG_mouseX = 1275;
	MLG_Xpos = MLG_mouseX >> 2;

//	MLG_ShotCycle = MLG_Xpos / 3 + 23 + Lightgun;
	MLG_ShotCycle = MLG_Xpos / 6 + 23 + Lightgun;

	if (MLG_ShotCycle >= 76) MLG_ShotLine++;	

	if (srv_mouse_button) IOPortA = IOPortA & 0xef;
	else IOPortA = IOPortA | 0x10;	
	MLG_Colour = (MLG_Colour + 1) & 0x07;
	
//	MLG_Xpos  += 60;

	ScreenBuffer[MLG_Ypos * 320 + MLG_Xpos + 1] = MLG_Colour;
	ScreenBuffer[MLG_Ypos * 320 + MLG_Xpos - 1] = MLG_Colour;
	ScreenBuffer[MLG_Ypos * 320 + MLG_Xpos + 320] = MLG_Colour;
	if (MLG_Ypos * 320 + MLG_Xpos > 320)
		ScreenBuffer[MLG_Ypos * 320 + MLG_Xpos - 320] = MLG_Colour;
}

void DoLightgun_R()
{
}

void DoCompumate_L()
{
}

void DoCompumate_R()
{
}

/*
** *JTZ* KidVid variables:
** TODO: "EJECT" tape when reset is pressed
*/

#define KVSMURFS 0x44
#define KVBBEARS 0x48
#define KVBLOCKS 6		/* number of bytes / block */
#define KVBLOCKBITS KVBLOCKS*8	/* number of bits / block */

//db KidVidStatus	= 0;
dd KidVidIdx = 0;
db KidVidBlock = 0;
db KidVidBlockIdx = 0;

/* number of blocks on tape: */
db KVBlocks[6] = 	{2+40,	2+21,  2+35,  /* Smurfs tapes 3, 1, 2 */
			42+60, 42+78, 42+60}; /* BBears tapes 1, 2, 3 (40 extra blocks for intro)*/

db KVData[6*8] =
{
/* KVData44 */
0x7b,	// 0111 1011b	; (1)0
0x1e,	// 0001 1110b	; 1
0xc6,	// 1100 0110b	; 00
0x31,	// 0011 0001b	; 01
0xec,	// 1110 1100b	; 0
0x60,	// 0110 0000b	; 0+

/* KVData48 */
0x7b,	// 0111 1011b	; (1)0
0x1e,	// 0001 1110b	; 1
0xc6,	// 1100 0110b	; 00
0x3d,	// 0011 1101b	; 10
0x8c,	// 1000 1100b	; 0
0x60,	// 0110 0000b	; 0+

/* KVData00 */
0xf6,	// 1111 0110b
0x31,	// 0011 0001b
0x8c,	// 1000 1100b
0x63,	// 0110 0011b
0x18,	// 0001 1000b
0xc0,	// 1100 0000b

/* KVData01 */
0xf6,	// 1111 0110b
0x31,	// 0011 0001b
0x8c,	// 1000 1100b
0x63,	// 0110 0011b
0x18,	// 0001 1000b
0xf0,	// 1111 0000b

/* KVData02 */
0xf6,	// 1111 0110b
0x31,	// 0011 0001b
0x8c,	// 1000 1100b
0x63,	// 0110 0011b
0x1e,	// 0001 1110b
0xc0,	// 1100 0000b

/* KVData03 */
0xf6,	// 1111 0110b
0x31,	// 0011 0001b
0x8c,	// 1000 1100b
0x63,	// 0110 0011b
0x1e,	// 0001 1110b
0xf0,	// 1111 0000b

/* KVPause */
0x3f,	// 0011 1111b
0xf0,	// 1111 0000b
0x00,	// 0000 0000b
0x00,	// 0000 0000b
0x00,	// 0000 0000b
0x00,	// 0000 0000b

/* KVData80 */
0xf7,	// 1111 0111b	; marks end of tape (green/yellow screen)
0xb1,	// 1011 0001b
0x8c,	// 1000 1100b
0x63,	// 0110 0011b
0x18,	// 0001 1000b
0xc0	// 1100 0000b
};

void DoKidVid_L()
{
}

void DoKidVid_R()
{
	if (KeyTable[KeyF1])
	{
		KidVidTape = 0;		/* "rewind Kid Vid tape */
		kv_CloseSampleFile();
	}
	if (KeyTable[Key1])
	{
		KidVidTape = 2;
		if (KidVid == KVBBEARS) KidVidIdx = KVBLOCKBITS;
		else KidVidIdx = 0;
		KidVidBlockIdx = KVBLOCKBITS;
		KidVidBlock = 0;
		kv_OpenSampleFile();
	}
	if (KeyTable[Key2])
	{
		KidVidTape = 3;
		if (KidVid == KVBBEARS) KidVidIdx = KVBLOCKBITS;
		else KidVidIdx = 0;
		KidVidBlockIdx = KVBLOCKBITS;
		KidVidBlock = 0;
		kv_OpenSampleFile();
	}
	if (KeyTable[Key3])
	{
		if (KidVid == KVBBEARS)		/* Berenstain Bears ? */
		{
			KidVidTape = 4;
			KidVidIdx = KVBLOCKBITS;
		}
		else				/* no, Smurf Save The Day */
		{
			KidVidTape = 1;
			KidVidIdx = 0;
		}
		KidVidBlockIdx = KVBLOCKBITS;
		KidVidBlock = 0;
		kv_OpenSampleFile();
	}

/* the "tape" is running */
	if ((KidVidTape != 0) && ((IOPortA & 0x01) == 0x01) && (kv_TapeBusy == 0))
	{
		IOPortA = (IOPortA & 0xf7) | (((KVData[KidVidIdx >> 3] << (KidVidIdx & 0x07)) & 0x80) >> 4);
		
/* increase to next bit */
		KidVidIdx++;
		KidVidBlockIdx--;
/* increase to next block (byte) */
		if (KidVidBlockIdx == 0)
		{
			if (KidVidBlock == 0)
				KidVidIdx = ((KidVidTape * 6) + 12 - KVBLOCKS) * 8; //KVData00-KVData=12
			else
			{
				if (KidVid == KVSMURFS)
				{
					if (KidVidBlock >= KVBlocks[KidVidTape - 1])
						KidVidIdx = 42 * 8; //KVData80-KVData=42
					else
					{
						KidVidIdx = 36 * 8;//KVPause-KVData=36
						kv_SetNextSong();
					}
				}
				else
				{
					if (KidVidBlock >= KVBlocks[KidVidTape + 2 - 1])				
						KidVidIdx = 42 * 8; //KVData80-KVData=42
					else
					{
						KidVidIdx = 36 * 8;//KVPause-KVData=36
						kv_SetNextSong();
					}
				}
			}
			KidVidBlock++;
			KidVidBlockIdx = KVBLOCKBITS;
		}
	}
}

//db MindlinkOR = 0x80;		/* set data bit */
//db MindlinkAND = 0x3f;	/* clear Mindlink-connected bit */
//db MindlinkTEST = 0x10;	/* test next out-bit signal */
dd MindlinkPos_L = 0x2800;	/* position value in Mindlink controller */
				/* gets transferred bitwise (16 bits) */
dd MindlinkPos_R = 0x2800;
dd MindlinkShift_L = 1;		/* which bit to transfer next */
dd MindlinkShift_R = 1;		/* which bit to transfer next */


void NextMindlinkBit_L()
{
	if (IOPortA & 0x10)
	{
		IOPortA = IOPortA & 0x3f;
		if (MindlinkPos_L & MindlinkShift_L) IOPortA = IOPortA | 0x80;
		MindlinkShift_L = MindlinkShift_L << 1;
	}
}

void NextMindlinkBit_R()
{
	if (IOPortA & 0x01)
	{
		IOPortA = IOPortA & 0xf3;
		if (MindlinkPos_R & MindlinkShift_R) IOPortA = IOPortA | 0x08;
		MindlinkShift_R = MindlinkShift_R << 1;
	}
}

void DoMindlink_L()
{
	static dd MindlinkPos1 = 0x2800;	/* position for player 1 (0x2800-0x3800) */
	static dd MindlinkPos2 = 0x1000;	/* position for player 2 (0x1000-0x2000) */

	IOPortA = IOPortA | 0xf0;
	if (MPdirection & 0x01) MindlinkPos_L = MindlinkPos2 + 0x1800;
	else MindlinkPos_L = MindlinkPos1;
	
	MindlinkPos_L = (MindlinkPos_L & 0x3fffffff) + (srv_micky_x << 3);
	if (MindlinkPos_L < 0x2800) MindlinkPos_L = 0x2800;
	if (MindlinkPos_L >= 0x3800) MindlinkPos_L = 0x3800;
	
	if (MPdirection & 0x01)
	{
		MindlinkPos2 = MindlinkPos_L - 0x1800;
		MindlinkPos_L = MindlinkPos2;
	}
	else MindlinkPos1 = MindlinkPos_L;
	MindlinkShift_L = 1;
	NextMindlinkBit_L();
	if (srv_mouse_button) MindlinkPos_L = MindlinkPos_L | 0x4000; /* this bit starts a game */
}

void DoMindlink_R()
{
	static dd MindlinkPos1 = 0x2800;	/* position for player 1 (0x2800-0x3800) */
	static dd MindlinkPos2 = 0x1000;	/* position for player 2 (0x1000-0x2000) */

	IOPortA = IOPortA | 0x0f;
	if (MPdirection & 0x01) MindlinkPos_R = MindlinkPos2 + 0x1800;
	else MindlinkPos_R = MindlinkPos1;
	
	MindlinkPos_R = (MindlinkPos_R & 0x3fffffff) + (srv_micky_x << 3);
	if (MindlinkPos_R < 0x2800) MindlinkPos_R = 0x2800;
	if (MindlinkPos_R >= 0x3800) MindlinkPos_R = 0x3800;
	
	if (MPdirection & 0x01)
	{
		MindlinkPos2 = MindlinkPos_R - 0x1800;
		MindlinkPos_R = MindlinkPos2;
	}
	else MindlinkPos1 = MindlinkPos_R;
	MindlinkShift_R = 1;
	NextMindlinkBit_R();
	if (srv_mouse_button) MindlinkPos_R = MindlinkPos_R | 0x4000; /* this bit starts a game */
}

db TrakBallController = 0;	/* Do we use any TrakBall type controller */
dd TrakBallCountV = 0;		/* how many new vertical values this frame */
dd TrakBallCountH = 0;		/* how many new horizontal values this frame */
dd TrakBallLinesV = 1;		/* how many lines to wait before sending new vert val */
dd TrakBallLinesH = 1;		/* how many lines to wait before sending new horz val */
dd TrakBallLeft = 0;		/* was TrakBall moved left or moved right instead */
dd TrakBallDown = 0;		/* was TrakBall moved down or moved up instead */
db TrakBallTableST_V[4] = {0x00, 0x10, 0x30, 0x20};		/* ST mouse / CX-80 */
db TrakBallTableST_H[4] = {0x00, 0x80, 0xc0, 0x40};		/* ST mouse / CX-80 */
db TrakBallTableAM_V[4] = {0x00, 0x80, 0xa0, 0x20};		/* Amiga mouse */
db TrakBallTableAM_H[4] = {0x00, 0x10, 0x50, 0x40};		/* Amiga mouse */
db TrakBallTableTB_V[2][2] = {{0x00, 0x10},{0x20, 0x30}};	/* CX-22 */
db TrakBallTableTB_H[2][2] = {{0x40, 0x00},{0xc0, 0x80}};	/* CX-22 */

/* this function gets called from RIOT.ASM before every SWCHA read */
void UpdateTrakBall(dd ScanLine)
{
	static dd ScanCountV = 0;
	static dd ScanCountH = 0;
	static dd CountV = 0;
	static dd CountH = 0;
	
	if (TrakBallController)
	{
		if (ScanCountV > ScanLine) ScanCountV = 0;
		if (ScanCountH > ScanLine) ScanCountH = 0;
		while ((ScanCountV + TrakBallLinesV) < ScanLine)
		{
			if (TrakBallCountV)
			{
				if (TrakBallDown) CountV--;
				else CountV++;
				TrakBallCountV--;
			}
			ScanCountV = ScanCountV + TrakBallLinesV;
		}
		
		while ((ScanCountH + TrakBallLinesH) < ScanLine)
		{
			if (TrakBallCountH)
			{
				if (TrakBallLeft) CountH--;
				else CountH++;
				TrakBallCountH--;
			}
			ScanCountH = ScanCountH + TrakBallLinesH;
		}
		CountV = CountV & 0x03;
		CountH = CountH & 0x03;

		switch (LeftController)
		{
			case ST:
				IOPortA = (IOPortA & 0x0f)
					| TrakBallTableST_V[CountV]
					| TrakBallTableST_H[CountH];
			break;
			case TB:
				IOPortA = (IOPortA & 0x0f)
					| TrakBallTableTB_V[CountV & 0x01][TrakBallDown]
					| TrakBallTableTB_H[CountH & 0x01][TrakBallLeft];
			break;
			case AM:
				IOPortA = (IOPortA & 0x0f)
					| TrakBallTableAM_V[CountV]
					| TrakBallTableAM_H[CountH];
			break;
		}		
		
		switch (RightController)
		{
			case ST:
				IOPortA = (IOPortA & 0xf0)
					| (TrakBallTableST_V[CountV] >> 4)
					| (TrakBallTableST_H[CountH] >> 4);
			break;
			case TB:
				IOPortA = (IOPortA & 0xf0)
					| (TrakBallTableTB_V[CountV & 0x01][TrakBallDown] >> 4)
					| (TrakBallTableTB_H[CountH & 0x01][TrakBallLeft] >> 4);
			break;
			case AM:
				IOPortA = (IOPortA & 0xf0)
					| (TrakBallTableAM_V[CountV] >> 4)
					| (TrakBallTableAM_H[CountH] >> 4);
			break;
		}
		SwapPorts();		
		IOPortA_Controllers = IOPortA;
	}
}

void CalculateTrakBall()
{
	TrakBallController = 1;
	if (srv_micky_y < 0) TrakBallLeft = 1;
	else TrakBallLeft = 0;
	if (srv_micky_x < 0) TrakBallDown = 0;
	else TrakBallDown = 1;
	TrakBallCountH = abs(srv_micky_y >> 1);
	TrakBallCountV = abs(srv_micky_x >> 1);
	TrakBallLinesH = LinesInFrame / (TrakBallCountH + 1);
	if (TrakBallLinesH == 0) TrakBallLinesH = 1;
	TrakBallLinesV = LinesInFrame / (TrakBallCountV + 1);
	if (TrakBallLinesV == 0) TrakBallLinesV = 1;
}

void DoMouseST_L()
{
	CalculateTrakBall();
	InputLatch[0] = 0x80;
	if(srv_mouse_button) InputLatch[0] = 0;
}

void DoMouseST_R()
{
	CalculateTrakBall();
	InputLatch[1] = 0x80;
	if(srv_mouse_button) InputLatch[1] = 0;
}

void DoTrakball_L()
{
	CalculateTrakBall();
	InputLatch[0] = 0x80;
	if(srv_mouse_button) InputLatch[0] = 0;
}

void DoTrakball_R()
{
	CalculateTrakBall();
	InputLatch[1] = 0x80;
	if(srv_mouse_button) InputLatch[1] = 0;
}

void DoMouseAmiga_L()
{
	CalculateTrakBall();
	InputLatch[0] = 0x80;
	if(srv_mouse_button) InputLatch[0] = 0;
}

void DoMouseAmiga_R()
{
	CalculateTrakBall();
	InputLatch[1] = 0x80;
	if(srv_mouse_button) InputLatch[1] = 0;
}

/*
**
** this function initiates the CompuMate keyboard controller
** it gets called from banks.asm
**
*/

void InitCompuMate()
{
	ChargeTrigger0[0] = CHARGEMAX;	/* will not expire / will be read as 0 */
	ChargeTrigger0[1] = 0;
	ChargeTrigger0[2] = 0;	/* will expire immediately / will be read as 0x80 */
	ChargeTrigger0[3] = CHARGEMAX;
}

/*
**
** this function handles the CompuMate keyboard controller
** it gets called every time something gets written to SWCHA
**
*/

void DoCompuMate_LR()
{
	ChargeTrigger0[1] = 0;	/* will expire immediately / will be read as 0x80 */
	ChargeTrigger0[2] = 0;

	InputLatch[0] = 0x80;
	InputLatch[1] = 0x80;
	ChargeTrigger0[0] = CHARGEMAX;	/* will not expire / will be read as 0 */
	ChargeTrigger0[3] = CHARGEMAX;
	IOPortA = IOPortA | (0x0c & 0x7f);

	if (KeyTable[KeyLShift]) ChargeTrigger0[3] = 0;
	if (KeyTable[KeyCtrl]) ChargeTrigger0[0] = 0;

	switch (CM_Collumn)
	{
		case 0:
			if (KeyTable[Key7]) InputLatch[0] = 0;
			if (KeyTable[KeyU]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyJ]) InputLatch[1] = 0;
			if (KeyTable[KeyM]) IOPortA = IOPortA & 0xf7;
		break;
		case 1:
			if (KeyTable[Key6]) InputLatch[0] = 0;
			if (KeyTable[KeyY]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyH]) InputLatch[1] = 0;
			if (KeyTable[KeyN]) IOPortA = IOPortA & 0xf7;
		break;
		case 2:
			if (KeyTable[Key8]) InputLatch[0] = 0;
			if (KeyTable[KeyI]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyK]) InputLatch[1] = 0;
			if (KeyTable[KeyComma]) IOPortA = IOPortA & 0xf7;
		break;
		case 3:
			if (KeyTable[Key2]) InputLatch[0] = 0;
			if (KeyTable[KeyW]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyS]) InputLatch[1] = 0;
			if (KeyTable[KeyX]) IOPortA = IOPortA & 0xf7;
		break;
		case 4:
			if (KeyTable[Key3]) InputLatch[0] = 0;
			if (KeyTable[KeyE]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyD]) InputLatch[1] = 0;
			if (KeyTable[KeyC]) IOPortA = IOPortA & 0xf7;
		break;
		case 5:
			if (KeyTable[Key0]) InputLatch[0] = 0;
			if (KeyTable[KeyP]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyColon]) InputLatch[1] = 0;
			if (KeyTable[KeySlash]) IOPortA = IOPortA & 0xf7;
		break;
		case 6:
			if (KeyTable[Key9]) InputLatch[0] = 0;
			if (KeyTable[KeyO]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyL]) InputLatch[1] = 0;
			if (KeyTable[KeyDot]) IOPortA = IOPortA & 0xf7;
		break;
		case 7:
			if (KeyTable[Key5]) InputLatch[0] = 0;
			if (KeyTable[KeyT]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyG]) InputLatch[1] = 0;
			if (KeyTable[KeyB]) IOPortA = IOPortA & 0xf7;
		break;
		case 8:
			if (KeyTable[Key1]) InputLatch[0] = 0;
			if (KeyTable[KeyQ]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyA]) InputLatch[1] = 0;
			if (KeyTable[KeyZ]) IOPortA = IOPortA & 0xf7;
		break;
		case 9:
			if (KeyTable[Key4]) InputLatch[0] = 0;
			if (KeyTable[KeyR]) IOPortA = IOPortA & 0xfb;
			if (KeyTable[KeyF]) InputLatch[1] = 0;
			if (KeyTable[KeyV]) IOPortA = IOPortA & 0xf7;
		break;
		default:
		break;
	}
}

void DoNoController_L()
{
	/* put unconnected pins in the default state */
	InputLatch[0] = 0x80;
	ChargeTrigger0[0] = CHARGEMAX;	/* will not expire / will be read as 0 */
	ChargeTrigger0[1] = CHARGEMAX;
}

void DoNoController_R()
{
	/* put unconnected pins in the default state */
	InputLatch[1] = 0x80;
	ChargeTrigger0[2] = CHARGEMAX;	/* will not expire / will be read as 0 */
	ChargeTrigger0[3] = CHARGEMAX;
}
#endif

void ControlSWCHAWrite()
{
#if 0
	if (LeftController == KP) DoKeypad_L();
	if (RightController == KP) DoKeypad_R();
	if ((LeftController == KP) || (RightController == KP))
	{
		SwapPorts();
	}
	if ((LeftController == CM) && (RightController == CM))
	{
		//DoCompuMate_LR();
		IOPortA_Controllers = 0xff;
	}
	if (LeftController == ML)
	{
		//NextMindlinkBit_L();
		IOPortA_Controllers = IOPortA_Controllers | 0xf0;
		SwapPorts();
	}
	if (RightController == ML)
	{
		//NextMindlinkBit_R();
		IOPortA_Controllers = IOPortA_Controllers | 0x0f;
		SwapPorts();
	}
#endif
}

/*
** Controller handling code
** gets called once per frame
*/

void Controls()
{
/* allow picture shifting only every couple of frames */
	static int CtrlSkipCount = 0;

	if (GamePaused && !GameReallyPaused)
	{
		if (StartInGUI)
		{
			srv_CopyScreen();
			StartInGUI = 0;
		}
		//gui();
		return;
	}

	srv_Events();			/* process SDL controller events */
	//srv_get_mouse_movement();	/* poll mouse */

	KeyTable[KeyAlt] = KeyTable[KeyLAlt] | KeyTable[KeyRAlt] | KeyTable[KeyLMeta] | KeyTable[KeyRMeta];
	KeyTable[KeyCtrl] = KeyTable[KeyLCtrl] | KeyTable[KeyRCtrl];

/* handle emulator functions */

//	if (KeyTable[KeyBackTick]) {	/* backtick calls the GUI */
//		gui();
//		KeyTable[KeyBackTick] = 0;
//	}

	if (KeyTable[KeyPgUp])		/* shift picture up */
	{
		if ((CFirst) && (!CtrlSkipCount))
		/* CFirst = 0 -> no shifting allowed */
		{
			CtrlSkipCount = 4;	/* keep shifting evrery 4 frames */
			if (CFirst != 1)	/* already at minimum position ? */
			{
				CFirst--;	/* shift picture up one scanline */
				DefaultCFirst = CFirst;	/* and remember for next game */
			}
		}
		if (CtrlSkipCount) CtrlSkipCount--;
	}

	if (KeyTable[KeyPgDn])	/* shift picture down */
	{
		if ((CFirst) && (!CtrlSkipCount))
		{
			CtrlSkipCount = 4;
			if (CFirst != 99)
			{
				CFirst++;
				DefaultCFirst = CFirst;
			}
		}
		if (CtrlSkipCount) CtrlSkipCount--;
	}

	if ((KeyTable[KeyHome]) && (OldCFirst))
	/* reset to initial position, if there is one (OldCFirst != 0) */
	{
		CFirst = OldCFirst;
		DefaultCFirst = 0xffff;	/*turn on automatic positioning */
	}	
	
/* exit emulator, if ESC or backslash are pressed */
//	ExitEmulator = (0 | KeyTable[KeyEsc] | KeyTable[KeyBackSlash]);

	if (KeyTable[KeyEsc])
	{
		KeyTable[KeyEsc] = 0;
		ExitEmulator = 128;
		GamePaused = !LaunchedFromCommandline;
		GameReallyPaused = 0;
		ROMLoaded = 0;
	}
	
// for OLPC XO-1

	if (KeyTable[SDLK_KP9])
	{
		KeyTable[SDLK_KP9] = 0;
		ExitEmulator = 128;
		GamePaused = !LaunchedFromCommandline;
		GameReallyPaused = 0;
		ROMLoaded = 0;
	}
	
	if (KeyTable[KeyBackSlash])
	{
		KeyTable[KeyBackSlash] = 0;
		ExitEmulator = 128;
		GamePaused = !LaunchedFromCommandline;
		GameReallyPaused = 0;
		ROMLoaded = 0;
	}

	if (KeyTable[KeyBackSpace]) 
	{
		GamePaused = 1;			/* pause game */
		GameReallyPaused = 1;
		KeyTable[KeyBackSpace] = 0;
	}

	if ((KeyTable[KeyEnter])&&(!KeyTable[KeyAlt])) 
	{
		GamePaused = 0;			/* resume game */
		GameReallyPaused = 0;
		//srv_reset_timing();		
	}
//	if ((KeyTable[KeyEnter])&&(KeyTable[KeyAlt])) 
//	{
//		FullScreen = !FullScreen;			/* toggle fullscreen */
//		srv_SetVideoMode();		
//	}
	
	if (KeyTable[KeyEquals])
	{
		KeyTable[KeyEquals] = 0;	/* only one screenshot per keystroke */
		//SaveScreenshot();		/* do screenshot when = is pressed */
	}

	if (KeyTable[KeyAlt])	/* change video mode only when ALT is pressed */
	{
	
// These have been disabled because changing video modes during a game doesn't currently work
// One less thing to worry about if you ask me.


		if (KeyTable[Key0])
		{
			VideoMode = 0;
			srv_SetVideoMode();	/* set new video mode */
			KeyTable[Key0] = 0;	/* make sure this happens only once ... */
			KeyTable[KeyAlt] = 0;	/* ... by marking key as unpressed */
			set_status("Sharp");
		}
		if (KeyTable[Key1])
		{
			VideoMode = 1;
			srv_SetVideoMode();
			KeyTable[Key1] = 0;
			KeyTable[KeyAlt] = 0;
			set_status("Blurry");
		}
		if (KeyTable[Key2])
		{
			VideoMode = 2;
			srv_SetVideoMode();
			KeyTable[Key2] = 0;
			KeyTable[KeyAlt] = 0;
			set_status("HD Sharp");
		}
		if (KeyTable[Key3])
		{
			VideoMode = 3;
			srv_SetVideoMode();
			KeyTable[Key3] = 0;
			KeyTable[KeyAlt] = 0;
			set_status("HD Blurry");
		}
		if (KeyTable[Key4])
		{
			VideoMode = 4;
			srv_SetVideoMode();
			KeyTable[Key4] = 0;
			KeyTable[KeyAlt] = 0;
			set_status("Fast Sharp");
		}
		if (KeyTable[Key5])
		{
			VideoMode = 5;
			srv_SetVideoMode();
			KeyTable[Key5] = 0;
			KeyTable[KeyAlt] = 0;
			set_status("Fast Blurry");
		}
//		if (KeyTable[Key6])
//		{
//			VideoMode = 6;
//			srv_SetVideoMode();
//			KeyTable[Key6] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("1280x800");
//		}
//
//		if (KeyTable[Key7])
//		{
//			VideoMode = 7;
//			srv_SetVideoMode();
//			KeyTable[Key7] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("1200x900");
//		}
//
//		if (KeyTable[Key8])
//		{
//			VideoMode = 8;
//			srv_SetVideoMode();
//			KeyTable[Key8] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("1280x1024");
//		}
//
		if (KeyTable[KeyS])
		{
			DoScanline = !DoScanline;
			srv_SetVideoMode();
			KeyTable[KeyE] = 0;
			KeyTable[KeyAlt] = 0;
			set_status("Toggle Scanline Mode");
		}

		if (KeyTable[KeyMinus])	/* cycle through palettes - NTSC, PAL, SECAM */
		{
			PaletteNumber++;
			/* make sure autodetected (0xff) palette gets changed too*/
			if (PaletteNumber == 0) PaletteNumber++;
			/* there are only 3 palettes, so start over at 0 */
			if (PaletteNumber > 2) PaletteNumber = 0;
			
			if (PaletteNumber == 0) set_status("NTSC colors");
			if (PaletteNumber == 1) set_status("PAL colors");
			if (PaletteNumber == 2) set_status("SECAM colors");
			
			srv_SetVideoMode();
			KeyTable[KeyMinus] = 0;
		}


//		/* ALT + bottom row (ZXCVBNM) disables/enables graphics */
//		/* These keys' normal functions shouldn't happen when Alt pressed */
//		if (KeyTable[KeyZ])
//		{
//			p0_mask = ~p0_mask;
//			KeyTable[KeyZ] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("toggle P0 display");
//		}
//		if (KeyTable[KeyX])
//		{
//			p1_mask = ~p1_mask;
//			KeyTable[KeyX] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("toggle P1 display");
//		}
//		if (KeyTable[KeyC])
//		{
//			m0_mask ^= 0x02;
//			KeyTable[KeyC] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("toggle M0 display");
//		}
//		if (KeyTable[KeyV])
//		{
//			m1_mask ^= 0x02;
//			KeyTable[KeyV] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("toggle M1 display");
//		}
//		if (KeyTable[KeyB])
//		{
//			bl_mask ^= 0x02;
//			KeyTable[KeyB] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("toggle BL display");
//		}
//		if (KeyTable[KeyN])
//		{
//			pf_mask ^= 0xff;
//			KeyTable[KeyN] = 0;
//			KeyTable[KeyAlt] = 0;
//			set_status("toggle PF display");
//		}

		if (KeyTable[KeyD])
		{
			ShowLineCount = !ShowLineCount;
			KeyTable[KeyD] = 0;
			KeyTable[KeyAlt] = 0;
//			set_status("toggle display");
		}
/*
		if (KeyTable[KeyM])
		{
		      if (MouseRude == 0)
			{
			      MouseRude = 1;
		      		SDL_ShowCursor(SDL_DISABLE);
		      		set_status("mouse rude");
			}
		      else
			{
			      MouseRude = 0;
		     		SDL_ShowCursor(SDL_ENABLE);
		     		set_status("mouse polite");
			}
			KeyTable[KeyM] = 0;
			KeyTable[KeyAlt] = 0;
		}
*/

		/*
			// maybe in the future we make pf0/1/2 separate.
		if (KeyTable[KeyM])
		{
			pf1_mask = ~pf1_mask;
			KeyTable[KeyM] = 0;
			KeyTable[KeyAlt] = 0;
		}
		if (KeyTable[KeyComma])
		{
			pf2_mask = ~pf2_mask;
			KeyTable[KeyComma] = 0;
			KeyTable[KeyAlt] = 0;
		}
		
		if (KeyTable[KeySlash])
		{
			p0_mask = p1_mask = m0_mask = m1_mask = bl_mask = pf_mask = 0xff;
			KeyTable[KeySlash] = 0;
			KeyTable[KeyAlt] = 0;
			set_status("all graphics enabled");
		}
		*/
	}
	
	if (KeyTable[KeyTab])	/* cycle through mouse directions for paddle */
	{
		KeyTable[KeyTab] = 0;
		MPdirection = (MPdirection - 1) & 0x03;
	}

	/* enable trace output when -t command line switch was used */
	if (KeyTable[KeyF11] && TraceEnabled) TraceCount = OldTraceCount;

	/* disable trace output when -t command line switch was used */
	if (KeyTable[KeyF12] && TraceEnabled) TraceCount = 0;

/* handle VCS console switches */	
	IOPortB = IOPortB | 0x03;	/* turn on Select and Reset bits */
	if (keys.key2 & KEY_START)
	{
		IOPortB = IOPortB & 0xfe;	/* bit 0 = RESET */
		set_status("RESET");
	}
	if (KeyTable[SelectKey]) 
	{
		IOPortB = IOPortB & 0xfd;	/* bit 1 = SELECT */
		set_status("SELECT");
	}

// for OLPC XO-1

//	if (KeyTable[SDLK_KP7]) 
//	{
//		IOPortB = IOPortB & 0xfe;	/* bit 0 = RESET */
//		set_status("RESET");
//	}
//	if (KeyTable[SDLK_KP1]) 
//	{
//		IOPortB = IOPortB & 0xfd;	/* bit 1 = SELECT */
//		set_status("SELECT");
//	}


	if (KeyTable[P0Easy]) 
	{
		IOPortB = IOPortB & 0xbf;	/* bit 6 = P0 difficulty */
		set_status("Player 0 Easy");
	}
	if (KeyTable[P0Hard]) 
	{
		IOPortB = IOPortB | 0x40;
		set_status("Player 0 Hard");
	}
	if (KeyTable[P1Easy]) 
	{
		IOPortB = IOPortB & 0x7f;	/* bit 7 = P1 difficulty */
		set_status("Player 1 Easy");
	}
	if (KeyTable[P1Hard]) 
	{
		IOPortB = IOPortB | 0x80;
		set_status("Player 1 Hard");
	}
	if (KeyTable[BWKey]) 
	{
		IOPortB = IOPortB & 0xf7;	/* bit 3 = COLOR / BW */
		set_status("Black and White");
	}
	if (KeyTable[ColorKey]) 
	{
		IOPortB = IOPortB | 0x08;
		set_status("Color");
	}

	switch (LeftController)
	{
		case JS:
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0x0f;
			DoJoystick_L();
			//DoBoosterGrip_L();
		break;
        /*
		case PC:
			DoPaddle_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0x3f;
		break;
		case KP:
			DoKeypad_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xff;
		break;
		case DC:
			DoDriving_L();
			DoBoosterGrip_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xcf;
		break;
		case LG:
			DoLightgun_L();
			DoBoosterGrip_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xef;
		break;
		case CM:
//			DoCompumate_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0x73;
		break;
		case KV:
//			DoKidVid_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0x7f;
		break;
		case ML:
			DoMindlink_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0x3f;
		break;
		case ST:
			DoMouseST_L();
			DoBoosterGrip_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0x0f;
		break;
		case TB:
			DoTrakball_L();
			DoBoosterGrip_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0x0f;
		break;
		case AM:
			DoMouseAmiga_L();
			DoBoosterGrip_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0x0f;
		break;
		case NC:
			DoNoController_L();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xff;
		break;
        */
	}
/*
	switch (RightController)
	{
		case JS:
			DoJoystick_R();
			DoBoosterGrip_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xf0;
		break;
		case PC:
			DoPaddle_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xf3;
		break;
		case KP:
			DoKeypad_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xff;
		break;
		case DC:
			DoDriving_R();
			DoBoosterGrip_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xfc;
		break;
		case LG:
//			DoLightgun_R();
			DoBoosterGrip_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xfe;
		break;
		case CM:
//			DoCompumate_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xf3;
		break;
		case KV:
			DoKidVid_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xf7;
		break;
		case ML:
			DoMindlink_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xf3;
		break;
		case ST:
			DoMouseST_R();
			DoBoosterGrip_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xf0;
		break;
		case TB:
			DoTrakball_R();
			DoBoosterGrip_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xf0;
		break;
		case AM:
			DoMouseAmiga_R();
			DoBoosterGrip_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xf0;
		break;
		case NC:
			DoNoController_R();
			IOPortA_UnusedBits = IOPortA_UnusedBits & 0xff;
		break;
	}
*/
	SwapPorts();

	/* grounded controller pins can be read even when SWACNT is set to output */
	/* IOPortA gets overwritten on output, so remember controller pins here */
	IOPortA_Controllers = IOPortA;
}

/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.  
	z26 is released subject to the terms and conditions of the
	GNU General Public License Version 2 (GPL).  
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
