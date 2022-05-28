#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <WinSock2.h>

#include <iostream>

#include "Define.h"

#define BUF_SIZE	1024
#define READ		3
#define WRITE		5

typedef struct {
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode;
} PER_IO_DATA, * LPPER_IO_DATA;

void SendMsg(std::string msg, SOCKET sock);
bool IsSpecial(char c);
std::vector<std::string> ChannelParser(const std::string& msg);
std::string AllToUpper(const std::string msg);
std::string DateToString();

#endif