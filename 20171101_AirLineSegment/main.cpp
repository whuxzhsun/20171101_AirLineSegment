#include "myclass.h"
#include <QtWidgets/QApplication>
#include "SAirLineSegment.h"
#include "SAddAirLineToLas.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//MyClass w;
	//w.show();

	//SZX::AirLineSegment als;
	//als.readPosFile("D:/TestData/20170804_R1000_calib/0804.pos");
	//als.convertFromXYZtoENZ(als.enzPos);
	//als.runAirLineSegment(0, als.enzPos);

	//als.outSplitAirLine("D:/TestData/20170804_R1000_calib/0804-airlinePoint-4.txt");

	SZX::AddAirLineToLas aatl;

	aatl.setPos("D:/temp_data/20171028_gxk/srcData/pos/1028_tc_2-1-3-04-best.pos");
	aatl.addAirLine("D:/temp_data/20171028_gxk/append.las");

	printf_s("%d, Done!\n", aatl.als.getAirLineCount());

	return a.exec();
}
