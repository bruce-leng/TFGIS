#include "stdafx.h"
#include "TilCore.h"

#include <stdarg.h>
#include "tilbase.h"
#include "TilError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

TILICEXPORT  void*  TilAlloc( int size )
{
	return NULL;
}

/* ÊÍ·ÅÄÚ´æ*/
TILICEXPORT  void   TilFree( void** ptr )
{
}
