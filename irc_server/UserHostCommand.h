#pragma once

#include "Data.h"

class UserHostCommand
{
public:
	UserHostCommand();
	~UserHostCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
	bool NeedMoreParams(const std::string& msg, const SOCKET& sock);
private:
	Data& mData = Data::GetInstance();
};