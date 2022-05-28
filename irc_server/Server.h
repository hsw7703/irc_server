#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <process.h>

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>

#include "User.h"
#include "Command.h"
#include "Channel.h"
#include "Utils.h"
#include "Data.h"

typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

class Server {
public:
	Server();
	~Server();

public:
	
private:
	void ErrorHandling(std::string msg);
	void ThreadMain(HANDLE pComPort);

private:
	Data& mData = Data::GetIncetence();
};