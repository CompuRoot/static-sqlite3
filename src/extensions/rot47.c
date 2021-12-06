/*
** 2021-12-05
**
** Copyright (C) 2021 by CompuRoot
**
** Permission to use, copy, modify, and/or distribute this software for any
** purpose with or without fee is hereby granted.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
** WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES OF
** MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
** ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
** WHATSOEVER RESULTING FROM LOSS OF USE,  DATA OR PROFITS,  WHETHER  IN AN
** ACTION  OF  CONTRACT,  NEGLIGENCE OR OTHER TORTIOUS ACTION,  ARISING OUT
** OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE
**
******************************************************************************
**
** This SQLite extension implements a rot47() function and a rot47
** collating sequence.
*/
#include "sqlite3ext.h"

SQLITE_EXTENSION_INIT1
#include <assert.h>
#include <string.h>

/*
** Perform rot47 monoalphabetic substitution on a single ASCII character.
** Encode all visible English ASCII 94 characters set in range
** ASCII DEC 33 to ASCII DEC 126 (regex [!-~]) using substitution cipher,
** similar to Caesar's cipher, that's why named as rot47 (94/2=47).
** Algorithm can be explained by modular arithmetic as: 33+((ASCII_char+14)%94)
** where ASCII_char is a single byte in range: 33-126
** Implementation don't use 33+((ASCII_char+14)%94) formula but faster analog.
**
** Most of the code is taken from official sqlite3 extension repository from
** ext/misc/rot13.c source file. The only rot47 function written from scratch.
**
** Lookup table for rot47:
**  !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
**  PQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO
**
*/
static unsigned char rot47(unsigned char c){
  if(c>='!' && c<='O'){
    c += 47;
  }else if(c>='P' && c<='~'){
    c -= 47;
  }
  return c;
}

/*
** Implementation of the rot47() function.
**
** Rotate ASCII alphabetic characters by 47 character positions.
** Non-ASCII characters are unchanged.  rot47(rot47(X)) should always
** equal X.
*/
static void rot47func(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  const unsigned char *zIn;
  int nIn;
  unsigned char *zOut;
  unsigned char *zToFree = 0;
  int i;
  unsigned char zTemp[127];
  assert( argc==1 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  zIn = (const unsigned char*)sqlite3_value_text(argv[0]);
  nIn = sqlite3_value_bytes(argv[0]);
  if( nIn<sizeof(zTemp)-1 ){
    zOut = zTemp;
  }else{
    zOut = zToFree = (unsigned char*)sqlite3_malloc64( nIn+1 );
    if( zOut==0 ){
      sqlite3_result_error_nomem(context);
      return;
    }
  }
  for(i=0; i<nIn; i++) zOut[i] = rot47(zIn[i]);
  zOut[i] = 0;
  sqlite3_result_text(context, (char*)zOut, i, SQLITE_TRANSIENT);
  sqlite3_free(zToFree);
}

/*
** Implement the rot47 collating sequence so that if
**
**      x=y COLLATE rot47
**
** Then
**
**      rot47(x)=rot47(y) COLLATE binary
*/
static int rot47CollFunc(
  void *notUsed,
  int nKey1, const void *pKey1,
  int nKey2, const void *pKey2
){
  const char *zA = (const char*)pKey1;
  const char *zB = (const char*)pKey2;
  int i, x;
  for(i=0; i<nKey1 && i<nKey2; i++){
    x = (int)rot47(zA[i]) - (int)rot47(zB[i]);
    if( x!=0 ) return x;
  }
  return nKey1 - nKey2;
}


#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_rot47_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;  /* Unused parameter */

  rc = sqlite3_create_function(db, "rot47", 1,
                   SQLITE_UTF8|SQLITE_INNOCUOUS|SQLITE_DETERMINISTIC,
                   0, rot47func, 0, 0);

  if( rc==SQLITE_OK ){
    rc = sqlite3_create_collation(db, "rot47", SQLITE_UTF8, 0, rot47CollFunc);
  }

  return rc;
}
