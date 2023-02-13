#pragma once

#include "Data.h"

class QuitCommand
{
public:
	QuitCommand();
	~QuitCommand();
	void Run(std::string msg, SOCKET sock);
public:
private:
private:
	Data& mData = Data::GetInstance();
};

