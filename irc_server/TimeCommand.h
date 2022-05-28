#pragma once

#include "Data.h"

class TimeCommand
{
public:
	TimeCommand();
	~TimeCommand();
	void Run(const SOCKET& sock);
private:
private:
	Data& mData = Data::GetIncetence();
};