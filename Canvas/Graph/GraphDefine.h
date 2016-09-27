#define Graph_TunArc_Save	1		// 在图形文件中保存隧道断面标记
#define Graph_Unit_Save		2		// 在图形文件中保存图元标记

#define Graph_Line          10		// 直线
#define Graph_Curve         11
#define Graph_Rectangle      12		// 矩形
#define Graph_RoundRectangle 13		// 带有圆弧倒角的矩形
#define Graph_Ellipse        14		// 椭圆
#define Graph_Round          15		// 圆
#define Graph_Polygon        16		// 多边形
#define Graph_LiEllipse      17		
#define Graph_RoundArc       18		// 圆弧
#define Graph_EllipseArc     19		// 椭圆弧
#define Graph_PolyBezier     20		// Bezier曲线
#define Graph_Text           21		// 文字
#define Graph_Magic			22		// 添加磁力线工具

// 判断是否支持各种画线功能
#define GH_SUPPORT_LINE			1
#define GH_SUPPORT_POLY			1
#define GH_SUPPORT_RECT			0
#define GH_SUPPORT_CIRCLE		0
#define GH_SUPPORT_ELLIPSE		0
#define GH_SUPPORT_ARC			0
#define GH_SUPPORT_MAGIC		0

#define Draw_Status          1
#define Drag_Status          2
#define Select_Status        3
#define Select_Color_Status  4
#define Change_Status        5
#define Circumrotate_Status  6

#define Max_Zoom_Factor		16			// 添加(070718)
#define Min_Zoom_Factor		0.0625	// 1/16,添加(070718)

#define Tunnel_Scale	2
#define TunArc_Scale	0.1		// 画布标准缩放比例尺
#define TunArc_Extend	10		// 扩展显示边界

#define GRAPH_LAYER_NONE	-1	// 当前无被选择的图层
#define GRAPH_LAYER_TUNARC	0	// 隧道拱形图形层
#define GRAPH_LAYER_ROCKS	1	// 岩层目标图形层
#define GRAPH_LAYER_JOINT	2	// 岩层节理图形层
#define GRAPH_LAYER_OTHER	3	// 其它信息层（如岩层产状、掌子面走向等标识）

