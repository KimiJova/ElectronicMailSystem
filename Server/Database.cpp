#include <cstdio>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include "Database.h"

Database::Database()
{
	//
}

void Database::InitDatabase()
{
	// Open all the files.
	UsersFile.open("../users.db", std::ios::in);
	EmailsFile.open("../emails.db", std::ios::in);
	StatsFile.open("../stats.db", std::ios::in);

	// Load all the data from the files into usable variables.
	User TemporaryUser;
	while (UsersFile >> TemporaryUser.Username)
	{
		UsersFile >> TemporaryUser.Password;

		Users.push_back(TemporaryUser);
	}

	Email TemporaryEmail;
	while (EmailsFile >> TemporaryEmail.Sender)
	{
		EmailsFile >> TemporaryEmail.Recipient;
		std::getline(EmailsFile, TemporaryEmail.Contents);

		// Remove whitespace from beginning.
		auto Begin = TemporaryEmail.Contents.find_first_not_of(" ");
		auto End = TemporaryEmail.Contents.find_last_not_of(" ");
		TemporaryEmail.Contents = TemporaryEmail.Contents.substr(Begin, End - Begin + 1);

		Emails.push_back(TemporaryEmail);
	}

	std::vector<User>::iterator it;
	for (it = Users.begin(); it != Users.end(); ++it)
	{
		std::string Username;

		StatsFile >> Username;
		StatsFile >> it->Stats.SentEmails;
		StatsFile >> it->Stats.InboxEmails;
		StatsFile >> it->Stats.DeletedEmails;
	}
}

void Database::AddEmail(Email ToSend)
{
	Emails.push_back(ToSend);
	EmailsFile.close();
	EmailsFile.open("../emails.db", std::ios::out | std::ios::app);
	EmailsFile.seekg(0, std::ios_base::end);
	EmailsFile << ToSend.Sender << " " << ToSend.Recipient << " " << ToSend.Contents << std::endl;
}

void Database::DeleteEmail(int EmailId)
{
	// Find the line in the database file to be deleted and remove it. Write back to database file.
	std::string FullLine = Emails[EmailId].Sender + " " + Emails[EmailId].Recipient + " " + Emails[EmailId].Contents;
	EmailsFile.close();
	EmailsFile.open("../emails.db", std::ios::in);
	std::vector<std::string> Lines;
	std::string Line;
	while (!EmailsFile.eof())
	{	
		getline(EmailsFile, Line);
		if (Line == FullLine) continue;
		Lines.push_back(Line);
	}
	EmailsFile.close();
	EmailsFile.open("../emails.db", std::ios::out | std::ios::trunc);
	for (auto x : Lines)
	{
		if (x.find_first_not_of(' ') == std::string::npos) continue;
		EmailsFile << x << std::endl;
	}

	// Remove the email from the variable.
	auto it = Emails.begin();
	std::advance(it, EmailId);
	Emails.erase(it);
}

void Database::UpdateStats()
{
	// Rewrite stats file from variables.
	StatsFile.close();
	StatsFile.open("../stats.db", std::ios::out | std::ios::trunc);

	for (auto it : Users)
	{
		StatsFile << it.Username << " " << it.Stats.SentEmails << " " << it.Stats.InboxEmails << " " << it.Stats.DeletedEmails << std::endl;
	}
}

Database::~Database()
{
	UsersFile.close();
	EmailsFile.close();
	StatsFile.close();
}