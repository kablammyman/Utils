#pragma once

#include <string>
#include <vector>

#include <curl.h>


class CurlUtils
{
	struct upload_status 
	{
		int lines_read;
	};
	
	static std::vector<std::string> curEmail;
	static std::vector<std::string> CreatePayloadText(std::string emailAddress, std::string subject, std::string message);
	static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
	static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp);
	
	public:
	static std::string lastError;
	static std::string readBuffer;
	static int PostToClick2Mail(std::string url, std::vector<std::string> &headerOptions,  std::string xmlData);
	static int GetFromClick2Mail(std::string url, std::string data);
	

	static int SendEmail(std::string emailAddress, std::string subject, std::string message);
	static int DumpToFile(std::string filename, std::string data);
	static size_t ftp_read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
	static int UploadToFTP(std::string ftpUrl, std::string localFile,std::string fileNameWithoutPath);
};