#ifndef __TILCORE_0001_H__
#define __TILCORE_0001_H__

#define TILICEXPORT __declspec(dllexport)
#define TILICIMPORT __declspec(dllimport)

/*分配内存，如果内存分配失败，将会引发异常*/
TILICEXPORT  void*  TilAlloc( int size );
/* 释放内存*/
TILICEXPORT  void   TilFree( void** ptr );

TILICEXPORT  void*  TilAlloc2D(int nWidth, int nHeight, int nDepth);
TILICEXPORT  void*  TilFree2D();

#endif //__TILCORE_0001_H__