#pragma once
#include <string>
#include <ctime>
#include <vector>

#include "StringUtils.h"


class DateTime
{
public:
	struct Time
	{
		int hour = 0;
		int minute = 0;
		int second = 0;
		Time(int h, int m, int s);
		Time(int h, int m);
		Time();
		void Init(int h, int m, int s);
		void SetTimeFromString(std::string timeString);
		std::string ToString();
		bool IsEmpty() const;
		void IncTime(Time& otherTime);
		void DecTime(Time& otherTime);
		void IncTime(std::string timeString);
		void DecTime(std::string timeString);


		Time TimeDiff(Time& otherTime);
		int GetTimeInSeconds();
		void SetTimeFromSeconds(int seconds);
		void SetTimeFromMilis(int mili);
		void Clear();
		bool operator==(const Time& t);
		bool operator!=(const Time& t);

		bool operator>(const Time& t);

		bool operator<(const Time& t);

		bool operator>=(const Time& t);
		bool operator<=(const Time& t);
	};

	static const std::string NOW;

	int year = 1900;
	int month = 1;
	int day = 1;
	Time myTime;
	DateTime();
	DateTime(int y, int m, int d);
	DateTime(int y, int m, int d, int h, int min, int s);
	DateTime(std::string date);
	void Clear();
	void ParseDateString(std::string dateString);
	void ParseSlashDateString(std::string dateString);
	void ParseISO8601DateString(std::string dateString);
	void ParseEmailDateString(std::string dateString);
	std::string ToString(bool includeTime = false);
	
	void SetCurrentDateTime();
	
	int TimeDiff(DateTime& otherDate);
	DateTime TimeDiffAsObj(DateTime& otherDate);

	//convert a date string that looks like mon/day/year to my format
	static std::string ConvertSlashDate(std::string slashDate);

	static std::string Now();
	static std::string PrettyPrintNow();
	
	bool IsDateEqual(const DateTime& d);//use this for comparing dates only when the objs has a time element

	DateTime operator=(const DateTime& d);
	bool operator==(const DateTime& d);
	bool operator!=(const DateTime& d);

	bool operator>(const DateTime& d);

	bool operator<(const DateTime& d);

	bool operator>=(const DateTime& d);
	bool operator<=(const DateTime& d);

	bool IsEmpty();
	int GetDayOfYear();
	void IncDay(int amt);
	void DecDay(int amt);
	void IncMonth(int amt);
	void DecMonth(int amt);
	
	int ConvertDateTimeIntoSeconds();
	

	std::string GetMonthAbrv(); //Jan
	void SetMonthFromAbrv(std::string month); //Jan
	std::string GetMonthWord(); //January
	void SetMonthFromWord(std::string month); //January
	std::string GetDayOfMonthWord();//1st,2nd, etc
	std::string PrettyPrint(bool includeTime = false);
	std::string PrintFormattedString(std::string format);

	static std::string GetMonthWord(int month);
	static std::string GetMonthAbrv(int month);
	static std::string GetDayOfMonthWord(int day);
	static std::string DigitToString(int num);//add a 0 in from for digits less than 10
	static int GetDaysInMonth(int month);
	static std::string GetCurrentDayOfWeekString();
	static int GetCurrentDayOfWeek();
	static int ConvertDaysIntoSeconds(int days);
};