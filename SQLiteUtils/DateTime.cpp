#include "DateTime.h"

DateTime::DateTime()
{
	year = 1900;
	month = 1;
	day = 1;
};
DateTime::DateTime(std::string date)
{
	ParseDateString(date);
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
}
std::string DateTime::ToString()
{
	std::string ret = std::to_string(year) + " ";
	if (month < 10)
		ret += "0";
	ret += std::to_string(month) + " ";
	if (day < 10)
		ret += "0";
	ret += std::to_string(day);

	return ret;
}
void DateTime::SetCurrentDateTime()
{
	time_t now = time(0);
	struct tm buf;
	localtime_s(&buf, &now);


	// print various components of tm structure.
	/*cout << "Year" << 1970 + ltm->tm_year<<endl;
	cout << "Month: "<< 1 + ltm->tm_mon<< endl;
	cout << "Day: "<<  ltm->tm_mday << endl;
	cout << "Time: "<< 1 + ltm->tm_hour << ":";
	cout << 1 + ltm->tm_min << ":";
	cout << 1 + ltm->tm_sec << endl;*/
	year = 1900 + buf.tm_year;
	month = 1 + buf.tm_mon;
	day = buf.tm_mday;
}

int DateTime::TimeDiff(DateTime& otherDate)
{
	if (year == otherDate.year && month == otherDate.month)
	{
		return abs(day - otherDate.day);
	}
	else
	{
		int yr = abs(year - otherDate.year);
		int mon = abs(month - otherDate.month);
		int days = abs(day - otherDate.day);

		//this is innaccurate, since this doesnt account for leap years, or how many days are in each month
		//for now, each month has 30 days, and eyach year has 365 days
		return (yr * 365) + (mon * 30) + days;
	}
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

DateTime DateTime::operator=(const DateTime& d)
{
	this->year = d.year;
	this->month = d.month;
	this->day = d.day;
	return *this;
}
bool DateTime::operator==(const DateTime& d)
{
	if (this->year == d.year &&
		this->month == d.month &&
		this->day == d.day)
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
	else if (this->month > d.month)
		return false;
	//else the year and the month is the same

	if (this->day > d.day)
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