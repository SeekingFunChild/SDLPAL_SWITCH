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

#include "font.h"
#include "ascii.h"
#include "util.h"

typedef struct tagFont
{
   LPWORD           lpBufChar;
   LPBYTE           lpBufGlyph;
   INT              nChar;
} FONT, *LPFONT;

static LPFONT gpFont = NULL;

INT
PAL_InitFont(
   VOID
)
/*++
  Purpose:

    Load the font files.

  Parameters:

    None.

  Return value:

    0 if succeed, -1 if cannot allocate memory, -2 if cannot load files.

--*/
{
   FILE *fp;

   if (gpFont != NULL)
   {
      //
      // Already initialized
      //
      return 0;
   }

   gpFont = (LPFONT)calloc(1, sizeof(FONT));
   if (gpFont == NULL)
   {
      return -1;
   }

   //
   // Load the wor16.asc file.
   //
   fp = UTIL_OpenRequiredFile("WOR16.ASC");

   //
   // Get the size of wor16.asc file.
   //
   fseek(fp, 0, SEEK_END);
   gpFont->nChar = ftell(fp);
   gpFont->nChar /= 2;

   //
   // Read all the character codes.
   //
   gpFont->lpBufChar = (LPWORD)calloc(gpFont->nChar, sizeof(WORD));
   if (gpFont->lpBufChar == NULL)
   {
      free(gpFont);
      gpFont = NULL;
      return -1;
   }

   fseek(fp, 0, SEEK_SET);
   fread(gpFont->lpBufChar, sizeof(WORD), gpFont->nChar, fp);

   //
   // Close wor16.asc file.
   //
   fclose(fp);

   //
   // Read all bitmaps from wor16.fon file.
   //
   fp = UTIL_OpenRequiredFile("WOR16.FON");

   gpFont->lpBufGlyph = (LPBYTE)calloc(gpFont->nChar, 30);
   if (gpFont->lpBufGlyph == NULL)
   {
      free(gpFont->lpBufChar);
      free(gpFont);
      gpFont = NULL;
      return -1;
   }

   //
   // The font glyph data begins at offset 0x682 in wor16.fon.
   //
   fseek(fp, 0x682, SEEK_SET);
   fread(gpFont->lpBufGlyph, 30, gpFont->nChar, fp);
   fclose(fp);

   return 0;
}

VOID
PAL_FreeFont(
   VOID
)
/*++
  Purpose:

    Free the memory used for fonts.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   if (gpFont != NULL)
   {
      free(gpFont->lpBufChar);
      free(gpFont->lpBufGlyph);
      free(gpFont);
   }

   gpFont = NULL;
}

VOID
PAL_DrawCharOnSurface(
   WORD                     wChar,
   SDL_Surface             *lpSurface,
   PAL_POS                  pos,
   BYTE                     bColor
)
/*++
  Purpose:

    Draw a BIG-5 Chinese character on a surface.

  Parameters:

    [IN]  wChar - the character to be drawn (in BIG-5).

    [OUT] lpSurface - the destination surface.

    [IN]  pos - the destination location of the surface.

    [IN]  bColor - the color of the character.

  Return value:

    None.

--*/
{
   int       i, j, dx;
   int       x = PAL_X(pos), y = PAL_Y(pos);
   LPBYTE    pChar;

   //
   // Check for NULL pointer.
   //
   if (lpSurface == NULL || gpFont == NULL)
   {
      return;
   }

   //
   // Locate for this character in the font lib.
   //
   for (i = 0; i < gpFont->nChar; i++)
   {
      if (gpFont->lpBufChar[i] == wChar)
      {
         break;
      }
   }

   if (i >= gpFont->nChar)
   {
      //
      // This character does not exist in the font lib.
      //
      return;
   }

   pChar = gpFont->lpBufGlyph + i * 30;

   //
   // Draw the character to the surface.
   //
   y *= lpSurface->pitch;
   for (i = 0; i < 30; i++)
   {
      dx = x + ((i & 1) << 3);
      for (j = 0; j < 8; j++)
      {
         if (pChar[i] & (1 << (7 - j)))
         {
            ((LPBYTE)(lpSurface->pixels))[y + dx] = bColor;
         }
         dx++;
      }
      y += (i & 1) * lpSurface->pitch;
   }
}

VOID
PAL_DrawASCIICharOnSurface(
   BYTE                     bChar,
   SDL_Surface             *lpSurface,
   PAL_POS                  pos,
   BYTE                     bColor
)
/*++
  Purpose:

    Draw a ASCII character on a surface.

  Parameters:

    [IN]  bChar - the character to be drawn.

    [OUT] lpSurface - the destination surface.

    [IN]  pos - the destination location of the surface.

    [IN]  bColor - the color of the character.

  Return value:

    None.

--*/
{
   int i, j, dx;
   int x = PAL_X(pos), y = PAL_Y(pos);
   LPBYTE pChar = &iso_font[(int)(bChar & ~128) * 15];

   //
   // Check for NULL pointer.
   //
   if (lpSurface == NULL)
   {
      return;
   }

   //
   // Draw the character to the surface.
   //
   y *= lpSurface->pitch;
   for (i = 0; i < 15; i++)
   {
      dx = x;
      for (j = 0; j < 8; j++)
      {
         if (pChar[i] & (1 << j))
         {
            ((LPBYTE)(lpSurface->pixels))[y + dx] = bColor;
         }
         dx++;
      }
      y += lpSurface->pitch;
   }
}
