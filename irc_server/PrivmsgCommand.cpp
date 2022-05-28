#include "PrivmsgCommand.h"

PrivmsgCommand::PrivmsgCommand() {

}

PrivmsgCommand::~PrivmsgCommand() {

}

void PrivmsgCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::string msgTarget = "";
	std::string privmsg = "";
	std::string command;
	size_t beforeFound;
	size_t afterFound;

	beforeFound = msg.find(" ");
	afterFound = msg.find(" :");
	command = msg.substr(0, beforeFound);
	std::cout << command << std::endl;
	if (afterFound == std::string::npos) {
		afterFound = msg.find(" ", beforeFound + 1);
	}
	if (beforeFound != std::string::npos)
		msgTarget = msg.substr(beforeFound + 1, afterFound - beforeFound - 1);
	if (afterFound != std::string::npos)
		privmsg = msg.substr(afterFound + 1, msg.size() - afterFound - 1);

	if (this->Error(command, msgTarget, privmsg, sock))
		return;

	this->mData.GetMutex().lock();
	if (msgTarget[0] == '#') {
		for (size_t i = 0; i < this->mData.GetChannel()[msgTarget].GetUser().size(); i++) {
			if (this->mData.GetChannel()[msgTarget].GetUser()[i]->GetSocket() != sock)
				SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " " + command + " " + msgTarget + " " + privmsg, this->mData.GetChannel()[msgTarget].GetUser()[i]->GetSocket());
		}
	}
	else {
		SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " " + command + " " + msgTarget + " " + privmsg, this->mData.FindUser(msgTarget)->GetSocket());
	}
	this->mData.GetMutex().unlock();
}

bool PrivmsgCommand::Error(const std::string& command, const std::string& msgTarget, const std::string& privmsg, const SOCKET& sock) {
	this->mData.GetMutex().lock();
	if (NoRecioient(command, msgTarget, sock))
		return true;
	if (this->NoTextToSend(privmsg, sock))
		return true;
	if (this->CanNotSendToChannel(msgTarget, sock))
		return true;
	if (this->NoSuchNick(msgTarget, sock))
		return true;
	this->mData.GetMutex().unlock();
	return false;
}

bool PrivmsgCommand::NoRecioient(const std::string& command, const std::string& msgTarget, const SOCKET& sock) {
	if (msgTarget == "") {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NORECIPIENT + " " + this->mData.GetUser()[sock].GetNick() + " :No recipient given (" + command + ")", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool PrivmsgCommand::NoTextToSend(const std::string& privmsg, const SOCKET& sock) {
	if (privmsg == "") {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOTEXTTOSEND + " " + this->mData.GetUser()[sock].GetNick() + " :No text to send", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool PrivmsgCommand::CanNotSendToChannel(const std::string& msgTarget, const SOCKET& sock) {
	if (msgTarget[0] == '#' && this->mData.GetChannel().find(msgTarget) == this->mData.GetChannel().end()) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_CANNOTSENDTOCHAN + " " + this->mData.GetUser()[sock].GetNick() + " " + msgTarget + " :Cannot send to channel", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool PrivmsgCommand::NoSuchNick(const std::string& msgTarget, const SOCKET& sock) {
	if (!this->mData.FindUser(msgTarget) && this->mData.GetChannel().find(msgTarget) == this->mData.GetChannel().end()) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOSUCHNICK + " " + this->mData.GetUser()[sock].GetNick() + " " + msgTarget + " :No such nick/channel", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}