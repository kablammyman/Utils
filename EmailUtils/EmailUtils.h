#ifndef EmailUtils_H
#define EmailUtils_H

#include <string>
#include <vector>
//#include <utility>      // std::pair
#include "curl/curl.h"

class EmailUtils
{
	CURL *curl;
	CURLcode res;

	std::string email;
	std::string password;
	bool isLoaded;
	
public:
	typedef std::pair <std::string, std::string> dbDataPair;

	EmailUtils(std::string email,std::string password);
	~EmailUtils();
	void SendEmail(std::string sender, std::string mailServer, int port, std::string reciever, std::string senderPW, std::string subject, std::string body);
	void SendEmailWithAttachment(std::string sender, std::string mailServer, int port, std::string reciever, std::string senderPW, std::string subject, std::string body, std::vector<std::string> &fileList);
	void OpenMailbox(std::string mailDomain, std::string emailAddy, std::string emailpw);
	void GetLatestEmail(int numLatest = 1);
	void PutSentEmailToSent();
	void GetMailByUID(int uid);
	void GetAllUnOpenedMail();
	void GetAllMail();
	void GetLatestMailUID();
	void GetAllUID();
	void SearchForEmail(std::string title,std::string body);
	void SearchForEmailViaSender(std::string sender, std::string body);
};
#endif //#define EmailUtils_H
