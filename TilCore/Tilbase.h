#ifndef __TILCORE_0001_H__
#define __TILCORE_0001_H__

#define TILICEXPORT __declspec(dllexport)
#define TILICIMPORT __declspec(dllimport)

/*�����ڴ棬����ڴ����ʧ�ܣ����������쳣*/
TILICEXPORT  void*  TilAlloc( int size );
/* �ͷ��ڴ�*/
TILICEXPORT  void   TilFree( void** ptr );

TILICEXPORT  void*  TilAlloc2D(int nWidth, int nHeight, int nDepth);
TILICEXPORT  void*  TilFree2D();

#endif //__TILCORE_0001_H__