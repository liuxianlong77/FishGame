#include "FishData.hpp"

#include "jai_csv.h"

FishData::FishData()
{
	jai::SimpleCSV csv;
	csv.importFromString(Lex::GetDataOfFile(Lex::GetResPath("text/fish.csv").data()));

	auto & rows = csv.getRows();

	for (auto & row : rows)
	{
		musts(row.size() == 16, "");
		Data data;
		int index = 0;
		data.type = jai::StringToNumber<int>(row[index++]);
		data.name = row[index++];
		data.pay = jai::StringToNumber<int>(row[index++]);
		data.payEx = jai::StringToNumber<int>(row[index++]);
		data.group = jai::StringToNumber<int>(row[index++]);
		data.groupEx = jai::StringToNumber<int>(row[index++]);
		data.path_type = jai::StringToNumber<int>(row[index++]);
		data.ani_normal = jai::StringToNumber<int>(row[index++]);
		data.ani_dying = jai::StringToNumber<int>(row[index++]);
		data.min_speed = jai::StringToNumber<int>(row[index++]);
		data.max_speed = jai::StringToNumber<int>(row[index++]);
		data.silver_coin = jai::StringToNumber<int>(row[index++]);
		data.gold_coin = jai::StringToNumber<int>(row[index++]);
		data.break_type = jai::StringToNumber<int>(row[index++]);
		data.circle_type = jai::StringToNumber<int>(row[index++]);
		data.rate = jai::StringToNumber<int>(row[index++]);
		mData[data.type] = data;
	}
	int rateAcc = 0;
	for (auto & x : mData)
	{
		rateAcc += x.second.rate;
		mRateAcc.push_back(std::make_pair(x.first, rateAcc));
	}
	mRateAcc.push_back(std::make_pair(0, rateAcc));
}

int FishData::nextType()
{
	int n = LexRand(mRateAcc.back().second);
	for (auto & x : mRateAcc)
	{
		if(n < x.second) return x.first;
	}
	return 0;
}
FishData * FishData::GetShared()
{
	static FishData * self = new FishData;
	return self;
}

FishData::Data * FishData::getInfo(int type)
{
	auto iter = mData.find(type);
	if(iter == mData.end()) return nullptr;
	return &(iter->second);
}
