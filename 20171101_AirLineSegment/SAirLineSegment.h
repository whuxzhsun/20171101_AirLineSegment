#ifndef SAIRLIENSEGMENT_H
#define SAIRLIENSEGMENT_H

#include <vector>
#include "geoCoordinateConvert.h"

struct Pos{
	double x;
	double y;
	double z;
	double r;
	double p;
	double yaw;
	double t;
	double latitude;
	double longitude;
	double standardDeviation;
	int	   numSatellites;
	Pos& operator=(Pos& val)
	{
		x = val.x;	y = val.y;	z = val.z;	t = val.t;
		r = val.r;	p = val.p;	yaw = val.yaw;
		latitude = val.latitude;
		longitude = val.longitude;
		standardDeviation = 0;
		numSatellites = 0;
		return *this;
	}
};

namespace SZX
{
	class AirLineSegment
	{
	public:
		AirLineSegment();
		~AirLineSegment();

		int runAirLineSegment(int platformType, std::vector<Pos> pos);
		int getPointInAirLine(double time);
		int getAirLineCount() { return cutPos.size(); }

		std::vector<Pos> enzPos;

		/*
			for test
		*/
		int convertFromXYZtoENZ(std::vector<Pos> pos);
		int readPosFile(const char *posFile);

		int outSplitAirLine(const char *outFile);

	private:
		size_t preId;
		std::vector<Pos> cutPos;
	};
}

#endif