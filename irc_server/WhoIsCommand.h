#pragma once

#include "Data.h"

class WhoIsCommand
{
public:
	WhoIsCommand();
	~WhoIsCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
	bool Error(const std::string& target, const SOCKET& sock);
	bool NoNicknameGiven(const std::string& target, const SOCKET& sock);
	bool NoSuchNick(const std::string& target, const SOCKET& sock);
private:
	Data& mData = Data::GetInstance();
};