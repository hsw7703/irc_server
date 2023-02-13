#pragma once

#include "Data.h"

class InviteCommand
{
public:
	InviteCommand();
	~InviteCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
	bool Error(const std::string& nick, const std::string& ch, const SOCKET& sock);
	bool NoSuchNick(const std::string& nick, const SOCKET& sock);
	bool NotOnChannel(const std::string& ch, const SOCKET& sock);
	bool UserOnChannel(const std::string& nick, const std::string& ch, const SOCKET& sock);
	bool NotChannelOperator(const std::string& ch, const SOCKET& sock);
private:
	Data& mData = Data::GetInstance();
};