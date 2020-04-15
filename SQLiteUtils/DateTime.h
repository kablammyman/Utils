#pragma once
#include <string>
#include <ctime>
#include <vector>

#include "StringUtils.h"


class DateTime
{
public:
	int year;
	int month;
	int day;
	DateTime();
	DateTime(std::string date);
	void ParseDateString(std::string dateString);
	std::string ToString();
	void SetCurrentDateTime();
	
	int TimeDiff(DateTime& otherDate);
	//convert a date string that looks like mon/day/year to my format
	static std::string ConvertSlashDate(std::string slashDate);

	static std::string Now();

	DateTime operator=(const DateTime& d);
	bool operator==(const DateTime& d);

	bool operator>(const DateTime& d);

	bool operator<(const DateTime& d);

	bool operator>=(const DateTime& d);
	bool operator<=(const DateTime& d);

	void IncDay(int amt);
	void DecDay(int amt);
	void IncMonth(int amt);
	void DecMonth(int amt);

	static int GetDaysInMonth(int month);
};