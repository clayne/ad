/*
**      ad -- ASCII dump
**      src/unicode.c
**
**      Copyright (C) 2015-2024  Paul J. Lucas
**
**      This program is free software: you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation, either version 3 of the License, or
**      (at your option) any later version.
**
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**      GNU General Public License for more details.
**
**      You should have received a copy of the GNU General Public License
**      along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file
 * Defines functions for working with Unicode characters.
 */

// local
#include "pjl_config.h"                 /* must go first */
#define AD_UNICODE_H_INLINE _GL_EXTERN_INLINE
#include "unicode.h"
#include "util.h"

/// @cond DOXYGEN_IGNORE

// standard
#include <assert.h>
#include <stdbool.h>

/// @endcond

/**
 * @addtogroup matching-group
 * @{
 */

///////////////////////////////////////////////////////////////////////////////

// extern constant definitions
char8_t const UTF8C_LEN_TABLE[] = {
  /*      0 1 2 3 4 5 6 7 8 9 A B C D E F */
  /* 0 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 1 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 2 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 3 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 4 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 5 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 6 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 7 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 8 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // continuation bytes
  /* 9 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //        |
  /* A */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //        |
  /* B */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //        |
  /* C */ 0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  // C0 & C1 are overlong ASCII
  /* D */ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  /* E */ 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  /* F */ 4,4,4,4,4,4,4,4,5,5,5,5,6,6,0,0
};

////////// extern functions ///////////////////////////////////////////////////

bool utf32c_8c( char32_t cp, char8_t u8c[static UTF8_CHAR_SIZE_MAX] ) {
  assert( u8c != NULL );

  static unsigned const Mask1 = 0x80;
  static unsigned const Mask2 = 0xC0;
  static unsigned const Mask3 = 0xE0;
  static unsigned const Mask4 = 0xF0;

  if ( cp < 0x80 ) {
    // 0xxxxxxx
    u8c[0] = STATIC_CAST( char8_t, cp );
  }
  else if ( cp < 0x800 ) {
    // 110xxxxx 10xxxxxx
    u8c[0] = STATIC_CAST( char8_t, Mask2 |  (cp >>  6)         );
    u8c[1] = STATIC_CAST( char8_t, Mask1 | ( cp        & 0x3F) );
  }
  else if ( cp < 0x10000 ) {
    // 1110xxxx 10xxxxxx 10xxxxxx
    u8c[0] = STATIC_CAST( char8_t, Mask3 |  (cp >> 12)         );
    u8c[1] = STATIC_CAST( char8_t, Mask1 | ((cp >>  6) & 0x3F) );
    u8c[2] = STATIC_CAST( char8_t, Mask1 | ( cp        & 0x3F) );
  }
  else if ( cp <= CP_VALID_MAX ) {
    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    u8c[0] = STATIC_CAST( char8_t, Mask4 |  (cp >> 18)         );
    u8c[1] = STATIC_CAST( char8_t, Mask1 | ((cp >> 12) & 0x3F) );
    u8c[2] = STATIC_CAST( char8_t, Mask1 | ((cp >>  6) & 0x3F) );
    u8c[3] = STATIC_CAST( char8_t, Mask1 | ( cp        & 0x3F) );
  }
  else {
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////

/** @} */

/* vim:set et sw=2 ts=2: */
