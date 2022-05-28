#include "JoinCommand.h"

JoinCommand::JoinCommand() {

}

JoinCommand::~JoinCommand() {

}

void JoinCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::vector<std::string> channelList = ChannelParser(msg);

	for (size_t i = 0; i < channelList.size(); i++) {
		std::string channelUser = "";
		std::vector<User*> user;

		switch (this->Error(channelList[i], sock))
		{
		case 1:
			continue;
			break;
		case 2:
			return;
		default:
			break;
		}

		if (channelList[i] == "0") {
			this->mPartCommand.Run("0", sock);
			continue;
		}
		// 없는 채널일 경우
		this->mData.GetMutex().lock();
		if (this->mData.GetChannel().find(channelList[i]) != this->mData.GetChannel().end()) {
			if (!this->mData.GetUser()[sock].FindChannel(channelList[i])) {
				this->mData.GetUser()[sock].AddChannel(channelList[i]);
				this->mData.GetChannel()[channelList[i]].AddUser(&this->mData.GetUser()[sock]);
			}
			else {
				this->mData.GetMutex().unlock();
				continue;
			}
		}
		else {
			// 채널에 추가
			this->mData.GetUser()[sock].AddChannel(channelList[i]);
			this->mData.GetChannel()[channelList[i]] = Channel();
			this->mData.GetChannel()[channelList[i]].AddUser(&this->mData.GetUser()[sock]);
			this->mData.GetChannel()[channelList[i]].SetAdmin(this->mData.GetUser()[sock].GetNick());
		}
		user = this->mData.GetChannel()[channelList[i]].GetUser();
		// 채널 관리자
		if (this->mData.GetChannel()[channelList[i]].GetAdmin() != "")
			channelUser = ":@" + this->mData.GetChannel()[channelList[i]].GetAdmin();
		// 채널 유저
		for (size_t j = 0; j < user.size(); j++) {
			SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " JOIN " + channelList[i], user[j]->GetSocket());
			if (user[j]->GetNick() != this->mData.GetChannel()[channelList[i]].GetAdmin())
				channelUser += " " + user[j]->GetNick();
		}
		//topic
		if (this->mData.GetChannel()[channelList[i]].GetTopic() != "")
			SendMsg(":" + this->mData.GetServName() + " " + RPL_TOPIC + " " + this->mData.GetUser()[sock].GetNick() + " " + channelList[i] + " :" + this->mData.GetChannel()[channelList[i]].GetTopic(), sock);
		SendMsg(":" + this->mData.GetServName() + " " + RPL_NAMREPLY + " " + this->mData.GetUser()[sock].GetNick() + " = " + channelList[i] + " " + channelUser, sock);
		SendMsg(":" + this->mData.GetServName() + " " + RPL_ENDOFNAMES + " " + this->mData.GetUser()[sock].GetNick() + " " + channelList[i] + " :End of NAMES list", sock);
		this->mData.GetMutex().unlock();
	}
}

int JoinCommand::Error(const std::string& msg, const SOCKET& sock) {
	this->mData.GetMutex().lock();
	if (this->NoSuchChannel(msg, sock))
		return 1;
	if (this->TooManyChannels(msg, sock))
		return 2;
	this->mData.GetMutex().unlock();
	return false;
}

bool JoinCommand::NoSuchChannel(const std::string& msg, const SOCKET& sock) {
	if (msg[0] != '#' && msg != "0") {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOSUCHCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + msg + " :No such channel", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool JoinCommand::TooManyChannels(const std::string& msg, const SOCKET& sock) {
	if (this->mData.GetUser()[sock].GetChannel().size() >= MAX_CHANNEL) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_TOOMANYCHANNELS + " " + this->mData.GetUser()[sock].GetNick() + " " + msg + " :You have joined too many channels", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}