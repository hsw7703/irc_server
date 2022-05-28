#include "Command.h"

Command::Command() {

}

Command::~Command() {

}

void Command::Run(std::string msg, SOCKET sock) {
	std::string upperMsg = AllToUpper(msg);

	std::cout << msg << std::endl;
	if (!strncmp(upperMsg.c_str(), "PASS ", 5) || upperMsg == "PASS")
		this->mPassCommand.Run(msg, sock);
	else if (!strncmp(upperMsg.c_str(), "NICK ", 5) || upperMsg == "NICK")
		this->mNickCommand.Run(msg, sock);
	else if (!strncmp(upperMsg.c_str(), "USER ", 5) || upperMsg == "USER")
		this->mUserCommand.Run(msg, sock);
	this->mData.GetMutex().lock();
	if (this->mData.GetUser()[sock].IsLogin()) {
		this->mData.GetMutex().unlock();
		if (!strncmp(upperMsg.c_str(), "QUIT ", 5))
			this->mQuitCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "JOIN ", 5))
			this->mJoinCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "PART ", 5))
			this->mPartCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "TOPIC ", 6))
			this->mTopicCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "NAMES ", 6))
			this->mNamesCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "LIST ", 5) || upperMsg == "LIST")
			this->mListCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "INVITE ", 7))
			this->mInviteCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "KICK ", 5))
			this->mKickCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "PRIVMSG ", 8) || upperMsg == "PRIVMSG"
			|| !strncmp(upperMsg.c_str(), "NOTICE ", 7) || upperMsg == "NOTICE")
			this->mPrivmsgCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "TIME ", 5) || upperMsg == "TIME")
			this->mTimeCommand.Run(sock);
		else if (!strncmp(upperMsg.c_str(), "WHOIS ", 6) || upperMsg == "WHOIS")
			this->mWhoIsCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "WHOWAS ", 7) || upperMsg == "WHOWAS")
			this->mWhoWasCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "PING ", 5))
			this->mPingPongCommand.Run(msg, sock);
		else if (!strncmp(upperMsg.c_str(), "USERHOST ", 9) || upperMsg == "USERHOST")
			this->mUserHostCommand.Run(msg, sock);
		return;
	}
	this->mData.GetMutex().unlock();
}