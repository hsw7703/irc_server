#pragma once

#include "Data.h"

class PartCommand
{
public:
	PartCommand();
	~PartCommand();
	void Run(const std::string& msg, const SOCKET& sock);
public:
private:
	bool Error(const std::string& msg, const SOCKET& sock);
	bool NoSuchChannel(const std::string& msg, const SOCKET& sock);
	bool NotOnChannel(const std::string& msg, const SOCKET& sock);
private:
	Data& mData = Data::GetIncetence();
};