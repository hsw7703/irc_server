#pragma once

#include "Data.h"

class TopicCommand
{
public:
	TopicCommand();
	~TopicCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
	bool Error(const std::string& msg, const SOCKET& sock);
	bool NotOnChannel(const std::string& msg, const SOCKET& sock);
	bool NotChannelOperator(const std::string& msg, const SOCKET& sock);
private:
	Data& mData = Data::GetIncetence();
};

