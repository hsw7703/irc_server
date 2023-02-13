#pragma once

#include "Data.h"

class UserCommand
{
public:
	UserCommand();
	~UserCommand();
	void Run(const std::string& msg, const SOCKET& sock);
public:
private:
	std::vector<std::string> Parser(const std::string& msg);
	bool Error(const std::vector<std::string>& msg, const SOCKET& sock);
	bool AlreadyRegister(const SOCKET& sock);
	bool NeedMoreParams(const std::vector<std::string>& msg, const SOCKET& sock);
private:
	Data& mData = Data::GetInstance();
};