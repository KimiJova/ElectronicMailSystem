#pragma once

class Stat
{
public:
	int SentEmails;
	int InboxEmails;
	int DeletedEmails;

	Stat()
	{
		SentEmails = 0;
		InboxEmails = 0;
		DeletedEmails = 0;
	}
};