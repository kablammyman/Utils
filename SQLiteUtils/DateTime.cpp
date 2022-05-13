#include "DateTime.h"

DateTime::Time::Time(int h, int m, int s)
{
	Init(h, m, s);
}
DateTime::Time::Time(int h, int m)
{
	hour = h;
	minute = m;
	second = 0;
}
DateTime::Time::Time()
{
	hour = 0;
	minute = 0;
	second = 0;
}


void DateTime::Time::Init(int h, int m, int s)
{
	hour = h;
	minute = m;
	second = s;
}

void DateTime::Time::SetTimeFromString(std::string timeString)
{
	std::vector<std::string> timeVec = StringUtils::Tokenize(timeString, ":");
	if (timeVec.size() < 2)
		return;
	hour = StringUtils::GetIntFromString(timeVec[0]);
	minute = StringUtils::GetIntFromString(timeVec[1]);
	if (timeVec.size() > 2)
		second = StringUtils::GetIntFromString(timeVec[2]);
}

std::string DateTime::Time::ToString()
{
	std::string ret = std::to_string(hour) + ":";
	if (minute < 10)
		ret += "0";
	ret += std::to_string(minute) + ":";
	if (second > 0)
	{
		if (second < 10)
			ret += "0";
	}
	ret += std::to_string(second);

	return ret;
}

bool DateTime::Time::IsEmpty() const
{
	if (hour == 0 && minute == 0 && second == 0)
		return true;
	return false;
}

void DateTime::Time::Clear()
{
	hour = 0; 
	minute = 0; 
	second = 0;
}

//DateTime DateTime::operator=(const DateTime& d)

bool DateTime::Time::operator==(const Time& t)
{
	if (hour == t.hour && minute == t.minute && second == t.second)
		return true;
	return false;
}
bool DateTime::Time::operator!=(const Time& t)
{
	if (hour != t.hour || minute != t.minute || second != t.second)
		return true;
	return false;
}

bool DateTime::Time::operator>(const Time& t)
{
	if (hour > t.hour)
		return true;
	else if(hour < t.hour) 
		return false;
	else if (hour == t.hour)
	{
		if (minute > t.minute)
			return true;
		else if (minute < t.minute)
			return false;
		else if (minute == t.minute)
		{
			if (second > t.second)
				return true;
			else if (second < t.second)
				return false;
		}
	}
	return false;
}

bool DateTime::Time::operator<(const Time& t)
{
	if (hour > t.hour)
		return false;
	else if (hour < t.hour)
		return true;
	else if (hour == t.hour)
	{
		if (minute > t.minute)
			return false;
		else if (minute < t.minute)
			return true;
		else if (minute == t.minute)
		{
			if (second > t.second)
				return false;
			else if (second < t.second)
				return true;
		}
	}
	return false;
}

bool DateTime::Time::operator>=(const Time& t)
{
	if (hour >= t.hour)
		return true;

	
	if (minute >= t.minute)
		return true;

	
	if (second >= t.second)
		return true;
	
		
	return false;
}
bool  DateTime::Time::operator<=(const Time& t)
{
	if (hour <= t.hour)
		return true;
	
	if (minute <= t.minute)
		return true;

	if (second <= t.second)
		return true;
	

	return false;
}


void DateTime::Time::IncTime(DateTime::Time& otherTime)
{
	//first convert hours to min
	int theirSeconds = otherTime.GetTimeInSeconds();
	int mySeconds = GetTimeInSeconds();

	//add em' up
	int totalSeconds = theirSeconds + mySeconds;

	SetTimeFromSeconds(totalSeconds);
}

void DateTime::Time::DecTime(DateTime::Time& otherTime)
{
	//first convert hours to min
	int theirSeconds = otherTime.GetTimeInSeconds();
	int mySeconds = GetTimeInSeconds();

	//not sure if this should be abs or not...
	int totalSeconds = theirSeconds - mySeconds;

	SetTimeFromSeconds(totalSeconds);
}

DateTime::Time DateTime::Time::TimeDiff(DateTime::Time& otherTime)
{
	//the times we have should already be in 24hr format...so we aint checking!
	Time ret;
	/*ret.minute = abs(otherTime.minute - minute);
	ret.hour = abs(otherTime.hour - hour);
	ret.second = abs(otherTime.second - second);*/
	//first convert hours to min
	int theirSeconds = otherTime.GetTimeInSeconds();
	int mySeconds = GetTimeInSeconds();
	int totalSeconds = abs(theirSeconds - mySeconds);
	ret.SetTimeFromSeconds(totalSeconds);

	return ret;
}

int DateTime::Time::GetTimeInSeconds()
{
	return (hour * 3600) + (minute * 60) + second;
}

void DateTime::Time::SetTimeFromSeconds(int totalSeconds)
{
	hour = totalSeconds / 3600;
	minute = (totalSeconds % 3600) / 60;
	second = totalSeconds % 60;
}


DateTime::DateTime()
{

};

DateTime::DateTime(int y, int m, int d)
{
	year = y;
	month = m;
	day = d;
	myTime.Init(0, 0, 0);
}
DateTime::DateTime(int y, int m, int d, int h, int min, int s)
{
	DateTime(y, m, d);
	myTime.Init(h,min,s);
}
DateTime::DateTime(std::string date)
{
	myTime.Init(0, 0, 0);
	ParseDateString(date);
}

bool DateTime::IsEmpty()
{
	if (year == 1900 && month == 1 && day == 1)
		return true;
	return false;
}

void DateTime::Clear()
{
	year = 1900;
	month = 1;
	day = 1;
	myTime.Clear();
}
void DateTime::ParseDateString(std::string dateString)
{
	std::vector<std::string> date;
	bool slashDate = false;
	//this is a date thats demited by slashes...not sure how to tell the diff between formats
	if (dateString.find("/") != std::string::npos)
	{
		date = StringUtils::Tokenize(dateString, "/");
		slashDate = true;
	}
	else if (dateString.find("\\") != std::string::npos)
	{
		date = StringUtils::Tokenize(dateString, "\\");
		slashDate = true;
	}
	else
	{
		date = StringUtils::Tokenize(dateString, " ");
	}
	if (date.size() < 3) //if(dateString == "0")
	{
		year = 1900;
		month = 1;
		day = 1;
	}
	else
	{
		if (slashDate)
		{
			year = atoi(date[2].c_str());
			month = atoi(date[0].c_str());
			day = atoi(date[1].c_str());
		}
		else
		{
			year = atoi(date[0].c_str());
			month = atoi(date[1].c_str());
			day = atoi(date[2].c_str());
		}
	}
	size_t found = dateString.find(":");
	if (found != std::string::npos)//we have a time component
	{
		//lets seperate it from the rest of the string
		std::vector<std::string> tokens = StringUtils::Tokenize(dateString, " ");
		//we can assume its the last part of the string
		myTime.SetTimeFromString(tokens.back());
	}
}
//this is the format that bluehost emails are in...maybe all emails?
void DateTime::ParseEmailDateString(std::string dateString)
{
	//Sun, 17 Dec 2017 16:44 : 27 - 0800
	//Sun, 13 Jun 2021 02:56 : 07 + 0000
	std::vector<std::string> dateTokens = StringUtils::Tokenize(dateString, " ");
	day = StringUtils::GetIntFromString(dateTokens[1]);
	SetMonthFromAbrv(dateTokens[2]);
	year = StringUtils::GetIntFromString(dateTokens[3]);
	myTime.SetTimeFromString(dateTokens[4]);
}

std::string DateTime::DigitToString(int num)
{
	if (num < 10)
		return "0" + std::to_string(num);

	return std::to_string(num);
}

std::string DateTime::ToString(bool includeTime)
{
	std::string ret = std::to_string(year) + " ";

	ret += DigitToString(month) + " ";

	ret += DigitToString(day);

	if (includeTime)
	{
		ret += " " + myTime.ToString();
	}
	return ret;
}



void DateTime::SetCurrentDateTime()
{
	time_t now = time(0);
	struct tm buf;
#ifdef _WIN32
	localtime_s(&buf, &now);
#else
	localtime_r(&now,&buf);
#endif

	


	// print various components of tm structure.
	/*cout << "Year" << 1970 + ltm->tm_year<<endl;
	cout << "Month: "<< 1 + ltm->tm_mon<< endl;
	cout << "Day: "<<  ltm->tm_mday << endl;
	cout << "Time: "<< 1 + ltm->tm_hour << ":";
	cout << 1 + ltm->tm_min << ":";
	cout << 1 + ltm->tm_sec << endl;
	 int tm_wday;        // day of the week */
	year = 1900 + buf.tm_year;
	month = 1 + buf.tm_mon;
	day = buf.tm_mday;
	myTime.hour = buf.tm_hour;
	myTime.minute = buf.tm_min;
	myTime.second = buf.tm_sec;
}


std::string DateTime::GetCurrentDayOfWeekString()
{
	time_t now = time(0);
	struct tm buf;
#ifdef _WIN32
	localtime_s(&buf, &now);
#else
	localtime_r(&now, &buf);
#endif

	switch (buf.tm_wday)
	{
	case 1:
		return "Monday";
	case 2:
		return "Tuesday";
	case 3:
		return "Wednesday";
	case 4:
		return "Thursday";
	case 5:
		return "Friday";
	case 6:
		return "Saturday";
	case 7:
		return "Sunday";
	default:
		"Unknown";
	}
}

//sunday = 0
int DateTime::GetCurrentDayOfWeek()
{
	time_t now = time(0);
	struct tm buf;
#ifdef _WIN32
	localtime_s(&buf, &now);
#else
	localtime_r(&now, &buf);
#endif

	return buf.tm_wday;
}

int DateTime::TimeDiff(DateTime& otherDate)
{
	if (year == otherDate.year && month == otherDate.month)
	{
		return abs(day - otherDate.day);
	}

	//convert dates into days
	int otherNumDays = otherDate.GetDayOfYear();
	int myNumDays = GetDayOfYear();
		
	if(year != otherDate.year)
	{
		//get the difference in days from the years
		int yearDiff = abs(otherDate.year - year) * 365; 
		otherNumDays += yearDiff;
	}

	return abs(otherNumDays - myNumDays);
}

DateTime DateTime::TimeDiffAsObj(DateTime& otherDate)
{
	int daysDiff = TimeDiff(otherDate);
	DateTime::Time timeDiff = myTime.TimeDiff(otherDate.myTime);
	timeDiff.GetTimeInSeconds();
	int totalSeconds = ConvertDaysIntoSeconds(daysDiff) + timeDiff.GetTimeInSeconds();

	DateTime ret;

	/*int day = 86400;
	int hour = 3600;
	int minute = 60;*/

	ret.year = 0;
	ret.month = 0;

	ret.day = floor(totalSeconds / 86400);
	ret.myTime.hour = floor((totalSeconds - ret.day * 86400) / 3600);
	ret.myTime.minute = floor((totalSeconds - ret.day * 86400 - ret.myTime.hour * 3600) / 60);
	ret.myTime.second = totalSeconds - ret.day * 86400 - ret.myTime.hour * 3600 - ret.myTime.minute * 60;

	return ret;
}

int DateTime::GetDayOfYear()
{
	int numDays = 0;
	for(int i = 1; i < month; i++)
		numDays += GetDaysInMonth(i);

	numDays += day;

	return numDays;
}
//convert a date string that looks like mon/day/year to my format
std::string DateTime::ConvertSlashDate(std::string slashDate)
{
	std::vector<std::string> date = StringUtils::Tokenize(slashDate, "/");
	return date[2] + " " + date[0] + " " + date[1];
}

std::string DateTime::Now()
{
	DateTime temp;
	temp.SetCurrentDateTime();

	return temp.ToString();
}

std::string DateTime::PrettyPrintNow()
{
	DateTime temp;
	temp.SetCurrentDateTime();

	return temp.PrettyPrint();
}

DateTime DateTime::operator=(const DateTime& d)
{
	this->year = d.year;
	this->month = d.month;
	this->day = d.day;
	this->myTime.hour = d.myTime.hour;
	this->myTime.minute = d.myTime.minute;
	this->myTime.second = d.myTime.second;
	return *this;
}
bool DateTime::operator==(const DateTime& d)
{
	if (myTime.IsEmpty() || d.myTime.IsEmpty())
	{
		if (this->year == d.year &&
			this->month == d.month &&
			this->day == d.day)
			return true;
	}
	if (this->year == d.year &&
		this->month == d.month &&
		this->day == d.day && 
		this->myTime == d.myTime)
		return true;

	return false;
}
bool DateTime::operator!=(const DateTime& d)
{
	if (this->myTime.IsEmpty() || d.myTime.IsEmpty())
	{
		if (this->year != d.year ||
			this->month != d.month ||
			this->day != d.day)
			return true;
	}
	if (this->year != d.year ||
		this->month != d.month ||
		this->day != d.day ||
		this->myTime != d.myTime)
		return true;

	return false;
}
bool DateTime::operator>(const DateTime& d)
{
	if (this->year > d.year)
		return true;
	else if (this->year < d.year)
		return false;
	//else the year is the same

	if (this->month > d.month)
		return true;
	else if (this->month < d.month)
		return false;
	//else the year and the month is the same

	if (this->day > d.day)
		return true;
	else if (this->day < d.day)
		return false;

	if (!this->myTime.IsEmpty() && !d.myTime.IsEmpty())
		if (myTime > d.myTime)
			return true;

	return false;
}

bool DateTime::operator<(const DateTime& d)
{
	if (this->year < d.year)
		return true;
	else if (this->year > d.year)
		return false;
	//else the year is the same

	if (this->month < d.month)
		return true;
	else if (this->month > d.month)
		return false;
	//else the year and the month is the same

	if (this->day < d.day)
		return true;
	else if (this->day > d.day)
		return false;

	if (!this->myTime.IsEmpty() && !d.myTime.IsEmpty())
		if (myTime < d.myTime)
			return true;

	return false;
}

bool DateTime::operator>=(const DateTime& d)
{
	if (*this == d)
		return true;
	else if (*this > d)
		return true;

	return false;
}

bool DateTime::operator<=(const DateTime& d)
{
	if (*this == d)
		return true;
	else if (*this < d)
		return true;
	

	return false;
}

void DateTime::IncDay(int amt)
{
	day += amt;
	int daysInCurMonth = GetDaysInMonth(month);
	while (day > daysInCurMonth)
	{
		day -= daysInCurMonth;
		IncMonth(1);
		daysInCurMonth = GetDaysInMonth(month);
	}

}

void DateTime::DecDay(int amt)
{
	day -= amt;
	int daysInCurMonth = GetDaysInMonth(month);
	while (day < 1)
	{
		DecMonth(1);
		daysInCurMonth = GetDaysInMonth(month);
		day += daysInCurMonth;	
	}
}

void DateTime::IncMonth(int amt)
{
	month += amt;
	while (month > 12)
	{
		year++;
		month -= 12;
	}
}

void DateTime::DecMonth(int amt)
{
	month -= amt;
	while (month < 1)
	{
		year--;
		month += 12;
	}
}



int DateTime::GetDaysInMonth(int mon)
{
	int days = 31;
	if(mon == 4 || mon == 6 || mon == 9 || mon == 11)
		days = 30;

	else if (mon == 2)
	{
		/*bool leapyear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

		if (leapyear == 0)
			days = 29;
		else*/
			days = 28;
	}

	return days;
}


std::string DateTime::GetMonthAbrv() 
{
	return GetMonthAbrv(month);
}
std::string DateTime::GetMonthWord()
{
	return GetMonthWord(month);
}
std::string DateTime::GetDayOfMonthWord()
{
	return GetDayOfMonthWord(day);
}
std::string DateTime::GetMonthWord(int month)
{
	switch (month)
	{
		case 1:
			return "January";
		case 2:
			return "February";
		case 3:
			return "March";
		case 4:
			return "April";
		case 5:
			return "May";
		case 6:
			return "June";
		case 7:
			return "July";
		case 8:
			return "August";
		case 9:
			return "September";
		case 10:
			return "October";
		case 11:
			return "November";
		case 12:
			return "December";
		default:
			return "";
	}
}
void DateTime::SetMonthFromWord(std::string monthWord)
{
	StringUtils::ToUpper(monthWord);
	if (monthWord == "JANUARY")
		month = 1;
	else if (monthWord == "FEBUARY")
		month = 2;
	else if (monthWord == "MARCH")
		month = 3;
	else if (monthWord == "APRIL")
		month = 4;
	else if (monthWord == "MAY")
		month = 5;
	else if (monthWord == "JUNE")
		month = 6;
	else if (monthWord == "JULY")
		month = 7;
	else if (monthWord == "AUGUST")
		month = 8;
	else if (monthWord == "SEPTEMBER")
		month = 9;
	else if (monthWord == "OCTOBER")
		month = 10;
	else if (monthWord == "NOVEMBER")
		month = 11;
	else if (monthWord == "DECEMBER")
		month = 12;
}

std::string DateTime::GetMonthAbrv(int month)
{
	switch (month)
	{
	case 1:
		return "Jan";
	case 2:
		return "Feb";
	case 3:
		return "Mar";
	case 4:
		return "Apr";
	case 5:
		return "May";
	case 6:
		return "Jun";
	case 7:
		return "Jul";
	case 8:
		return "Aug";
	case 9:
		return "Sept";
	case 10:
		return "Oct";
	case 11:
		return "Nov";
	case 12:
		return "Dec";
	default:
		return "";
	}
}
void DateTime::SetMonthFromAbrv(std::string monthAbrv)
{
	StringUtils::ToUpper(monthAbrv);
	if (monthAbrv == "JAN")
		month = 1;
	else if (monthAbrv == "FEB")
		month = 2;
	else if (monthAbrv == "MAR")
		month = 3;
	else if (monthAbrv == "APR")
		month = 4;
	else if (monthAbrv == "MAY")
		month = 5;
	else if (monthAbrv == "JUN")
		month = 6;
	else if (monthAbrv == "JUL")
		month = 7;
	else if (monthAbrv == "AUG")
		month = 8;
	else if (monthAbrv == "SEPT" || monthAbrv == "SEP")
		month = 9;
	else if (monthAbrv == "OCT")
		month = 10;
	else if (monthAbrv == "NOV")
		month = 11;
	else if (monthAbrv == "DEC")
		month = 12;
}

std::string DateTime::GetDayOfMonthWord(int day)
{
	std::string word = std::to_string(day);
	if(day == 1 || day == 21 || day == 31)
		return word +"st";
	else if(day == 2 || day == 22)
		return word +"nd";
	else if(day == 3 || day == 23)
		return word + "rd";
	else
		return word + "th";
}

std::string DateTime::PrettyPrint(bool includeTime)
{
	std::string ret =  GetMonthWord() + " " + GetDayOfMonthWord() + " " + std::to_string(year);
	if (includeTime)
	{
		if(!myTime.IsEmpty())
			ret += ", " + myTime.ToString();
	}
	return ret;
}

std::string DateTime::PrintFormattedString(std::string format)
{
	std::string ret;
	//"_%Y_%m_%d_%H_%M"
	size_t i = 0;
	while ( i < format.size() )
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] == 'Y' || format[i] == 'y')
				ret += std::to_string(year);
			else if (format[i] == 'm')
				ret += DigitToString(month);
			else if (format[i] == 'd')
				ret += DigitToString(day);
			else if (format[i] == 'H') //24 hour clock
				ret += DigitToString(myTime.hour);
			else if (format[i] == 'M') //big M is minute? okay python...
				ret += DigitToString(myTime.minute);
			else
				ret += format[i];
		}
		else
			ret += format[i];
		i++;
	}
	return ret;
}

int DateTime::ConvertDaysIntoSeconds(int days)
{
	return 86400 * days;
}

int DateTime::ConvertDateTimeIntoSeconds()
{
	int dateAsSeconds = ConvertDaysIntoSeconds(GetDayOfYear());
	int intTimeAsSeconds = myTime.GetTimeInSeconds();
	return dateAsSeconds + intTimeAsSeconds;
}