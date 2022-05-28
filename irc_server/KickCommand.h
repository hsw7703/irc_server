#pragma once

#include "Data.h"

class KickCommand
{
public:
	KickCommand();
	~KickCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
	bool Error(const std::string& ch, const std::string& nick, const SOCKET& sock);
	bool NoSuchChannel(const std::string& ch, const SOCKET& sock);
	bool NotChannelOperator(const std::string& ch, const SOCKET& sock);
	bool UserNotInChannel(const std::string& ch, const std::string& nick, const SOCKET& sock);
	bool NotOnChannel(const std::string& ch, const SOCKET& sock);
private:
	Data& mData = Data::GetIncetence();
};