// TilPoint.h: interface for the TilPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILPOINT_H)
#define AFX_TILPOINT_H

typedef struct POINT_3F
{
	POINT_3F(){};
	//##ModelId=401F4142030E
	inline POINT_3F(float fX,float fY, float fZ){x = fX; y = fY; z = fZ;};
	float x;
	float y;
	float z;
} POINT_3F, *PPOINT_3F, **PPPOINT_3F;

inline POINT_3F operator + (const POINT_3F& pt1, const POINT_3F& pt2)
{
	return POINT_3F(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
};

inline POINT_3F operator - (const POINT_3F& pt1, const POINT_3F& pt2)
{
	return POINT_3F(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
};

inline POINT_3F operator * (const POINT_3F& pt1, const POINT_3F& pt2)
{
	return POINT_3F(pt1.x * pt2.x, pt1.y * pt2.y, pt1.z * pt2.z);
};

inline POINT_3F operator / (const POINT_3F& pt1, const POINT_3F& pt2)
{
	return POINT_3F(pt1.x / pt2.x, pt1.y / pt2.y, pt1.z / pt2.z);
};

inline POINT_3F& operator += (POINT_3F& pt1, const POINT_3F& pt2)
{
	pt1.x += pt2.x;
	pt1.y += pt2.y;
	pt1.z += pt2.z;
	return pt1;
};

inline POINT_3F& operator -= (POINT_3F& pt1, const POINT_3F& pt2)
{
	pt1.x -= pt2.x;
	pt1.y -= pt2.y;
	pt1.z -= pt2.z;
	return pt1;
};

inline POINT_3F operator *= (POINT_3F& pt1, const POINT_3F& pt2)
{
	pt1.x *= pt2.x;
	pt1.y *= pt2.y;
	pt1.z *= pt2.z;
	return pt1;
};

inline POINT_3F operator / (POINT_3F& pt1, const POINT_3F& pt2)
{
	pt1.x /= pt2.x;
	pt1.y /= pt2.y;
	pt1.z /= pt2.z;
	return pt1;
};

inline bool operator == (const POINT_3F& pt1, const POINT_3F& pt2)
{
	if (pt1.x==pt2.x && pt1.y==pt2.y&&pt1.z==pt2.z)
		return true;
	return false;
}

inline bool operator != (const POINT_3F& pt1, const POINT_3F& pt2)
{
	if (pt1.x!=pt2.x || pt1.y!=pt2.y || pt1.z!=pt2.z)
		return true;
	return false;
}

//##ModelId=401F414202CE
struct POINT2DN
{
	//##ModelId=401F414202DE
	inline POINT2DN(int Dx,int Dy){x = Dx; y=Dy;};
	//##ModelId=401F414202F0
	int x;
	//##ModelId=401F414202F1
	int y;
};

inline POINT2DN operator + (const POINT2DN& pt1, const POINT2DN& pt2)
{
	return POINT2DN(pt1.x + pt2.x, pt1.y + pt2.y);
};

inline POINT2DN operator - (const POINT2DN& pt1, const POINT2DN& pt2)
{
	return POINT2DN(pt1.x - pt2.x, pt1.y - pt2.y);
};

inline POINT2DN operator * (const POINT2DN& pt1, const POINT2DN& pt2)
{
	return POINT2DN(pt1.x * pt2.x, pt1.y * pt2.y);
};

inline POINT2DN operator / (const POINT2DN& pt1, const POINT2DN& pt2)
{
	return POINT2DN(pt1.x / pt2.x, pt1.y / pt2.y);
};

inline POINT2DN& operator += (POINT2DN& pt1, const POINT2DN& pt2)
{
	pt1.x += pt2.x;
	pt1.y += pt2.y;
	return pt1;
};

inline POINT2DN& operator -= (POINT2DN& pt1, const POINT2DN& pt2)
{
	pt1.x -= pt2.x;
	pt1.y -= pt2.y;
	return pt1;
};

inline POINT2DN operator *= (POINT2DN& pt1, const POINT2DN& pt2)
{
	pt1.x *= pt2.x;
	pt1.y *= pt2.y;
	return pt1;
};

inline POINT2DN operator / (POINT2DN& pt1, const POINT2DN& pt2)
{
	pt1.x /= pt2.x;
	pt1.y /= pt2.y;
	return pt1;
};

//##ModelId=401F414202FD
typedef struct POINT2DF
{
	POINT2DF(){};
	//##ModelId=401F4142030E
	inline POINT2DF(double Dx,double Dy){x = Dx; y=Dy;};
	//##ModelId=401F4142031D
	double x;
	//##ModelId=401F4142032C
	double y;
}POINT2DF, *PPOINT2DF, **PPPOINT2DF;

inline POINT2DF operator + (const POINT2DF& pt1, const POINT2DF& pt2)
{
	return POINT2DF(pt1.x + pt2.x, pt1.y + pt2.y);
};

inline POINT2DF operator - (const POINT2DF& pt1, const POINT2DF& pt2)
{
	return POINT2DF(pt1.x - pt2.x, pt1.y - pt2.y);
};

inline POINT2DF operator * (const POINT2DF& pt1, const POINT2DF& pt2)
{
	return POINT2DF(pt1.x * pt2.x, pt1.y * pt2.y);
};

inline POINT2DF operator / (const POINT2DF& pt1, const POINT2DF& pt2)
{
	return POINT2DF(pt1.x / pt2.x, pt1.y / pt2.y);
};

inline POINT2DF& operator += (POINT2DF& pt1, const POINT2DF& pt2)
{
	pt1.x += pt2.x;
	pt1.y += pt2.y;
	return pt1;
};

inline POINT2DF& operator -= (POINT2DF& pt1, const POINT2DF& pt2)
{
	pt1.x -= pt2.x;
	pt1.y -= pt2.y;
	return pt1;
};

inline POINT2DF operator *= (POINT2DF& pt1, const POINT2DF& pt2)
{
	pt1.x *= pt2.x;
	pt1.y *= pt2.y;
	return pt1;
};

inline POINT2DF operator / (POINT2DF& pt1, const POINT2DF& pt2)
{
	pt1.x /= pt2.x;
	pt1.y /= pt2.y;
	return pt1;
};

//##ModelId=401F4142033C
struct POINT3DN
{
	//##ModelId=401F4142033E
	inline POINT3DN(int Dx,int Dy, int Dz){x = Dx; y=Dy; z = Dz;};
	//##ModelId=401F4142034C
	int x;
	//##ModelId=401F4142034D
	int y;
	//##ModelId=401F4142035B
	int z;
};

inline POINT3DN operator + (const POINT3DN& pt1, const POINT3DN& pt2)
{
	return POINT3DN(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
};

inline POINT3DN operator - (const POINT3DN& pt1, const POINT3DN& pt2)
{
	return POINT3DN(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
};

inline POINT3DN operator * (const POINT3DN& pt1, const POINT3DN& pt2)
{
	return POINT3DN(pt1.x * pt2.x, pt1.y * pt2.y, pt1.z * pt2.z);
};

inline POINT3DN operator / (const POINT3DN& pt1, const POINT3DN& pt2)
{
	return POINT3DN(pt1.x / pt2.x, pt1.y / pt2.y, pt1.z / pt2.z);
};

inline POINT3DN& operator += (POINT3DN& pt1, const POINT3DN& pt2)
{
	pt1.x += pt2.x;
	pt1.y += pt2.y;
	pt1.z += pt2.z;
	return pt1;
};

inline POINT3DN& operator -= (POINT3DN& pt1, const POINT3DN& pt2)
{
	pt1.x -= pt2.x;
	pt1.y -= pt2.y;
	pt1.z -= pt2.z;
	return pt1;
};

inline POINT3DN operator *= (POINT3DN& pt1, const POINT3DN& pt2)
{
	pt1.x *= pt2.x;
	pt1.y *= pt2.y;
	pt1.z *= pt2.z;
	return pt1;
};

inline POINT3DN operator /= (POINT3DN& pt1, const POINT3DN& pt2)
{
	pt1.x /= pt2.x;
	pt1.y /= pt2.y;
	pt1.z /= pt2.z;
	return pt1;
};

//##ModelId=401F4142035C
typedef struct POINT3DF
{
	POINT3DF(){};
	//##ModelId=401F4142036C
	inline POINT3DF(double Dx, double Dy, double Dz);
	//##ModelId=401F4142037A
	double x;
	//##ModelId=401F4142037B
	double y;
	//##ModelId=401F4142038A
	double z;
}POINT3DF;

inline POINT3DF operator + (const POINT3DF& pt1, const POINT3DF& pt2)
{
	return POINT3DF(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
};

inline POINT3DF operator - (const POINT3DF& pt1, const POINT3DF& pt2)
{
	return POINT3DF(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
};

inline POINT3DF operator * (const POINT3DF& pt1, const POINT3DF& pt2)
{
	return POINT3DF(pt1.x * pt2.x, pt1.y * pt2.y, pt1.z * pt2.z);
};

inline POINT3DF operator / (const POINT3DF& pt1, const POINT3DF& pt2)
{
	return POINT3DF(pt1.x / pt2.x, pt1.y / pt2.y, pt1.z / pt2.z);
};

inline POINT3DF& operator += (POINT3DF& pt1, const POINT3DF& pt2)
{
	pt1.x += pt2.x;
	pt1.y += pt2.y;
	pt1.z += pt2.z;
	return pt1;
};

inline POINT3DF& operator -= (POINT3DF& pt1, const POINT3DF& pt2)
{
	pt1.x -= pt2.x;
	pt1.y -= pt2.y;
	pt1.z -= pt2.z;
	return pt1;
};

inline POINT3DF operator *= (POINT3DF& pt1, const POINT3DF& pt2)
{
	pt1.x *= pt2.x;
	pt1.y *= pt2.y;
	pt1.z *= pt2.z;
	return pt1;
};

inline POINT3DF operator / (POINT3DF& pt1, const POINT3DF& pt2)
{
	pt1.x /= pt2.x;
	pt1.y /= pt2.y;
	pt1.z /= pt2.z;
	return pt1;
};

//##ModelId=401F41420399
struct POINT4DF
{
	//##ModelId=401F4142039B
	inline POINT4DF(double Dx, double Dy, double Dz, double Ds = 1)
	{x=Dx, y=Dy, z = Dz, s = Ds;};
	//##ModelId=401F414203AD
	double x;
	//##ModelId=401F414203B9
	double y;
	//##ModelId=401F414203BA
	double z;
	//##ModelId=401F414203BB
	double s;
};

inline POINT4DF operator + (const POINT4DF& pt1, const POINT4DF& pt2)
{
	return POINT4DF(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
};

inline POINT4DF operator - (const POINT4DF& pt1, const POINT4DF& pt2)
{
	return POINT4DF(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
};

inline POINT4DF operator * (const POINT4DF& pt1, const POINT4DF& pt2)
{
	return POINT4DF(pt1.x * pt2.x, pt1.y * pt2.y, pt1.z * pt2.z);
};

inline POINT4DF operator / (const POINT4DF& pt1, const POINT4DF& pt2)
{
	return POINT4DF(pt1.x / pt2.x, pt1.y / pt2.y, pt1.z / pt2.z);
};

inline POINT4DF& operator += (POINT4DF& pt1, const POINT4DF& pt2)
{
	pt1.x += pt2.x;
	pt1.y += pt2.y;
	pt1.z += pt2.z;
	return pt1;
};

inline POINT4DF& operator -= (POINT4DF& pt1, const POINT4DF& pt2)
{
	pt1.x -= pt2.x;
	pt1.y -= pt2.y;
	pt1.z -= pt2.z;
	return pt1;
};

inline POINT4DF operator *= (POINT4DF& pt1, const POINT4DF& pt2)
{
	pt1.x *= pt2.x;
	pt1.y *= pt2.y;
	pt1.z *= pt2.z;
	return pt1;
};

inline POINT4DF operator / (POINT4DF& pt1, const POINT4DF& pt2)
{
	pt1.x /= pt2.x;
	pt1.y /= pt2.y;
	pt1.z /= pt2.z;
	return pt1;
};

//##ModelId=401F414203C8
struct POINT4DN
{
	//##ModelId=401F414203D9
	inline POINT4DN(double Dx, double Dy, double Dz, double Ds = 1)
	{x=Dx, y=Dy, z = Dz, s = Ds;};
	//##ModelId=401F41430001
	double x;
	//##ModelId=401F41430002
	double y;
	//##ModelId=401F4143000F
	double z;
	//##ModelId=401F41430010
	double s;
};

inline POINT4DN operator + (const POINT4DN& pt1, const POINT4DN& pt2)
{
	return POINT4DN(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
};

inline POINT4DN operator - (const POINT4DN& pt1, const POINT4DN& pt2)
{
	return POINT4DN(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
};

inline POINT4DN operator * (const POINT4DN& pt1, const POINT4DN& pt2)
{
	return POINT4DN(pt1.x * pt2.x, pt1.y * pt2.y, pt1.z * pt2.z);
};

inline POINT4DN operator / (const POINT4DN& pt1, const POINT4DN& pt2)
{
	return POINT4DN(pt1.x / pt2.x, pt1.y / pt2.y, pt1.z / pt2.z);
};

inline POINT4DN& operator += (POINT4DN& pt1, const POINT4DN& pt2)
{
	pt1.x += pt2.x;
	pt1.y += pt2.y;
	pt1.z += pt2.z;
	return pt1;
};

inline POINT4DN& operator -= (POINT4DN& pt1, const POINT4DN& pt2)
{
	pt1.x -= pt2.x;
	pt1.y -= pt2.y;
	pt1.z -= pt2.z;
	return pt1;
};

inline POINT4DN operator *= (POINT4DN& pt1, const POINT4DN& pt2)
{
	pt1.x *= pt2.x;
	pt1.y *= pt2.y;
	pt1.z *= pt2.z;
	return pt1;
};

inline POINT4DN operator / (POINT4DN& pt1, const POINT4DN& pt2)
{
	pt1.x /= pt2.x;
	pt1.y /= pt2.y;
	pt1.z /= pt2.z;
	return pt1;
};

//掌子面的空间位置描述
//##ModelId=401F4143002E
struct SLICE_COORD 
{
public:
	//结构大小=sizeof(SLICE_COORD)
	//##ModelId=401F4143009C
	long lSize;

	//定位点坐标X
	//##ModelId=401F4143009D
	double dbAnchorX;

	//定位点坐标Y
	//##ModelId=401F4143009E
	double dbAnchorY;

	//X方向的分辨率
	//##ModelId=401F414300AB
	double dbScaleX;

	//Y方向分辨率
	//##ModelId=401F414300AC
	double dbScaleY;

	//与XOY平面的夹角
	//##ModelId=401F414300BB
	double dbRotateXY;

	//与XOZ平面的夹角
	//##ModelId=401F414300BC
	double dbRotateXZ;

	//与YZ平面的夹角
	//##ModelId=401F414300CB
	double dbRotateYZ;
	
	//坐标数目
	//##ModelId=401F432D038A
	int m_nCoordNum;

	//bIsScreenCoord = TRUE:
	//xyCoord记录的是屏幕坐标，空间坐标需要结合结构中其他参数
	//换算
	//
	//bIsScreenCoord = FALSE:
	//xyCoord记录的是世界坐标,将忽略其他参数.
	//##ModelId=401F41430030
	bool bIsScreenCoord;

	//##ModelId=401F576603A9
	POINT2DF* pxyCoord;
};

#endif // !defined(AFX_TILPOINT_H__40C12392_582C_48C1_91AE_740F658ACE91__INCLUDED_)
