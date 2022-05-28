#include "NamesCommand.h"

NamesCommand::NamesCommand() {

}

NamesCommand::~NamesCommand() {

}

void NamesCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::vector<std::string> channelList = ChannelParser(msg);
	std::string sendMsg;

	for (size_t i = 0; i < channelList.size(); i++) {
		if (this->mData.GetChannel().find(channelList[i]) == this->mData.GetChannel().end()
			|| !this->mData.GetUser()[sock].FindChannel(channelList[i])) {
			continue;
		}

		sendMsg.assign(":" + this->mData.GetServName() + " " + RPL_NAMREPLY + " " + this->mData.GetUser()[sock].GetNick() + " = " + channelList[i] + " :");
		this->mData.GetMutex().lock();
		for (size_t j = 0; j < this->mData.GetChannel()[channelList[i]].GetUser().size(); j++) {
			if (this->mData.GetChannel()[channelList[i]].GetUser()[j]->GetNick() == this->mData.GetChannel()[channelList[i]].GetAdmin())
				sendMsg += "@" + this->mData.GetChannel()[channelList[i]].GetUser()[j]->GetNick();
			else
				sendMsg += " " + this->mData.GetChannel()[channelList[i]].GetUser()[j]->GetNick();
		}
		SendMsg(sendMsg, sock);
		this->mData.GetMutex().unlock();
		SendMsg(":" + this->mData.GetServName() + " " + RPL_ENDOFNAMES + " " + this->mData.GetUser()[sock].GetNick() + " " + channelList[i] + " :End of NAMES list", sock);
	}
}