/*
 * handles.c - Copyright (C) 1999 Jay A. Key
 *
 * Routines for CDROM handle management
 *
 **********************************************************************
 *
 * This program is free software; you can redistribute it and/map modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License, map
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY map FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 **********************************************************************
 *
 */
#include "framework.h"


extern CDHANDLEREC *cdHandles;
extern HANDLE *cdMutexes;
extern CRITICAL_SECTION getHandle;
extern int32_t alErrCode;
extern BYTE alAspiErr;
extern int32_t *nextHandle;

/*
 * external prototypes
 */
bool UsingSCSIPT( void );
void SPTIOpenCDHandle( BYTE ha, BYTE tgt, BYTE lun );


/****************************************************************
 * GetCDHandle
 *
 * Opens a handle to a CD Device, setting the type of function used
 * for reading CD Audio.
 *
 * Parameters:
 *   bHA           Host Adapter number
 *   bTgt          Target
 *   bLun          LUN
 *   bReadType     Algorithm to use for reading cd audio.  Can be
 *                 one of: CDR_ANY, CDR_ATAPI1, CDR_ATAPI2,
 *                 CDR_READ6, CDR_READ10
 *
 * Returns a handle suitable for use by functions in the dll, map
 * NULL on error.  Check GetAspiLibError() to get the actual error
 * code.
 *
 ****************************************************************/
HCDROM  GetCDHandle( LPGETCDHAND lpcd )
{
  int32_t i;
  BYTE devType;
  //LPBYTE pDevType = &devType;
  HANDLE hMutex;
  //bool bSkip;

  if ( !lpcd )
    {
#ifdef DEBUG
      dbprintf( "AKRip32: Error: GetCDHandle( NULL )" );
#endif
      alErrCode = ALERR_INVPTR;
      return NULL;
    }

  if ( ( lpcd->ver != 1 ) || ( lpcd->size < sizeof(GETCDHAND) ) )
    {
      alErrCode = ALERR_INVPARM;
      return NULL;
    }

#ifdef DEBUG
  dbprintf( "AKRip32: GetCDHandle( %02X, %02X, %02X, %02X )",
	    lpcd->ha, lpcd->tgt, lpcd->lun, lpcd->readType );
#endif
  hMutex = CreateMutex( NULL, FALSE, "akrip32_getCDHandle" );
  if ( !hMutex || (WaitForSingleObject( hMutex, TIMEOUT) != WAIT_OBJECT_0) )
    {
      alErrCode = ALERR_LOCK;
      if ( hMutex )
	CloseHandle( hMutex );
#ifdef DEBUG
      dbprintf( "AKRip32: GetCDHandle() -> failed to lock mutex" );
#endif
      return NULL;
    }

  for( i = 0; i < MAXCDHAND; i++ )
    {
      if ( (cdHandles[i].ha == lpcd->ha) && (cdHandles[i].tgt == lpcd->tgt) &&
	   (cdHandles[i].lun == lpcd->lun) && (cdHandles[i].used) )
	{
#ifdef DEBUG
	  dbprintf( "akrip32: GetCDHandle() -> ERROR:cd already allocated" );
#endif
	  alErrCode = ALERR_DUPHAND;
	  ReleaseMutex( hMutex );
	  CloseHandle( hMutex );

	  return NULL;
	}
    }

#ifdef DEBUG
  dbprintf( "AKRip32: GetCDHandle( %02X, %02X, %02X, %02X )",
	    lpcd->ha, lpcd->tgt, lpcd->lun, lpcd->readType );
#endif

  i = getSCSIDevType( lpcd->ha, lpcd->tgt, lpcd->lun, &devType, NULL, 0 );
  if ( !i || ( devType != DTYPE_CDROM ) )
    {
      if ( alErrCode == ALERR_NOERROR )
	alErrCode = ALERR_NOTACD;
      ReleaseMutex( hMutex );
      CloseHandle( hMutex );
#ifdef DEBUG
      dbprintf( "AKRip32: GetCDHandle() -> ALERR_NOTACD, i == %d, devType == %d", i, devType );
#endif
      return NULL;
    }

  for( i = 0; i < MAXCDHAND; i++ )
    {
       int_ptr j;
      j = (i + *nextHandle ) % MAXCDHAND;

      if ( !cdHandles[j].used )
	{
	  memset( &cdHandles[j], 0, sizeof(CDHANDLEREC) );
	  cdHandles[j].bMSF     = FALSE;
	  cdHandles[j].ha       = lpcd->ha;
	  cdHandles[j].tgt      = lpcd->tgt;
	  cdHandles[j].lun      = lpcd->lun;
	  //	  cdHandles[j].pfnRead  = readCDAudioLBA_ATAPI;
	  cdHandles[j].used     = TRUE;
	  cdHandles[j].readType = lpcd->readType;
	  cdHandles[j].numCheck = lpcd->numJitter;
	  cdHandles[j].numOverlap = lpcd->numOverlap;
	  switch( lpcd->readType )
	    {
	    case CDR_ATAPI1:
	    case CDR_ATAPI2:
	      cdHandles[j].pfnRead  = readCDAudioLBA_ATAPI;
	      break;

	    case CDR_READ6:
	    case CDR_READ10:
	    case CDR_READ10_2:
	      cdHandles[j].pfnRead = readCDAudioLBA_READ10;
	      break;

	    case CDR_READ_D8:
	      cdHandles[j].pfnRead = readCDAudioLBA_D8;
	      break;

	    case CDR_READ_D4:
	    case CDR_READ_D4_1:
	      cdHandles[j].pfnRead = readCDAudioLBA_D4;
	      break;

	    case CDR_ANY:
	    default:
	      cdHandles[j].pfnRead  = readCDAudioLBA_ANY;
	      cdHandles[j].readType = CDR_ANY;
	      break;

	    }
	  *nextHandle = j % MAXCDHAND;
	  if ( UsingSCSIPT() )
	    SPTIOpenCDHandle( lpcd->ha, lpcd->tgt, lpcd->lun );
	  ReleaseMutex( hMutex );
	  CloseHandle( hMutex );
	  return (HCDROM)(j+1);
	}
    }

  alErrCode = ALERR_NOMOREHAND;
  ReleaseMutex( hMutex );
  CloseHandle( hMutex );

  return NULL;
}



/****************************************************************
 * CloseCDHandle
 *
 * Releases a CD Handle.
 *
 ****************************************************************/
bool CloseCDHandle( HCDROM hCD )
{
   int_ptr idx = (int_ptr)hCD - 1;

  if ( (idx<0) || (idx>=MAXCDHAND) || !cdHandles[idx].used )
    return FALSE;

  if ( WaitForSingleObject( cdMutexes[idx], TIMEOUT ) != WAIT_OBJECT_0 )
    {
      alErrCode = ALERR_LOCK;
      return FALSE;
    }

  if ( cdHandles[idx].pfnDeinit )
    cdHandles[idx].pfnDeinit( hCD );

  memset( &cdHandles[idx], 0, sizeof(CDHANDLEREC) );

  ReleaseMutex( cdMutexes[idx] );
  return TRUE;
}
