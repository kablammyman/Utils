#include "CurlUtils.h"

#include "StringUtils.h"
#include <fstream>
//for teh sleep method
#include <chrono>
#include <thread>

#ifndef _WIN32
#include <cstring> //for strlen
#include <sys/stat.h> //struct stat
#endif

using namespace std;
vector<string> CurlUtils::curEmail;
string CurlUtils::readBuffer = "";
string CurlUtils::lastError = "";


size_t CurlUtils::WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
int CurlUtils::HttpsPost(string url,vector<string> &headerOptions,  string postFields)
{
	CURL *curl;
	CURLcode res = CURLE_OK;
	readBuffer.clear();
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
		struct curl_slist *headers = NULL;
		for(size_t i = 0; i < headerOptions.size(); i++)
			headers = curl_slist_append(headers, headerOptions[i].c_str());

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);

	return res;
}

int CurlUtils::HttpsGet(string url, vector<string>& headerOptions, string data)
{
	CURL *curl;
	CURLcode res = CURLE_OK;
	readBuffer.clear();
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		struct curl_slist *headers = NULL;
		for (size_t i = 0; i < headerOptions.size(); i++)
			headers = curl_slist_append(headers, headerOptions[i].c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		res = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);

	return res;
}




int CurlUtils::DumpToFile(string filename, string data)
{
	ofstream curList;
	curList.open(filename,fstream::app);
	if(curList.is_open())
	{
		curList << data <<endl;
		curList.close();
		return 0;
	}
	return -1;
}


/// <summary>
/// //////////////////////////////ftp stuff/////////////////////////////////////////////////

/* NOTE: if you want this example to work on Windows with libcurl as a
DLL, you MUST also provide a read callback with CURLOPT_READFUNCTION.
Failing to do so will give you a crash since a DLL may not use the
variable's memory when passed in to it from an app like this. */
size_t CurlUtils::ftp_read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	curl_off_t nread;
	/* in real-world cases, this would probably get this data differently
	as this fread() stuff is exactly what the library already would do
	by default internally */
	size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);

	nread = (curl_off_t)retcode;

	fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
		" bytes from file\n", nread);
	return retcode;
}


int CurlUtils::UploadToFTP(string ftpUrl, string username, string password, string localFile,string fileNameWithoutPath)
{
	CURL *curl;
	CURLcode res;
	FILE *hd_src;
	struct stat file_info;
	curl_off_t fsize;

	string whileUpload = "while-uploading.txt";
	ftpUrl += whileUpload;

	struct curl_slist *headerlist = NULL;
	string buf_1 = "RNFR " + whileUpload;
	string buf_2 = "RNTO " + fileNameWithoutPath;

	/* get the file size of the local file */
	if(stat(localFile.c_str(), &file_info))
	{
		//cout << "Couldn't open " << localFile << endl;
		return -1;
	}
	fsize = (curl_off_t)file_info.st_size;

	printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

	/* get a FILE * of the same file */
#if _WIN32
	fopen_s(&hd_src,localFile.c_str(), "rb");
#else
    hd_src = fopen(localFile.c_str(), "rb");
#endif
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if(curl) {
		/* build a list of commands to pass to libcurl */
		headerlist = curl_slist_append(headerlist, buf_1.c_str());
		headerlist = curl_slist_append(headerlist, buf_2.c_str());

		/* we want to use our own read function */
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ftp_read_callback);

		/* enable uploading */
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		/* specify target */
		curl_easy_setopt(curl, CURLOPT_URL, ftpUrl.c_str());

		//create the dirs iof they dont exist
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS,CURLFTP_CREATE_DIR_RETRY);

		//provide the usernaem and password for the ftp server
		string userPass = username + ":" + password;
		curl_easy_setopt( curl, CURLOPT_USERPWD, userPass.c_str());

		/* pass in that last of FTP commands to run after the transfer */
		curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

		/* now specify which file to upload */
		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

		/* Set the size of the file to upload (optional).  If you give a *_LARGE
		option you MUST make sure that the type of the passed-in argument is a
		curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
		make sure that to pass in a type 'long' argument. */
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)fsize);

		/* Now run off and do what you've been told! */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}
		/* clean up the FTP commands list */
		curl_slist_free_all(headerlist);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	fclose(hd_src); /* close the local file */

	curl_global_cleanup();
	return (int)res;
}

//////////////////////////send email code/////////////////////////////////////////////////////
//got this from stactoverflow
const std::string CurrentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y %m %d.%X", &tstruct);

	return buf;
}

vector<string> CurlUtils::CreatePayloadText(string emailAddress, string fromEmail,string domain, string subject, string message)
{
	vector<string> email;
	email.push_back("Date: " + CurrentDateTime() + "\r\n");
	email.push_back("To: " +emailAddress +"\r\n");
	email.push_back("From: "+fromEmail+"\r\n");
	string messageID = "Message-ID: <";
	messageID += StringUtils::GetRandomAlphaNumericString(8);
	messageID += "-" + StringUtils::GetRandomAlphaNumericString(4);
	messageID += "-" + StringUtils::GetRandomAlphaNumericString(4);
	messageID += "-" + StringUtils::GetRandomAlphaNumericString(4);
	messageID += "-" + StringUtils::GetRandomAlphaNumericString(12);
	messageID += "@" + domain+"\r\n";

	//email.push_back("Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@victorsvacantland.com\r\n");
	email.push_back(messageID);
	email.push_back("Subject: "+ subject+"\r\n");
	email.push_back("\r\n"); // empty line to divide headers from body, see RFC5322
	email.push_back(message+"\r\n");
	email.push_back("\0");

	return email;
}



size_t CurlUtils::payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct upload_status *upload_ctx = (struct upload_status *)userp;
	const char *data;

	if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}

	/*data = payload_text[upload_ctx->lines_read];

	if(data) {
	size_t len = strlen(data);
	memcpy(ptr, data, len);
	upload_ctx->lines_read++;

	return len;
	}*/
	data = curEmail[upload_ctx->lines_read].c_str();

	if(data)
	{
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;

		return len;
	}
	return 0;
}

int CurlUtils::SendEmail(string toEmailAddress, string fromEmailAddress, string password, string sendingDomain,string smtpUrlAndPort, string subject, string message)
{
	CURL *curl;
	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	struct upload_status upload_ctx;

	upload_ctx.lines_read = 0;

	curl = curl_easy_init();
	if(curl)
	{
		curEmail.clear();
		curEmail = CreatePayloadText(toEmailAddress, fromEmailAddress, sendingDomain, subject, message);

		/* This is the URL for your mailserver */

		//curl_easy_setopt(curl, CURLOPT_USERNAME, "victor@victorsvacantland.com");
		curl_easy_setopt(curl, CURLOPT_USERNAME, fromEmailAddress.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, smtpUrlAndPort.c_str());
		curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

		/* Note that this option isn't strictly required, omitting it will result
		* in libcurl sending the MAIL FROM command with empty sender data. All
		* autoresponses should have an empty reverse-path, and should be directed
		* to the address in the reverse-path which triggered them. Otherwise,
		* they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
		* details.
		*/
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromEmailAddress.c_str());

		/* Add two recipients, in this particular case they correspond to the
		* To: and Cc: addressees in the header, but they could be any kind of
		* recipient. */
		recipients = curl_slist_append(recipients, toEmailAddress.c_str());
		//recipients = curl_slist_append(recipients, CC_ADDR);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

		/* We're using a callback function to specify the payload (the headers and
		* body of the message). You could just use the CURLOPT_READDATA option to
		* specify a FILE pointer to read from. */
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		/* Since the traffic will be encrypted, it is very useful to turn on debug
		* information within libcurl to see what is happening during the transfer.
		*/

		//when debugging, turn this back on
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		/* Send the message */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}
		/* Free the list of recipients */
		curl_slist_free_all(recipients);

		/* curl won't send the QUIT command until you call cleanup, so you should
		* be able to re-use this connection for additional messages (setting
		* CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and calling
		* curl_easy_perform() again. It may not be a good idea to keep the
		* connection open for a very long time though (more than a few minutes
		* may result in the server timing out the connection), and you do want to
		* clean up in the end.
		*/
		curl_easy_cleanup(curl);
	}

	return (int)res;
}
bool CurlUtils::SendEmail(string password, string sendingDomain, string smtpUrlAndPort, CurlUtils::EmailStruct email)
{
	int ret = SendEmail(email.to, email.from, password, sendingDomain, smtpUrlAndPort, email.subject, email.message);
	if (ret == CURLE_OK)
		return true;
	return false;
}
CurlUtils::EmailStruct CurlUtils::ReadEmail(string username, string password,string url, string imapArgs)
{

	CURL* curl;
	CURLcode res = CURLE_OK;
	readBuffer.clear();
	curl = curl_easy_init();
	CurlUtils::EmailStruct email;
	if (curl)
	{
		/* Set username and password */
		curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

		/* This will fetch message 1 from the user's inbox */
		//curl_easy_setopt(curl, CURLOPT_URL,"imap://imap.example.com/INBOX/;UID=1");
		//string completeUrl = "imap://" + url + ":" + port + "/INBOX/;UID=1";


		string completeUrl = "imap://" + url + "/" + imapArgs;
		curl_easy_setopt(curl, CURLOPT_URL, completeUrl.c_str());

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);//THIS LINE IS VERY IMPORTANT!

		/* Perform the fetch */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}
		else
		{
			//first get who sent this:
			email.from = StringUtils::GetDataBetweenSubStrings(readBuffer, "\nFrom:", "\r\n");
			StringUtils::TrimWhiteSpace(email.from);

			//see who this email is meant for
			email.to = StringUtils::GetDataBetweenSubStrings(readBuffer, "\nTo:", "\r\n");
			StringUtils::TrimWhiteSpace(email.to);

			//get the subject
			email.subject = StringUtils::GetDataBetweenSubStrings(readBuffer, "\nSubject:", "\r\n");
			StringUtils::TrimWhiteSpace(email.subject);

			email.dateRecv = StringUtils::GetDataBetweenSubStrings(readBuffer, "\nDate:", "\r\n");
			StringUtils::TrimWhiteSpace(email.dateRecv);

			//get the text of the email, not the attachments.
			//later on, Ill add a way to get all Conent-Types within an email and do shit with them
			size_t messageStart = readBuffer.find("Content-Type: text/plain;");
			if (messageStart == string::npos)
				messageStart = readBuffer.find("Content-Type: text/html;");

			messageStart += 25;//25 = strlen of "Content-Type: text/plain;"
			size_t messageEnd = readBuffer.find("Content-Type:", messageStart);

			if (messageEnd != string::npos)
				email.message = readBuffer.substr(messageStart, messageEnd- messageStart);
			else
				email.message = readBuffer.substr(messageStart);
		}
		/* Always cleanup */
		curl_easy_cleanup(curl);
	}

	return email;

}

void CurlUtils::EmailStruct::GetNameAndEmailFromSender(string& name, string& emailAddy)
{
	//Chris Stevens <chrisss3133@gmail.com>

	//no <> means no name and email combined
	size_t found = from.find("<");
	if (found == string::npos)
		return;

	name = from.substr(0, found);
	for(size_t i = 0; i < name.size(); i++)
		if(name[i] == '"')
			name[i] = ' ';
	emailAddy = StringUtils::GetDataBetweenChars(from, '<', '>', found);
}
bool CurlUtils::MoveEmailToFolder(std::string username, std::string password,std::string emailUrl,  std::string uid, std::string newFolderName)
{

	CURL* curl;
	CURLcode res = CURLE_OK;
	readBuffer.clear();
	curl = curl_easy_init();
	bool ret = true;

	if (!curl)
		return false;

	curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
	curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
	curl_easy_setopt(curl, CURLOPT_URL, emailUrl.c_str());
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	// Set the COPY command specifying the message ID and destination folder
	//curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "COPY 1 FOLDER");
	string copyCommand = "UID COPY " + uid +" "+ newFolderName;
	string deleteCommand = "UID STORE " + uid + " Flags \\Deleted";

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, copyCommand.c_str());

	res = curl_easy_perform(curl);
	// Check for errors
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		ret = false;
		goto cleanup;
	}


	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, deleteCommand.c_str());
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		ret = false;
		goto cleanup;
	}

	// Set the EXPUNGE command, although you can use the CLOSE command if you don't want to know the result of the STORE

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXPUNGE");
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		ret = false;
		goto cleanup;
	}

	cleanup:
	// Always cleanup
	curl_easy_cleanup(curl);

	return ret;
}
bool CurlUtils::DeleteEmail(std::string username, std::string password, std::string emailUrl, std::string uid)
{

	CURL* curl;
	CURLcode res = CURLE_OK;
	readBuffer.clear();
	curl = curl_easy_init();
	bool ret = true;

	if (!curl)
		return false;

	curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
	curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
	curl_easy_setopt(curl, CURLOPT_URL, emailUrl.c_str());
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	string deleteCommand = "UID STORE " + uid + " Flags \\Deleted";

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, deleteCommand.c_str());
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		ret = false;
		goto cleanup;
	}

	// Set the EXPUNGE command, although you can use the CLOSE command if you don't want to know the result of the STORE

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXPUNGE");
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		ret = false;
		goto cleanup;
	}

cleanup:
	// Always cleanup
	curl_easy_cleanup(curl);

	return ret;
}
string CurlUtils::FetchWebpage(string url)
{

	CURL* curl;
	CURLcode res = CURLE_OK;
	readBuffer.clear();
	curl = curl_easy_init();

	if (!curl)
		return "";


	/* specify URL to get */
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	/* send all data to this function  */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);


	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

	/* some servers don't like requests that are made without a user-agent
	field, so we provide one */
	//curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.135 Safari/537.36");

	// get it!
	res = curl_easy_perform(curl);

	// check for errors
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return "";
	}

	// Always cleanup
	curl_easy_cleanup(curl);

	return readBuffer;
}
