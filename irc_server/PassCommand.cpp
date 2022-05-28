#include "PassCommand.h"

PassCommand::PassCommand() {

}

PassCommand::~PassCommand() {

}

void PassCommand::Run(const std::string& msg, const SOCKET& sock) {
	size_t found;

	if (this->Error(msg, sock))
		return;
	found = msg.find(" ") + 1;

	this->mData.GetMutex().lock();
	if (this->mData.GetPassword() == msg.substr(found, msg.size() - found)) {
		this->mData.GetUser()[sock].SetPass();
	}
	if (this->mData.GetUser()[sock].IsLogin() && !this->mData.GetUser()[sock].IsConnect())
		this->mData.GetUser()[sock].Login(this->mData.GetServName(), this->mData.GetDate(), this->mData.GetVersion());
	this->mData.GetMutex().unlock();
}

bool PassCommand::Error(const std::string& msg, const SOCKET& sock) {
	this->mData.GetMutex().lock();
	if (this->AlreadyRegister(sock))
		return true;
	if (this->NeedMoreParams(msg, sock))
		return true;
	this->mData.GetMutex().unlock();

	return false;
}

bool PassCommand::AlreadyRegister(const SOCKET& sock) {
	if (this->mData.GetUser()[sock].GetPass()) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_ALREADYREGISTER + " " + this->mData.GetUser()[sock].GetNick() + " :already registered", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool PassCommand::NeedMoreParams(const std::string& msg, const SOCKET& sock) {
	if (msg == "PASS") {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NEEDMOREPARAMS + " " + this->mData.GetUser()[sock].GetNick() + " PASS :Not enough parameters", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}