#include "SAirLineSegment.h"
#include <cmath>
#include "geoCoordinateConvert.h"

#define angleThresVal 30

using namespace SZX;

/*
	如果计算失败，则返回-1，不设置航线号
*/

AirLineSegment::AirLineSegment() : preId(0)
{

}

AirLineSegment::~AirLineSegment()
{

}

int AirLineSegment::runAirLineSegment(int platformType, std::vector<Pos> pos)
{
	if (pos.size() < 1)
	{
		return -1;
	}

	double samplingDistance = 0;
	if (platformType == 0)
	{
		samplingDistance = 32;
	}
	else if (platformType == 1)
	{
		samplingDistance = 256;
	}
	else
	{
		return -1;
	}

	Pos tpPos;
	std::vector<Pos> tempRes;
	double diffHeading;
	double sumDist = 0;

	tpPos = pos[0];
	for (size_t i = 1; i < pos.size(); i++)
	{
		if (sumDist < samplingDistance)
		{
			sumDist += sqrt(pow(pos[i].x - pos[i - 1].x, 2) + pow(pos[i].y - pos[i - 1].y, 2));
		}
		else
		{
			sumDist = 0;
			diffHeading = abs(pos[i].yaw - tpPos.yaw) * RAD_TO_DEG;
			if (diffHeading > angleThresVal && diffHeading < 360 - angleThresVal) // 判断是否分段
			{
				tempRes.push_back(tpPos);
			}
			tpPos = pos[i];
		}
	}

	std::vector<double> dist;
	double tpDist;
	int n = tempRes.size();

	for (int i = 1; i < n; i++)
	{
		tpDist = sqrt(pow(tempRes[i].x - tempRes[i - 1].x, 2) + pow(tempRes[i].y - tempRes[i - 1].y, 2));
		dist.push_back(tpDist);
	}
	double meanDist = samplingDistance * 2;

	std::vector<int> judgeVal(n, 0);
	int record[1024] = { 0 };
	for (int i = 0; i < n - 1; i++)
	{
		int j = 0;
		if (dist[i] > meanDist && judgeVal[i] != -1) // 长间距则保存
		{
			judgeVal[i] = 1;
			judgeVal[i + 1] = 1;
		}
		else
		{
			if (dist[i] < meanDist)
			{
				judgeVal[i] = -1;
				judgeVal[i + 1] = -1;
				record[j++] = i;
				for (i = i + 1; i < n - 1; i++)
				{
					// 如果连续短间距，则标记不可用
					if (dist[i] < meanDist)
					{
						judgeVal[i] = -1;
						judgeVal[i + 1] = -1;
						record[j++] = i;
					}
					else
					{
						break;
					}
				}

				// 此处是关键，添加连续短间隔的最后一个节点
				record[j++] = i;
				// 将连续短间距的中间点置为节点
				judgeVal[record[int(1.0 * j / 2)]] = 1;
				i = i - 1;
			}
		}
		for (j = 0; j < 1024; j++)	// 清0 
		{
			record[j] = 0;
		}
	}

	for (int i = 0; i < n; i++)
	{
		if (i == 0 || i == n - 1)
		{
			cutPos.push_back(tempRes[i]);
		}
		else
		{
			if (judgeVal[i] == 1)
			{
				cutPos.push_back(tempRes[i]);
			}
		}
	}

	return 0;
}

int AirLineSegment::getPointInAirLine(double time)
{
	int id = 0;

	int n = cutPos.size();
	for (int i = preId; i < n; i++)
	{
		if (time >= cutPos[preId].t)
		{
			id = preId + 1;
			if (time < cutPos[id].t)
			{
				break;	//航线号为id = preId + 1;
			}
			else
			{
				for (int j = id; j < n; j++)
				{
					if (time < cutPos[j].t)
					{
						id = j;
						break;	//直到搜索出下一个
					}
				}
				break;
			}
		}
		else
		{
			id = preId;
			if (time > cutPos[id - 1].t)
			{
				break;	//航线号 id = preId
			}
			else
			{
				for (int j = id - 1; j >= 0; j--)
				{
					if (time > cutPos[j].t)
					{
						id = j + 1;
						break;
					}
				}
				break;
			}
		}
	}

	return id;
}

int AirLineSegment::convertFromXYZtoENZ(std::vector<Pos> pos)
{
	if (pos.size() < 1)
	{
		return -1;
	}

	GeoCorCvt gcc;

	enzPos.resize(pos.size());

	for (size_t i = 0; i < pos.size(); i++)
	{
		enzPos[i] = pos[i];

		gcc.XYZ2LonLatAlt(enzPos[i].x, enzPos[i].y, enzPos[i].z);

		if (i == 0)
		{
			int centerLine = int((enzPos[i].x + 1.5) / 3) * 3;
			gcc.init(centerLine);
		}

		gcc.LonLat2XY(enzPos[i].x, enzPos[i].y);
	}

	return 0;
}

int AirLineSegment::readPosFile(const char *posFile)
{
	FILE *fp = NULL;
	fopen_s(&fp, posFile, "r");
	if (fp == NULL)
	{
		return -1;
	}

	int jjh;
	char headstring[512];

	for (jjh = 0; jjh<40; jjh++)
	{
		fgets(headstring, 512, fp);
	}

	GeoCorCvt gcc;
	gcc.init(114);

	Pos pos;
	while (!feof(fp))
	{
		char lineStr[512];
		if (fgets(lineStr, 512, fp))
		{
			sscanf_s(lineStr, "%lf%lf%lf%lf%lf%lf%lf",
				&pos.t, &pos.y, &pos.x, &pos.z, &pos.r, &pos.p, &pos.yaw);

			gcc.LonLatAlt2XYZ(pos.x, pos.y, pos.z);

			pos.yaw = pos.yaw * DEG_TO_RAD;
			pos.p = -pos.p   * DEG_TO_RAD;
			pos.r = -pos.r   * DEG_TO_RAD;

			enzPos.push_back(pos);
		}
	}

	fclose(fp);

	return 0;
}

int AirLineSegment::outSplitAirLine(const char *outFile)
{
	FILE *fp;
	fopen_s(&fp, outFile, "w");
	if (!fp)
	{
		return -1;
	}

	for (size_t i = 0; i < cutPos.size(); i++)
	{
		//fprintf(fp, "%d %.3lf %.3lf %.3lf %.3lf %.3lf\n", 
		//	i, cutPos[i].x, cutPos[i].y, cutPos[i].z, cutPos[i].t, cutPos[(i + 1) % cutPos.size()].t);
		fprintf(fp, "%.3lf %.3lf %.3lf\n",
			cutPos[i].x, cutPos[i].y, cutPos[i].z);
	}

	fclose(fp);

	return 0;
}