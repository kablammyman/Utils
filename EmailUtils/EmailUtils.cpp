
#include "EmailUtils.h"
#include <vector>
#include <string>

 
using namespace std;


EmailUtils::EmailUtils(std::string email,std::string password)
{
	isLoaded = false;
	curl = curl_easy_init();
	if (curl)
	{
		isLoaded = true;
	}
}

//--------------------------------------------------------------------------------------------------
EmailUtils::~EmailUtils()
{ 
	curl_easy_cleanup(curl);
}
//--------------------------------------------------------------------------------------------------
 
void EmailUtils::SendEmail(string sender, string mailServer, int port, string reciever, string senderPW, string subject, string body)
{
	/*smtpserver = smtplib.SMTP(mailServer, port)
	smtpserver.ehlo()
	smtpserver.starttls()
	smtpserver.ehlo
	smtpserver.login(sender, senderPW)
	bcc = [MY_EMAIL_ADDY]
	recieverList = [reciever] + bcc

	header = 'To:' + reciever + '\n' + 'From: ' + sender + '\n' + 'Subject:' + subject + '\n'
	#print header
	msg = header + body
	smtpserver.sendmail(sender, recieverList, msg)
	#print 'done!'
	smtpserver.close()*/
}

void EmailUtils::SendEmailWithAttachment(string sender, string mailServer, int port, string reciever, string senderPW, string subject, string body, vector<string> &fileList)
{
	/*msg = MIMEMultipart()
		msg['Subject'] = subject
		msg['From'] = sender
		bcc = [MY_EMAIL_ADDY]

		recieverList = [reciever] + bcc

		msg['To'] = reciever
		#msg['To'] = ', '.join(targets)
		#targets = ['HE@EXAMPLE.COM', 'SHE@EXAMPLE.COM']

		txt = MIMEText(body)
		msg.attach(txt)
		allFiles = []

		attachment = None

		if type(fileList) is str :
	allFiles.append(fileList)
		else:
	allFiles = fileList


		#send multiple attachments
		for file in allFiles :
	print file
		with open(file, 'rb') as f :
	if file.lower().endswith(('.png', '.jpg', '.jpeg')) :
		attachment = MIMEImage(f.read())

		elif file.lower().endswith('.pdf') :
		attachment = MIMEApplication(f.read())

	else:
	attachment = MIMEText(f.read())

		attachment.add_header('Content-Disposition', 'attachment', filename = os.path.basename(file))

		msg.attach(attachment)

		server = smtplib.SMTP(mailServer, port)
		server.ehlo()
		server.starttls()
		server.ehlo
		server.login(sender, senderPW)
		server.sendmail(sender, recieverList, msg.as_string())
		server.quit()*/
}

void EmailUtils::OpenMailbox(string mailDomain, string emailAddy, string emailpw)
{
	/*mail = imaplib.IMAP4_SSL(mailDomain)#"mail.victorsvacantland.com"
		mail.login(emailAddy, emailpw)#MY_EMAIL_ADDY, EMAIL_PW

		'''
		status, folders = mail.list()
		print len(folders)
		for i in folders :
	print i
		'''
		#search by search index(can point to wrong email if things are added or removed)
		# Out: list of "folders" aka labels in gmail.
		mail.select("inbox") # connect to inbox.
		return mail*/
}

void EmailUtils::GetLatestEmail(int numLatest)
{
	/*mail = OpenMailbox("mail.victorsvacantland.com",MY_EMAIL_ADDY, EMAIL_PW)
	result, data = mail.search(None, "ALL")

	ids = data[0] # data is a list.

	id_list = ids.split() # ids is a space separated string
	numLatest+=1
	for i in range(1, numLatest):
		latest_email_id = id_list[i*-1] # get the latest

		# fetch the email body (RFC822) for the given ID
		result, data = mail.fetch(latest_email_id, "(RFC822)")

		email_message = email.message_from_string(data[0][1])
		#print email_message

		result, whatsThis = mail.uid("SEARCH", latest_email_id, "ALL")
		t = whatsThis[0].split()
		print " message id " + str(latest_email_id)+ "'s uid " + str(t)
		print email_message['To']
		print email_message['Subject']
		print email.utils.parseaddr(email_message['From'])
		#print email_message.items() # print all headers


		raw_email = data[0][1] # here's the body, which is raw text of the whole email

	raw_email = ""
	return raw_email*/
}

void EmailUtils::PutSentEmailToSent()
{
	/*mail = OpenMailbox("mail.victorsvacantland.com",MY_EMAIL_ADDY, EMAIL_PW)
		
	result, data = mail.search(None, "ALL")
	ids = data[0] # data is a list.
	id_list = ids.split() # ids is a space separated string

	for i in range(1, 2):
		latest_email_id = id_list[i*-1] # get the latest

		# fetch the email body (RFC822) for the given ID
		result, data = mail.fetch(latest_email_id, "(RFC822)") 
		
		email_message = email.message_from_string(data[0][1])
				
		if email.utils.parseaddr(email_message['From'])[1] == MY_EMAIL_ADDY:
			#print "found my email"
			mail.copy(latest_email_id, "INBOX.Sent")
			# What are the flags now?
			#typ, response = mail.fetch(latest_email_id, '(FLAGS)')
			#print 'Flags before:', response
			
			# Change the Deleted flag
			typ, response = mail.store(latest_email_id, '+FLAGS', r'(\Deleted)')
			
			# What are the flags now?
			typ, response = mail.fetch(latest_email_id, '(FLAGS)')
			#print 'Flags after:', response
			
			# Really delete the message.
			typ, response = mail.expunge()
			#print 'Expunged:', response
		
			
	### On the IMAP connection, we need to move the mail in the "SENT" box
    # you may need to be smarter there, since this name may change*/

}

void EmailUtils::GetMailByUID(int uid)
{
	/*mail = OpenMailbox("mail.victorsvacantland.com", MY_EMAIL_ADDY, EMAIL_PW)
		# fetch the email body(RFC822) for the given ID
		result, data = mail.uid("FETCH", uid, "(RFC822)")

		raw_email = data[0][1] # here's the body, which is raw text of the whole email

		return raw_email */
}

void EmailUtils::GetAllUnOpenedMail()
{
	/*mail = OpenMailbox("mail.victorsvacantland.com",MY_EMAIL_ADDY, EMAIL_PW)

	# Use search(), not status()
	status, response = mail.search(None, '(UNSEEN)')
	unread_msg_nums = response[0].split()

	# Print the count of all unread messages
	#print len(unread_msg_nums) # some sort of reg id...based on num of emails in inbox?
	#print unread_msg_nums
	unopendRawEmail = []
	for e_id in unread_msg_nums:
		result, whatsThis = mail.uid("SEARCH", e_id, "ALL")
		t = whatsThis[0].split()
		print " message id " + str(e_id)+ "'s uid " + str(t)
		
		result, response = mail.fetch(e_id, '(UID BODY[TEXT])')
		unopendRawEmail.append(response[0][1])
		
		#email_message = email.message_from_string(response[0][1])
		#print email_message
		print response
		'''
		print email_message['To']
		print email_message['Subject']
		print email.utils.parseaddr(email_message['From'])
		print email_message.items() # print all headers
		'''
	return unopendRawEmail

    # Mark them as seen
    #for e_id in unread_msg_nums:
    #    imap.store(e_id, '+FLAGS', '\Seen')*/
}

void EmailUtils::GetAllMail()
{
	/*	retList = []
		mail = OpenMailbox("mail.victorsvacantland.com",MY_EMAIL_ADDY, EMAIL_PW)

		result, data = mail.search(None, 'ALL')

		ids = data[0] # data is a list.
		id_list = ids.split() # ids is a space separated string

		for id in id_list:
			try:
				latest_email_id = id
			except:
				continue

			# fetch the email body (RFC822) for the given ID
			result, data = mail.fetch(latest_email_id, "(RFC822)")

			retList.append(data[0][1]) # here's the body, which is raw text of the whole email
			# including headers and alternate payloads
		return retList
	   # Mark them as seen
		#for e_id in unread_msg_nums:
		#    imap.store(e_id, '+FLAGS', '\Seen')*/
}

void EmailUtils::GetLatestMailUID()
{
	/*	mail = OpenMailbox("mail.victorsvacantland.com",MY_EMAIL_ADDY, EMAIL_PW)
		result, uids = mail.uid("SEARCH", None, "ALL")
		uids = uids[0].split()
		latest_email_uid = uids[-1]# get the lates
		print "latest uid" + str(latest_email_uid)

		# fetch the email body (RFC822) for the given ID
		result, data = mail.uid("FETCH", latest_email_uid, "(RFC822)")

		raw_email = data[0][1] # here's the body, which is raw text of the whole email
		return raw_email*/
}

void EmailUtils::GetAllUID()
{
	/*mail = OpenMailbox("mail.victorsvacantland.com",MY_EMAIL_ADDY, EMAIL_PW)
	result, uids = mail.uid("SEARCH", None, "ALL")
	uids = uids[0].split()
	return uids*/
}

void EmailUtils::SearchForEmail(string title,string body)
{
	/*mail = OpenMailbox("mail.victorsvacantland.com",MY_EMAIL_ADDY, EMAIL_PW)

	searchString = "(SUBJECT \""+title+"\")" 
	print searchString
	result, data = mail.search(None, searchString)

		
	ids = data[0] # data is a list.
	id_list = ids.split() # ids is a space separated string
	try:
		latest_email_id = id_list[-1] # get the latest
	except:
		return None
		
	# fetch the email body (RFC822) for the given ID
	result, data = mail.fetch(latest_email_id, "(RFC822)") 

	raw_email = data[0][1] # here's the body, which is raw text of the whole email
	# including headers and alternate payloads
	return raw_email*/
}

void EmailUtils::SearchForEmailViaSender(string sender, string body)
{
	/*	mail = OpenMailbox("mail.victorsvacantland.com",MY_EMAIL_ADDY, EMAIL_PW)

		searchString = "(FROM \""+sender+"\")"
		result, data = mail.search(None, searchString)

		ids = data[0] # data is a list.
		id_list = ids.split() # ids is a space separated string
		try:
			latest_email_id = id_list[-1] # get the latest
		except:
			return None

		# fetch the email body (RFC822) for the given ID
		result, data = mail.fetch(latest_email_id, "(RFC822)")

		raw_email = data[0][1] # here's the body, which is raw text of the whole email
		# including headers and alternate payloads
		return raw_email*/
}


