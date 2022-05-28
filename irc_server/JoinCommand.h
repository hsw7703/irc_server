#pragma once

#include "Data.h"
#include "PartCommand.h"

class JoinCommand
{
public:
	JoinCommand();
	~JoinCommand();
	void Run(const std::string& msg, const SOCKET& sock);
public:
private:
	int Error(const std::string& msg, const SOCKET& sock);
	bool NoSuchChannel(const std::string& msg, const SOCKET& sock);
	bool TooManyChannels(const std::string& msg, const SOCKET& sock);

//	bool NeedMoreParams(std::string msg, SOCKET sock);
private:
	Data& mData = Data::GetIncetence();
	PartCommand mPartCommand;
};