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
	int hour;
	int minute;
	int second;
	DateTime();
	DateTime(int y, int m, int d);
	DateTime(int y, int m, int d, int h, int min, int s);
	DateTime(std::string date);
	void ParseDateString(std::string dateString);
	std::string ToString();
	void SetCurrentDateTime();
	
	int TimeDiff(DateTime& otherDate);
	//convert a date string that looks like mon/day/year to my format
	static std::string ConvertSlashDate(std::string slashDate);

	static std::string Now();
	static std::string PrettyPrintNow();

	DateTime operator=(const DateTime& d);
	bool operator==(const DateTime& d);
	bool operator!=(const DateTime& d);

	bool operator>(const DateTime& d);

	bool operator<(const DateTime& d);

	bool operator>=(const DateTime& d);
	bool operator<=(const DateTime& d);

	int GetDayOfYear();
	void IncDay(int amt);
	void DecDay(int amt);
	void IncMonth(int amt);
	void DecMonth(int amt);
	std::string GetMonthAbrv(); //Jan
	void SetMonthFromAbrv(std::string month); //Jan
	std::string GetMonthWord(); //January
	void SetMonthFromWord(std::string month); //January
	std::string GetDayOfMonthWord();//1st,2nd, etc
	std::string PrettyPrint(bool includeTime = false);
	static std::string GetMonthWord(int month);
	static std::string GetMonthAbrv(int month);
	static std::string GetDayOfMonthWord(int day);
	static int GetDaysInMonth(int month);
};