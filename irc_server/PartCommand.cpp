#include "PartCommand.h"

PartCommand::PartCommand() {

}

PartCommand::~PartCommand() {

}

void PartCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::vector<std::string> channelList;
	std::string partMsg;

	if (msg == "0") {
		channelList = this->mData.GetUser()[sock].GetChannel();
		partMsg = "Left all channels";
	}
	else {
		channelList = ChannelParser(msg);
		partMsg = msg.substr(msg.find(":") + 1, msg.size() - msg.find(":") - 1);
	}

	for (size_t i = 0; i < channelList.size(); i++) {
		if (this->Error(channelList[i], sock))
			continue;

		this->mData.GetMutex().lock();
		for (size_t j = 0; j < this->mData.GetChannel()[channelList[i]].GetUser().size(); j++) {
			SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " PART " + channelList[i] + " :" + partMsg, this->mData.GetChannel()[channelList[i]].GetUser()[j]->GetSocket());
		}
		if (this->mData.GetChannel()[channelList[i]].GetAdmin() == this->mData.GetUser()[sock].GetNick())
			this->mData.GetChannel()[channelList[i]].SetAdmin("");
		this->mData.GetChannel()[channelList[i]].EraseUser(&this->mData.GetUser()[sock]);
		this->mData.GetUser()[sock].EraseChannel(channelList[i]);
		if (this->mData.GetChannel()[channelList[i]].GetUser().size() == 0)
			this->mData.GetChannel().erase(channelList[i]);
		this->mData.GetMutex().unlock();
	}
}

bool PartCommand::Error(const std::string& msg, const SOCKET& sock) {
	if (this->NoSuchChannel(msg, sock))
		return true;
	if (this->NotOnChannel(msg, sock))
		return true;
	return false;
}

bool PartCommand::NoSuchChannel(const std::string& msg, const SOCKET& sock) {
	if (this->mData.GetChannel().find(msg) == this->mData.GetChannel().end()) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOSUCHCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + msg + " :No such channel", sock);
		return true;
	}
	return false;
}

bool PartCommand::NotOnChannel(const std::string& msg, const SOCKET& sock) {
	if (!this->mData.GetUser()[sock].FindChannel(msg)) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOTONCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + msg + " :You're not on that channel", sock);
		return true;
	}
	return false;
}