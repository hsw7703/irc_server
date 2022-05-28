#pragma once

#include "Utils.h"

class UserHistory
{
public:
	UserHistory();
	UserHistory(const std::string& name, const std::string& realname, const std::string& host, const std::string& nick);
	~UserHistory();
	const std::string& GetName() const;
	const std::string& GetRealname() const;
	const std::string& GetHost() const;
	const std::string& GetNick() const;
	const std::string& GetDate() const;
private:
private:
	std::string mName;
	std::string mRealname;
	std::string mHost;
	std::string mNick;
	std::string mDate;
};