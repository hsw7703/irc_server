#pragma once

#include "Data.h"

class WhoWasCommand
{
public:
	WhoWasCommand();
	~WhoWasCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
	bool NoNicknameGiven(const std::string& msg, const SOCKET& sock);
	bool WasNoSuchNick(const std::string& nick, const SOCKET& sock);
private:
	Data& mData = Data::GetIncetence();
};