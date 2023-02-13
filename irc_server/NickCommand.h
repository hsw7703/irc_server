#pragma once

#include <map>

#include "Define.h"
#include "User.h"
#include "Channel.h"
#include "Data.h"

class NickCommand
{
public:
	NickCommand();
	~NickCommand();
	void Run(const std::string& msg, const SOCKET& sock);
	
public:
private:
	bool Error(const std::string& msg, std::string& nick, const SOCKET& sock);
	bool NoNicknameGiven(const std::string& msg, const SOCKET& sock);
	bool ErroneousNickname(const std::string& nick, const SOCKET& sock);
	bool NicknameInuse(const std::string& nick, const SOCKET& sock);
private:
	Data& mData = Data::GetInstance();
};