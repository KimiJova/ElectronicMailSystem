#pragma once

#include <string>
#include "Database.h"

class Commands
{
private:
	std::string CurrentUser;
	int			CurrentUserId;

	void Login(Database& Db, std::string Username, std::string Password);
	void Logout(Database& Db);
	void SendEmail(Database& Db, std::string Who, std::string Where, std::string What);
	void Check(Database& Db);
	void Stats(Database& Db);
	void Delete(Database& Db, int EmailId);
	void Clean(Database& Db);
	void Receive(Database& Db, int EmailId);
public:
	int	CommandSize;
	int	ServerId;

	Commands();
	void ParseCommand(Database& Db, std::string Command);
	~Commands();
};
