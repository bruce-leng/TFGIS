#define Graph_TunArc_Save	1		// ��ͼ���ļ��б������������
#define Graph_Unit_Save		2		// ��ͼ���ļ��б���ͼԪ���

#define Graph_Line          10		// ֱ��
#define Graph_Curve         11
#define Graph_Rectangle      12		// ����
#define Graph_RoundRectangle 13		// ����Բ�����ǵľ���
#define Graph_Ellipse        14		// ��Բ
#define Graph_Round          15		// Բ
#define Graph_Polygon        16		// �����
#define Graph_LiEllipse      17		
#define Graph_RoundArc       18		// Բ��
#define Graph_EllipseArc     19		// ��Բ��
#define Graph_PolyBezier     20		// Bezier����
#define Graph_Text           21		// ����
#define Graph_Magic			22		// ��Ӵ����߹���

// �ж��Ƿ�֧�ָ��ֻ��߹���
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

#define Max_Zoom_Factor		16			// ���(070718)
#define Min_Zoom_Factor		0.0625	// 1/16,���(070718)

#define Tunnel_Scale	2
#define TunArc_Scale	0.1		// ������׼���ű�����
#define TunArc_Extend	10		// ��չ��ʾ�߽�

#define GRAPH_LAYER_NONE	-1	// ��ǰ�ޱ�ѡ���ͼ��
#define GRAPH_LAYER_TUNARC	0	// �������ͼ�β�
#define GRAPH_LAYER_ROCKS	1	// �Ҳ�Ŀ��ͼ�β�
#define GRAPH_LAYER_JOINT	2	// �Ҳ����ͼ�β�
#define GRAPH_LAYER_OTHER	3	// ������Ϣ�㣨���Ҳ��״������������ȱ�ʶ��

