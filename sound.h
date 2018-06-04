//
// Copyright (c) 2009, Wei Mingzhi <whistler_wmz@users.sf.net>.
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

#ifndef SOUND_H
#define SOUND_H

#include "common.h"

#ifndef PAL_SAMPLE_RATE
#define PAL_SAMPLE_RATE     49716
#endif

#ifndef PAL_CHANNELS
#define PAL_CHANNELS        2
#endif

#ifdef __cplusplus
extern "C"
{
#endif
extern INT          g_iVolume;
INT
SOUND_OpenAudio(
   VOID
);

VOID
SOUND_CloseAudio(
   VOID
);

VOID
SOUND_PlayChannel(
   INT    iSoundNum,
   INT    iChannel
);

VOID
SOUND_AdjustVolume(
   INT    iDirectory
);

VOID
PAL_PlayMUS(
   INT       iNumRIX,
   BOOL      fLoop,
   FLOAT     flFadeTime
);

#define SOUND_Play(i) SOUND_PlayChannel((i), 0)

extern BOOL       g_fNoSound;
extern BOOL       g_fNoMusic;
#ifdef PAL_HAS_NATIVEMIDI
extern BOOL       g_fUseMidi;
#endif

#ifdef __cplusplus
}
#endif

#endif
