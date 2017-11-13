#ifndef SADDAIRLINETOLAS_H
#define SADDAIRLINETOLAS_H

#include <string>
#include "SAirLineSegment.h"

namespace SZX
{
	class AddAirLineToLas
	{
	public:
		AddAirLineToLas();
		~AddAirLineToLas();

		int setPos(std::string posFile);
		int addAirLine(std::string lasFile);

	public:
		AirLineSegment als;
	};
}

#endif