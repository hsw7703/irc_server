#include "TopicCommand.h"

TopicCommand::TopicCommand() {

}

TopicCommand::~TopicCommand() {

}

void TopicCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::string ch = msg.substr(msg.find(" ") + 1, msg.find(" :") - msg.find(" ") - 1);
	std::string topic = "";

	if (this->Error(ch, sock))
		return;

	if (msg.find(" :") != std::string::npos)
		topic = msg.substr(msg.find(" :") + 2, msg.size() - msg.find(" :") - 1);
	if (msg.find(" :") == std::string::npos) {
		if (this->mData.GetChannel()[ch].GetTopic() == "") {
			SendMsg(":" + this->mData.GetServName() + " " + RPL_NOTOPIC + " " + this->mData.GetUser()[sock].GetNick() + " " + ch + " :No topic is set", sock);
		}
		else {
			SendMsg(":" + this->mData.GetServName() + " " + RPL_TOPIC + " " + this->mData.GetUser()[sock].GetNick() + " " + ch + " :" + this->mData.GetChannel()[ch].GetTopic(), sock);
		}
		return;
	}
	this->mData.GetMutex().lock();
	for (size_t i = 0; i < this->mData.GetChannel()[ch].GetUser().size(); i++) {
		SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " TOPIC " + ch + " :" + topic, this->mData.GetChannel()[ch].GetUser()[i]->GetSocket());
	}
	this->mData.GetChannel()[ch].SetTopic(topic);
	this->mData.GetMutex().unlock();
}

bool TopicCommand::Error(const std::string& msg, const SOCKET& sock) {
	if (this->NotOnChannel(msg, sock))
		return true;
	if (this->NotChannelOperator(msg, sock))
		return true;
	return false;
}

bool TopicCommand::NotOnChannel(const std::string& msg, const SOCKET& sock) {
	if (!this->mData.GetUser()[sock].FindChannel(msg)) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOTONCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + msg + " :You're not on that channel", sock);
		return true;
	}
	return false;
}

bool TopicCommand::NotChannelOperator(const std::string& msg, const SOCKET& sock) {
	if (this->mData.GetChannel().find(msg)->second.GetAdmin() != this->mData.GetUser()[sock].GetNick()) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_CHANOPRIVSNEEDED + " " + this->mData.GetUser()[sock].GetNick() + " " + msg + " :You're not channel operator", sock);
		return true;
	}
	return false;
}