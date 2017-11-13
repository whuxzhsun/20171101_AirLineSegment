#include "SAddAirLineToLas.h"
#include <fstream>
#include "liblas/liblas.hpp"
#include "liblas/header.hpp"
#include "liblas/point.hpp"
#include "liblas/writer.hpp"

using namespace SZX;
using namespace std;

AddAirLineToLas::AddAirLineToLas()
{

}

AddAirLineToLas::~AddAirLineToLas()
{

}

int AddAirLineToLas::setPos(std::string posFile)
{
	int result = als.readPosFile(posFile.c_str());
	result += als.convertFromXYZtoENZ(als.enzPos);
	result += als.runAirLineSegment(1, als.enzPos);

	return result;
}

int AddAirLineToLas::addAirLine(std::string lasFile)
{
	int k = lasFile.find_last_of(".");
	string outlas = lasFile.substr(0, k - 1) + "_flightLine.las";

	ifstream inLas(lasFile, ios::in | ios::binary);
	liblas::Reader readLas(inLas);
	liblas::Header const &hdrLas = readLas.GetHeader();

	unsigned int point_count = 0;
	std::ofstream ofs(outlas, std::ios::out | std::ios::binary);
	liblas::Header header;

	// 设置文件头，点数、格式、缩放因子、偏移量

	header.SetVersionMajor(1);
	header.SetVersionMinor(2);
	header.SetDataFormatId(liblas::PointFormatName::ePointFormat3);
	header.SetScale(0.001, 0.001, 0.001);

	double px = hdrLas.GetOffsetX();
	double py = hdrLas.GetOffsetY();

	header.SetOffset(int(px), int(py), 0);

	liblas::Writer writer(ofs, header);

	liblas::Point point(&header);
	liblas::Point srcPt(&hdrLas);

	double minPt[3] = { 9999999, 9999999, 9999999 };
	double maxPt[3] = { 0, 0, 0 };
	double pt[3] = { 0 };
	double height = 0;
	while (readLas.ReadNextPoint())
	{
		srcPt = readLas.GetPoint();

		point.SetX(srcPt.GetX());
		point.SetY(srcPt.GetY());
		point.SetZ(srcPt.GetZ());

		point.SetPointSourceID(srcPt.GetPointSourceID());
		point.SetColor(srcPt.GetColor());
		point.SetIntensity(srcPt.GetIntensity());
		point.SetTime(srcPt.GetTime());
		point.SetPointSourceID(als.getPointInAirLine(srcPt.GetTime()));

		writer.WritePoint(point);

		point_count++;
	}

	header.SetPointRecordsCount(point_count);
	header.SetMax(maxPt[0], maxPt[1], maxPt[2]);
	header.SetMin(minPt[0], minPt[1], minPt[2]);

	writer.SetHeader(header);
	writer.WriteHeader();

	return 0;
}