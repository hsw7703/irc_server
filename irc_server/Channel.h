#pragma once

#include <time.h>

#include <string>
#include <vector>

#include "User.h"

class Channel
{
public:
	Channel();
	~Channel();
	void SetAdmin(std::string admin);
	std::string GetAdmin();
	std::vector<User*> GetUser();
	void SetTopic(std::string topic);
	std::string GetTopic();
	
	void AddUser(User* user);
	void EraseUser(const User* user);
public:
private:
private:
	std::string mAdmin;
	std::vector<User*> mUser;
	std::string mTopic = "";
	std::string mDate = "";
};