#pragma once

#include "Data.h"

class PingPongCommand
{
public:
	PingPongCommand();
	~PingPongCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
	
private:
	Data& mData = Data::GetIncetence();
};