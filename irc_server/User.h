#pragma once

#include <WinSock2.h>

#include <iostream>
#include <vector>

#include "Utils.h"
//#include "Data.h"

class User
{
public:
	User();
	User(SOCKET sock);
	~User();
	User(const User& user);
	User &operator =(const User& user);
	const SOCKET &GetSocket() const;
	void SetNick(const std::string& nick);
	const std::string& GetNick() const;
	void SetName(const std::string& name);
	const std::string& GetName() const;
	void SetRealname(const std::string& realname);
	const std::string& GetRealname() const;
	void SetHost(const std::string& host);
	const std::string& GetHost() const;
	void SetPass();
	const bool& GetPass() const;
	void SetConnect();
	const std::vector<std::string>& GetChannel() const;
	void AddChannel(std::string ch);
	const bool FindChannel(std::string ch) const;
	void EraseChannel(std::string ch);
	const std::string GetMask() const;
	bool IsLogin();
	bool IsConnect();
	void Login(std::string servName, std::string date, std::string ver);
	
public:
	
private:
	
private:
	SOCKET mSock;
	std::string mNick = "";
	std::string mName = "";
	std::string mRealname = "";
	std::string mHost;
	bool mbPass = false;
	bool mbConnect = false;

	std::vector<std::string> mChannel;
//	Data& mData = Data::GetIncetence();
};