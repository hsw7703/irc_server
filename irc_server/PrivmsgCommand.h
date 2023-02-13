#pragma once

#include "Data.h"

class PrivmsgCommand
{
public:
	PrivmsgCommand();
	~PrivmsgCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
	bool Error(const std::string& command, const std::string& msgTarget, const std::string& privmsg, const SOCKET& sock);
	bool NoRecioient(const std::string& command, const std::string& msgTarget, const SOCKET& sock);
	bool NoTextToSend(const std::string& privmsg, const SOCKET& sock);
	bool CanNotSendToChannel(const std::string& msgTarget, const SOCKET& sock);
	bool NoSuchNick(const std::string& msgTarget, const SOCKET& sock);
private:
	Data& mData = Data::GetInstance();
};