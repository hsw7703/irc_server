#pragma once

#include "Data.h"

class ListCommand
{
public:
	ListCommand();
	~ListCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
private:
	Data& mData = Data::GetIncetence();
};