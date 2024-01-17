#pragma once

#include <fstream>
#include <vector>
#include "User.h"
#include "Email.h"

class Database
{
private:
	std::ifstream UsersFile;
	std::fstream EmailsFile;
	std::fstream StatsFile;

public:
	std::vector<User> Users;
	std::vector<Email> Emails;

	Database();
	void InitDatabase();
	void AddEmail(Email ToSend);
	void DeleteEmail(int EmailId);
	void UpdateStats();
	~Database();
};
