#pragma once

#include "Data.h"

class NamesCommand
{
public:
	NamesCommand();
	~NamesCommand();
	void Run(const std::string& msg, const SOCKET& sock);
private:
private:
	Data& mData = Data::GetIncetence();
};