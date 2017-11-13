#ifndef GEOCOORDINATECONVERT_H
#define GEOCOORDINATECONVERT_H

#ifndef GEOCORCVTDLL
#define GEOCORCVTDLLIMPORTEXPORT _declspec(dllimport)
#else
#define GEOCORCVTDLLIMPORTEXPORT _declspec(dllexport)
#endif

#include "include/proj_api.h"

#define BJ54	"krass"
#define XIAN80	"IAU76"
#define WGS84	"WGS84"
#define Gauss_Kruger	"tmerc"
#define UTM				"utm"

struct /*GEOCORCVTDLLIMPORTEXPORT*/ EllipsePara 
{
	const char	*name;	// ��������
	double		a;		// ���򳤰���
	double		e2;		// �����һƫ���ʵ�ƽ��
	EllipsePara()
	{
		// Ĭ��ΪWGS84����
		name = "WGS84";
		a	 = 6378137.0;
		e2	 = 0.00669437999013;
	}
	EllipsePara(const char *setName, double inA, double inE2)
	{
		name = setName;
		a    = inA;
		e2   = inE2;
	}
// 	EllipsePara& operator=(EllipsePara inEP)
// 	{
// 		name = inEP.name;
// 		a    = inEP.a;
// 		e2   = inEP.e2;
// 
// 		return *this;
// 	}
};

class GEOCORCVTDLLIMPORTEXPORT GeoCorCvt
{
public:
	GeoCorCvt();
	~GeoCorCvt();
	/******************************************************************************************
	@para	setMeridianLine	�������뾭��
	@para	setEp	�������������Ĭ��ΪWGS84����
	@brief	��ʼ��PROJ��
	******************************************************************************************/
	int init(double setMeridianLine, const char* setPj = Gauss_Kruger, EllipsePara setEp = EllipsePara::EllipsePara());
	/******************************************************************************************
	@para	u	in,out	����γ�ȡ�����X
	@para	v	in,out  ���뾭�ȡ�����Y
	@brief	����γ������ͶӰΪƽ������
	******************************************************************************************/
	int LonLat2XY(double &u, double &v);
	/******************************************************************************************
	@para	u	in,out	����X����������
	@para	v	in,out  ����Y������γ��
	@brief	��ͶӰ�µ�ƽ������ת��Ϊ�������
	******************************************************************************************/
	int XY2LonLat(double &u, double &v);
	/******************************************************************************************
	@para	u	in,out	���뾭�ȡ�����X
	@para	v	in,out  ����γ�ȡ�����Y
	@para   w   in,out  ��������ߡ�����Z
	@brief	���������ת�����ο������µĿռ�ֱ������
	******************************************************************************************/
	int LonLatAlt2XYZ(double &u, double &v, double &w);
	/******************************************************************************************
	@para	u	in,out	����X����������
	@para	v	in,out  ����Y������γ��
	@para   w   in,out  ����Z�����������
	@brief	���ο������µĿռ�ֱ������ת��Ϊ�������
	******************************************************************************************/
	int XYZ2LonLatAlt(double &u, double &v, double &w);

private:
	projPJ pj;
	double meridianline;
	EllipsePara ep;	// Ĭ��WGS84
};


#endif