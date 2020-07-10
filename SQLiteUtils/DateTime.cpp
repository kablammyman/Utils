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
	else if (this->month < d.month)
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

std::string DateTime::PrettyPrint()
{
	return GetMonthWord() + " " + GetDayOfMonthWord() + " " + std::to_string(year);
}