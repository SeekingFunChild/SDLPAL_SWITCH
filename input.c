//
// Copyright (c) 2009, Wei Mingzhi <whistler_wmz@users.sf.net>.
// Portions Copyright (c) 2009, netwan.
//
// All rights reserved.
//
// This file is part of SDLPAL.
//
// SDLPAL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "main.h"
#include <math.h>
#include "SDL_events.h"
#include <switch.h>

PALINPUTSTATE            g_InputState;
#ifdef PAL_HAS_JOYSTICKS
static SDL_Joystick     *g_pJoy = NULL;
#endif
BOOL                     g_fUseJoystick = TRUE;

#if defined(GPH)
#define MIN_DEADZONE -16384
#define MAX_DEADZONE 16384
#endif
int get_dir_by_key(DWORD key)
{
	BOOL isLeftPress = (key & kKeyLeft) != 0;
	BOOL isRightPress = (key & kKeyRight) != 0;
	BOOL isUpPress = (key & kKeyUp) != 0;
	BOOL isDownPress = (key & kKeyDown) != 0;
	int keyNumber = isLeftPress + isRightPress + isUpPress + isDownPress;

	// ֻ��һ����������ð죬ֱ��ȡ��������
	if (keyNumber == 0) {
		return kDirUnknown;
	} else if (keyNumber == 1) {
		if (isLeftPress) {
			return kDirWest;
		} else if (isRightPress) {
			return kDirEast;
		} else if (isUpPress) {
			return kDirNorth;
		} else if (isDownPress) {
			return kDirSouth;
		} else {
			return kDirUnknown;
		}
	}

	switch (g_InputState.dir) {
	case kDirNorth:
		if (isUpPress) {
			return kDirNorth;
		} else if (isLeftPress) {
			return kDirWest;
		} else if (isRightPress) {
			return kDirEast;
		} else if (isDownPress) {
			return kDirSouth;
		}
		break;
	case kDirSouth:
		if (isDownPress) {
			return kDirSouth;
		} else if (isLeftPress) {
			return kDirWest;
		} else if (isRightPress) {
			return kDirEast;
		} else if (isUpPress) {
			return kDirNorth;
		}
		break;
	case kDirWest:
		if (isLeftPress) {
			return kDirWest;
		} else if (isUpPress) {
			return kDirNorth;
		} else if (isDownPress) {
			return kDirSouth;
		} else if (isRightPress) {
			return kDirEast;
		} 
		break;
	case kDirEast:
		if (isRightPress) {
			return kDirEast;
		} else if (isUpPress) {
			return kDirNorth;
		} else if (isDownPress) {
			return kDirSouth;
		} else if (isLeftPress) {
			return kDirWest;
		}
		break;
	case kDirUnknown:
		return kDirUnknown;
		break;
	}

	return kDirUnknown;
}

VOID
PAL_KeyboardEventFilter(
   const SDL_Event       *lpEvent
)
/*++
  Purpose:

    Handle keyboard events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
   switch (lpEvent->type)
   {
   case SDL_KEYDOWN:
      switch (lpEvent->key.keysym.sym)
      {
        case SDLK_PLUS: //Pressed + (Exit program)...
          PAL_Shutdown();
          exit(0);

        case SDLK_UP:
          g_InputState.dir = kDirNorth;
          g_InputState.dwKeyPress |=kKeyUp;
          g_InputState.dirKeyPress |= kKeyUp;
          break;

        case SDLK_DOWN:
          g_InputState.dir = kDirSouth;
          g_InputState.dwKeyPress |= kKeyDown;
          g_InputState.dirKeyPress |= kKeyDown;
          break;

        case SDLK_LEFT:
          g_InputState.dir = kDirWest;
          g_InputState.dwKeyPress |= kKeyLeft;
          g_InputState.dirKeyPress |= kKeyLeft;
          break;

        case SDLK_RIGHT:
          g_InputState.dir = kDirEast;
          g_InputState.dwKeyPress |= kKeyRight;
          g_InputState.dirKeyPress |= kKeyRight;
          break;

        case SDLK_MINUS: //switch "-" key
        case SDLK_b:
          g_InputState.dwKeyPress |= kKeyMenu;
          break;

        case SDLK_a:  //switch "A" key
          g_InputState.dwKeyPress |= kKeySearch;
          break;

        case SDLK_l:  //switch "L" key
          g_InputState.dwKeyPress |= kKeyPgUp;
          break;

        case SDLK_r:  //switch "R" key
          g_InputState.dwKeyPress |= kKeyPgDn;
          break;

        case SDLK_7: //7 for mobile device
        //case SDLK_r:
          g_InputState.dwKeyPress |= kKeyRepeat;
          break;

        case SDLK_2: //2 for mobile device
        //case SDLK_a:
          g_InputState.dwKeyPress |= kKeyAuto;
          break;

        case SDLK_d:
          g_InputState.dwKeyPress |= kKeyDefend;
          break;

        case SDLK_x:
          g_InputState.dwKeyPress |= kKeyUseItem;
          break;

        case SDLK_w:
          g_InputState.dwKeyPress |= kKeyThrowItem;
          break;

        case SDLK_q:
          g_InputState.dwKeyPress |= kKeyFlee;
          break;

        case SDLK_y:
          g_InputState.dwKeyPress |= kKeyStatus;
          break;

        case SDLK_f:
        case SDLK_5: // 5 for mobile device
          g_InputState.dwKeyPress |= kKeyForce;
          break;

        case SDLK_HASH: //# for mobile device
        case SDLK_p:
          VIDEO_SaveScreenshot();
          break;

        default:
          break;
      }
      break;

   case SDL_KEYUP:
      //
      // Released a key
      //
      switch (lpEvent->key.keysym.sym)
      {
      case SDLK_UP:
		  if ((g_InputState.dirKeyPress & kKeyUp) != 0) {
			  g_InputState.dirKeyPress ^= kKeyUp;
		  }
		  
         g_InputState.dir = get_dir_by_key(g_InputState.dirKeyPress);
         break;

      case SDLK_DOWN:
		  if ((g_InputState.dirKeyPress & kKeyDown) != 0) {
			  g_InputState.dirKeyPress ^= kKeyDown;
		  }
         g_InputState.dir = get_dir_by_key(g_InputState.dirKeyPress);
         break;

      case SDLK_LEFT:
		  if ((g_InputState.dirKeyPress & kKeyLeft) != 0) {
			  g_InputState.dirKeyPress ^= kKeyLeft;
		  }
         g_InputState.dir = get_dir_by_key(g_InputState.dirKeyPress);
         break;

      case SDLK_RIGHT:
		  if ((g_InputState.dirKeyPress & kKeyRight) != 0) {
			  g_InputState.dirKeyPress ^= kKeyRight;
		  }
         g_InputState.dir = get_dir_by_key(g_InputState.dirKeyPress);
         break;

      default:
         break;
      }
      break;
   }
}


int GetInputDir(int mouseX, int mouseY)
{
	int nScrW = 320;
	int nScrH = 200;
    
	// Ŀǰ���������ƺ���������Ļ���ģ���ʹ�ߵ���ͼ����;
	int nHeroX = nScrW / 2;
	int nHeroY = nScrH / 2;
    
	int nDeltaX = mouseX - nHeroX;
	int nDeltaY = nHeroY - mouseY;
    
    if (nDeltaY >= 0 && nDeltaX >= 0) {
        return kDirNorth;
    } else if (nDeltaY >= 0 && nDeltaX < 0) {
        return kDirWest;
    } else if (nDeltaY < 0 && nDeltaX >= 0) {
        return kDirEast;
    } else if (nDeltaY < 0 && nDeltaX < 0) {
        return kDirSouth;
    }
    
    return kDirUnknown;
}

int GetMouseMoveDir(int nMouseX, int nMouseY)
{
	// why use initial ? Maybe fix later 
	int nScrW = 320;
	int nScrH = 200;

	// Ŀǰ���������ƺ���������Ļ���ģ���ʹ�ߵ���ͼ����;
	int nHeroX = nScrW / 2;
	int nHeroY = nScrH / 2;

	int nDeltaX = nMouseX - nHeroX;
	int nDeltaY = nHeroY - nMouseY;

	int nAbsDeltaX = abs(nDeltaX);
	int nAbsDeltaY = abs(nDeltaY);

	int nDir = eMoveDirUnknown;

	if (nDeltaX > 0) {
		if (nDeltaY > 0) {
			// ��һ����,N;
			if (nAbsDeltaX > nAbsDeltaY) {
				nDir = eMoveDirNE;
			} else if (nAbsDeltaX == nAbsDeltaY) {
				nDir = eMoveDirNorth;
			} else {
				nDir = eMoveDirNW;
			}
		} else {
			// ��������,E;
			if (nAbsDeltaX > nAbsDeltaY) {
				nDir = eMoveDirEN;
			} else if (nAbsDeltaX == nAbsDeltaY) {
				nDir = eMoveDirEast;
			} else {
				nDir = eMoveDirES;
			}
		}
	} else {
		if (nDeltaY > 0) {
			// �ڶ�����,W;
			if (nAbsDeltaX > nAbsDeltaY) {
				nDir = eMoveDirWS;
			} else if (nAbsDeltaX == nAbsDeltaY) {
				nDir = eMoveDirWest;
			} else {
				nDir = eMoveDirWN;
			}
		} else {
			// ��������,S;
			if (nAbsDeltaX > nAbsDeltaY) {
				nDir = eMoveDirSW;
			} else if (nAbsDeltaX == nAbsDeltaY) {
				nDir = eMoveDirSouth;
			} else {
				nDir = eMoveDirSE;
			}
		}
	}

	return nDir;
}

VOID
PAL_MouseEventFilter(
   const SDL_Event *lpEvent
)
/*++
  Purpose:

    Handle mouse events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
#ifdef PAL_HAS_MOUSE
   static short hitTest = 0; // Double click detect;

   double       screenWidth, gridWidth;
   double       screenHeight, gridHeight;
   double       mx, my;
   double       thumbx;
   double       thumby;
   INT          gridIndex;
   static INT   lastReleaseButtonTime, lastPressButtonTime;
   static INT   lastPressx = 0;
   static INT   lastPressy = 0;
   static INT   lastReleasex = 0;
   static INT   lastReleasey = 0;
   static INT   lastMotionX = 0;
   static INT   lastMotionY = 0;
   int mouseX, mouseY;
   int eventType;
#ifdef __ANDROID__
   eventType = lpEvent->tfinger.type;
    if (lpEvent->tfinger.type!= SDL_FINGERDOWN && lpEvent->tfinger.type != SDL_FINGERUP
	   && lpEvent->tfinger.type != SDL_FINGERMOTION)
      return;
#else
    eventType = lpEvent->type;
   if (lpEvent->type!= SDL_MOUSEBUTTONDOWN && lpEvent->type != SDL_MOUSEBUTTONUP
	   && lpEvent->type != SDL_MOUSEMOTION)
      return;
#endif
   screenWidth = g_wInitialWidth;
   screenHeight = g_wInitialHeight;
   gridWidth = screenWidth / 3;
   gridHeight = screenHeight / 3;

#ifdef __ANDROID__
   mx = lpEvent->tfinger.x;
   my = lpEvent->tfinger.y;
   mouseX = lpEvent->tfinger.x;
   mouseY = lpEvent->tfinger.y;
#else
   mx = lpEvent->button.x;
   my = lpEvent->button.y;
   mouseX = lpEvent->button.x;
   mouseY = lpEvent->button.y;
#endif

   thumbx = ceil(mx / gridWidth);
   thumby = floor(my / gridHeight);
   gridIndex = thumbx + thumby * 3 - 1;
  
   // б45�ȣ���������б���ߵ�
/*
0   1   2
3   4   5
6   7   8
*/

   g_InputState.controlType = CONTROL_TYPE_NONE;

   switch (eventType)
   {
#ifdef __ANDROID__
       case SDL_FINGERDOWN:
#else
   case SDL_MOUSEBUTTONDOWN:
#endif
//   LOGI("SDL_FINGERDOWN: %d  %d  %d     %d  %d", eventType, mouseX,  mouseY, g_wInitialWidth, g_wInitialHeight);
      lastPressButtonTime = SDL_GetTicks();

      lastPressx = mouseX;
      lastPressy = mouseY;

	  g_InputState.touchEventType = TOUCH_DOWN;
	  g_InputState.touchX = mouseX * 320.0 / g_wInitialWidth;
	  g_InputState.touchY = mouseY * 200.0 / g_wInitialHeight;
      
       g_InputState.controlType = CONTROL_TYPE_MOUSE_WALK;
       g_InputState.nMoveDir = GetMouseMoveDir(g_InputState.touchX, g_InputState.touchY);
           	  
	  switch (gridIndex)
	  {
	  case 2:
		  g_InputState.dir = kDirNorth;
		  break;
	  case 6:
		  g_InputState.dir = kDirSouth;
		  break;
	  case 0:
		  g_InputState.dir = kDirWest;
		  break;
	  case 8:
		  g_InputState.dir = kDirEast;
		  break;
	  case 1:
		  g_InputState.dir = kDirNorth;
		  break;
	  case 7:
		  g_InputState.dir = kDirSouth; 
		  break;
	  case 3:
		  g_InputState.dir = kDirWest;
		  break;
	  case 5:
		  g_InputState.dir = kDirEast;
		  break;
       case 4:
              g_InputState.dir = GetInputDir(g_InputState.touchX, g_InputState.touchY);
        break;
	  }
      break;
#ifdef __ANDROID__
       case SDL_FINGERMOTION:
#else
   case SDL_MOUSEMOTION:
	   if (lpEvent->motion.state == 0) {
		   break;
	   }
#endif
       if (mouseX == lastMotionX && mouseY == lastMotionY) {
           break;
       }
//  LOGI("SDL_MOUSEMOTION: %d  %d  %d     %d  %d", eventType, mouseX,  mouseY, g_wInitialWidth, g_wInitialHeight);
    lastMotionX = mouseX;
       lastMotionY = mouseY;

       g_InputState.controlType = CONTROL_TYPE_MOUSE_WALK;
       g_InputState.nMoveDir = GetMouseMoveDir(mouseX * 320.0 / g_wInitialWidth, mouseY * 200.0 / g_wInitialHeight);

	  switch (gridIndex)
	  {
	  case 2:
		  g_InputState.dir = kDirNorth;
		  break;
	  case 6:
		  g_InputState.dir = kDirSouth;
		  break;
	  case 0:
		  g_InputState.dir = kDirWest;
		  break;
	  case 8:
		  g_InputState.dir = kDirEast;
		  break;
	  case 1:
		  g_InputState.dir = kDirNorth;
		  break;
	  case 7:
		  g_InputState.dir = kDirSouth; 
		  break;
	  case 3:
		  g_InputState.dir = kDirWest;
		  break;
	  case 5:
		  g_InputState.dir = kDirEast;
		  break;
          case 4:
              g_InputState.dir = GetInputDir(g_InputState.touchX, g_InputState.touchY);
	  }
	   break;
#ifdef __ANDROID__
       case SDL_FINGERUP:
#else
   case SDL_MOUSEBUTTONUP:
#endif
//     LOGI("SDL_FINGERUP: %d  %d  %d     %d  %d", eventType, mouseX,  mouseY, g_wInitialWidth, g_wInitialHeight);
       lastReleaseButtonTime = SDL_GetTicks();
      lastReleasex = mouseX;
      lastReleasey = mouseY;
      hitTest ++;

	  if (lastReleaseButtonTime - lastPressButtonTime <= 1000) {
		g_InputState.touchEventType = TOUCH_UP;
		g_InputState.touchX = mouseX * 320.0 / g_wInitialWidth;
		g_InputState.touchY = mouseY * 200.0 / g_wInitialHeight;
	  } else {
		g_InputState.touchEventType = TOUCH_NONE;
	  }
           
       if (gridIndex == 4) {
           if (abs(lastReleasex - lastPressx) <= 25
               && abs(lastReleasey - lastPressy) <= 25) {
               if (lastReleaseButtonTime - lastPressButtonTime <= 500) {
                   g_InputState.dwKeyPress |= kKeyMainSearch;
               }
           }
       }
               
	  g_InputState.dir = kDirUnknown;
	  g_InputState.nMoveDir = eMoveDirUnknown;
      break;
   }
#endif
}

VOID
PAL_JoystickEventFilter(
   const SDL_Event       *lpEvent
)
/*++
  Purpose:

    Handle joystick events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
  static BOOL joyhasmotioned = FALSE;
  static int threshold = 18000;

  static JoystickPosition pos_left;
  hidJoystickRead(&pos_left, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
  if(abs(pos_left.dx)<=threshold&&abs(pos_left.dy)<=threshold)
  {
    if(joyhasmotioned)
    {
      if ((g_InputState.dirKeyPress & kKeyUp) != 0) {
        g_InputState.dirKeyPress ^= kKeyUp;
      }
      if ((g_InputState.dirKeyPress & kKeyDown) != 0) {
        g_InputState.dirKeyPress ^= kKeyDown;
      }
      if ((g_InputState.dirKeyPress & kKeyLeft) != 0) {
        g_InputState.dirKeyPress ^= kKeyLeft;
      }
      if ((g_InputState.dirKeyPress & kKeyRight) != 0) {
        g_InputState.dirKeyPress ^= kKeyRight;
      }
      g_InputState.dir = get_dir_by_key(g_InputState.dirKeyPress);

      joyhasmotioned=FALSE;
    }
    else
    {
      return;
    }

  }
  else
  {
    if(abs(pos_left.dx)>abs(pos_left.dy))
    {
      if(pos_left.dx>threshold)  //right
      {
          g_InputState.dir = kDirEast;
          g_InputState.dwKeyPress |=kKeyRight;
          g_InputState.dirKeyPress |= kKeyRight;       

      }
      else if(pos_left.dx<(0-threshold)) //left
      {
          g_InputState.dir = kDirWest;
          g_InputState.dwKeyPress |=kKeyLeft;
          g_InputState.dirKeyPress |= kKeyLeft;   
      }
    }
    else
    {
      if(pos_left.dy>threshold)  //up
      {
          g_InputState.dir = kDirNorth;
          g_InputState.dwKeyPress |=kKeyUp;
          g_InputState.dirKeyPress |= kKeyUp;
      }
      else if(pos_left.dy<(0-threshold)) //down
      {
          g_InputState.dir = kDirSouth;
          g_InputState.dwKeyPress |=kKeyDown;
          g_InputState.dirKeyPress |= kKeyDown;
      }
    }

    joyhasmotioned = TRUE;
  }
}

#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION <= 2
static int SDLCALL
PAL_EventFilter(
   const SDL_Event       *lpEvent
)
#else
int PAL_EventFilter(
   void                  *userdata,
   const SDL_Event       *lpEvent
)
#endif
/*++
  Purpose:

    SDL event filter function. A filter to process all events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    1 = the event will be added to the internal queue.
    0 = the event will be dropped from the queue.

--*/
{
   PAL_KeyboardEventFilter(lpEvent);
   PAL_MouseEventFilter(lpEvent);

   //
   // All events are handled here; don't put anything to the internal queue
   //
   return 0;
}

VOID
PAL_ClearKeyState(
   VOID
)
/*++
  Purpose:

    Clear the record of pressed keys.

  Parameters:

    None.

  Return value:

    None.

--*/
{   
   g_InputState.dwKeyPress = 0;
    g_InputState.touchEventType = TOUCH_NONE;   
}

VOID
PAL_InitInput(
   VOID
)
/*++
  Purpose:

    Initialize the input subsystem.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   memset(&g_InputState, 0, sizeof(g_InputState));
   g_InputState.dir = kDirUnknown;

   SDL_SetEventFilter(PAL_EventFilter);

   //
   // Check for joystick
   //
#ifdef PAL_HAS_JOYSTICKS
   if (SDL_NumJoysticks() > 0 && g_fUseJoystick)
   {
      g_pJoy = SDL_JoystickOpen(0);
      if (g_pJoy != NULL)
      {
         SDL_JoystickEventState(SDL_ENABLE);
      }
   }
#endif
}

VOID
PAL_ShutdownInput(
   VOID
)
/*++
  Purpose:

    Shutdown the input subsystem.

  Parameters:

    None.

  Return value:

    None.

--*/
{
#ifdef PAL_HAS_JOYSTICKS
   if (SDL_JoystickOpened(0))
   {
      assert(g_pJoy != NULL);
      SDL_JoystickClose(g_pJoy);
      g_pJoy = NULL;
   }
#endif
}

VOID
PAL_ProcessEvent(
   VOID
)
/*++
  Purpose:

    Process all events.

  Parameters:

    None.

  Return value:

    None.

--*/
{
  PAL_JoystickEventFilter(NULL);

  SDL_Event evt;
	while (SDL_PollEvent(&evt)) {

   }
}

BOOL PAL_IsTouch(int x, int y, int w, int h)
{
//	LOGI("is touch: %d %d %d %d %d %d", x, y, w, h, g_InputState.touchX, g_InputState.touchY);
	if (g_InputState.touchEventType == TOUCH_NONE) {
		return FALSE;
	}

//	LOGI("yes is touch: %d %d %d %d %d %d", x, y, w, h, g_InputState.touchX, g_InputState.touchY);
	if (g_InputState.touchX >= x && g_InputState.touchX <= x + w
		&& g_InputState.touchY >= y && g_InputState.touchY <= y + h) {
//			LOGI("yes is touch in: %d %d %d %d %d %d", x, y, w, h, g_InputState.touchX, g_InputState.touchY);
			return TRUE;
	}

	return FALSE;
}

BOOL PAL_IsTouchDown(int x, int y, int w, int h)
{
	if (g_InputState.touchEventType != TOUCH_DOWN) {
		return FALSE;
	}

	if (g_InputState.touchX >= x && g_InputState.touchX <= x + w
		&& g_InputState.touchY >= y && g_InputState.touchY <= y + h) {
			return TRUE;
	}

	return FALSE;
}

BOOL PAL_IsTouchUp(int x, int y, int w, int h)
{
	if (g_InputState.touchEventType != TOUCH_UP) {
		return FALSE;
	}

	if (g_InputState.touchX >= x && g_InputState.touchX <= x + w
		&& g_InputState.touchY >= y && g_InputState.touchY <= y + h) {
			return TRUE;
	}

	return FALSE;
}
