#pragma once

#include "User.h"
#include "Server.h"
#include "Channel.h"
#include "Define.h"
#include "Utils.h"
#include "Data.h"

#include "InviteCommand.h"
#include "JoinCommand.h"
#include "KickCommand.h"
#include "ListCommand.h"
#include "NickCommand.h"
#include "NamesCommand.h"
#include "PartCommand.h"
#include "PassCommand.h"
#include "PrivmsgCommand.h"
#include "QuitCommand.h"
#include "TopicCommand.h"
#include "UserCommand.h"
#include "TimeCommand.h"
#include "WhoIsCommand.h"
#include "WhoWasCommand.h"
#include "PingPongCommand.h"
#include "UserHostCommand.h"

#include <map>
#include <vector>
#include <cctype>

class Command {
public:
	Command();
	~Command();
	void Run(std::string msg, SOCKET sock);
public:
private:
private:
	Data& mData = Data::GetIncetence();

	NickCommand mNickCommand;
	PassCommand mPassCommand;
	UserCommand mUserCommand;
	QuitCommand mQuitCommand;
	JoinCommand mJoinCommand;
	PartCommand mPartCommand;
	TopicCommand mTopicCommand;
	NamesCommand mNamesCommand;
	ListCommand mListCommand;
	InviteCommand mInviteCommand;
	KickCommand mKickCommand;
	PrivmsgCommand mPrivmsgCommand;
	TimeCommand mTimeCommand;
	WhoIsCommand mWhoIsCommand;
	WhoWasCommand mWhoWasCommand;
	PingPongCommand mPingPongCommand;
	UserHostCommand mUserHostCommand;
};