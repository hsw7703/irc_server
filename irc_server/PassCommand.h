#pragma once

#include "User.h"
#include "Data.h"

#include <map>

class PassCommand
{
public:
	PassCommand();
	~PassCommand();
	void Run(const std::string &msg, const SOCKET& sock);
public:
private:
	bool Error(const std::string& msg, const SOCKET& sock);
	bool AlreadyRegister(const SOCKET& sock);
	bool NeedMoreParams(const std::string& msg, const SOCKET& sock);

private:
	Data& mData = Data::GetInstance();
};