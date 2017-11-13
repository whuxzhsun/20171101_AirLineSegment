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
	const char	*name;	// 椭球名称
	double		a;		// 椭球长半轴
	double		e2;		// 椭球第一偏心率的平方
	EllipsePara()
	{
		// 默认为WGS84椭球
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
	@para	setMeridianLine	设置中央经线
	@para	setEp	设置椭球参数，默认为WGS84椭球
	@brief	初始化PROJ库
	******************************************************************************************/
	int init(double setMeridianLine, const char* setPj = Gauss_Kruger, EllipsePara setEp = EllipsePara::EllipsePara());
	/******************************************************************************************
	@para	u	in,out	传入纬度、传出X
	@para	v	in,out  传入经度、传出Y
	@brief	将经纬度坐标投影为平面坐标
	******************************************************************************************/
	int LonLat2XY(double &u, double &v);
	/******************************************************************************************
	@para	u	in,out	传入X、传出经度
	@para	v	in,out  传入Y、传出纬度
	@brief	将投影下的平面坐标转换为大地坐标
	******************************************************************************************/
	int XY2LonLat(double &u, double &v);
	/******************************************************************************************
	@para	u	in,out	传入经度、传出X
	@para	v	in,out  传入纬度、传出Y
	@para   w   in,out  传入椭球高、传出Z
	@brief	将大地坐标转换到参考椭球下的空间直角坐标
	******************************************************************************************/
	int LonLatAlt2XYZ(double &u, double &v, double &w);
	/******************************************************************************************
	@para	u	in,out	传入X、传出经度
	@para	v	in,out  传入Y、传出纬度
	@para   w   in,out  传入Z、传出椭球高
	@brief	将参考椭球下的空间直角坐标转换为大地坐标
	******************************************************************************************/
	int XYZ2LonLatAlt(double &u, double &v, double &w);

private:
	projPJ pj;
	double meridianline;
	EllipsePara ep;	// 默认WGS84
};


#endif