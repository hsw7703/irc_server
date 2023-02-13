#pragma once

#include <map>
#include <mutex>
#include <vector>

#include "User.h"
#include "UserHistory.h"
#include "Channel.h"
#include "UserHistory.h"

class Data
{
public:
	static Data& GetInstance();
	std::map<SOCKET, User>& GetUser();
	std::map<std::string, Channel>& GetChannel();
	const std::string& GetDate() const;
	const std::string& GetServName() const;
	const std::string& GetPassword() const;
	const std::string& GetVersion() const;
	std::mutex& GetMutex();
	void AddUserHistory(const User& user);
	std::vector<UserHistory> FindUserHistory(std::string nick);

	User* FindUser(const std::string& nick);
private:
	Data();
	~Data();
	Data(const Data& data);
	Data& operator=(const Data& data);
private:
	std::map<SOCKET, User> mUser;
	std::map<std::string, Channel> mChannel;
	std::vector<UserHistory> mUserHistory;
	std::mutex mMutex;

	std::string mDate;
	std::string mServName = SERVER_NAME;
	std::string mPassword = PASSWORD;
	std::string mVersion = VERSION;
};