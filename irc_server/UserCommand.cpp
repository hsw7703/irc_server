#include "UserCommand.h"

UserCommand::UserCommand() {

}

UserCommand::~UserCommand() {

}

void UserCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::vector<std::string> vMsg;

	vMsg = this->Parser(msg);

	if (this->Error(vMsg, sock))
		return;

	this->mData.GetMutex().lock();
	this->mData.GetUser()[sock].SetName(vMsg[0]);
	this->mData.GetUser()[sock].SetRealname(vMsg[3]);
	if (this->mData.GetUser()[sock].IsLogin() && !this->mData.GetUser()[sock].IsConnect())
		this->mData.GetUser()[sock].Login(this->mData.GetServName(), this->mData.GetDate(), this->mData.GetVersion());
	this->mData.GetMutex().unlock();
}

std::vector<std::string> UserCommand::Parser(const std::string& msg) {
	size_t beforeFound = 0, afterFound;
	std::vector<std::string> vMsg;
	int i = 0;

	afterFound = msg.find(" ");
	while (i < 3 && afterFound != std::string::npos) {
		beforeFound = afterFound + 1;
		afterFound = msg.find(" ", beforeFound);
		vMsg.push_back(msg.substr(beforeFound, afterFound - beforeFound));
		i++;
	}
	beforeFound = afterFound + 1;
	afterFound = msg.find(":", beforeFound) + 1;
	vMsg.push_back(msg.substr(afterFound, msg.size() - afterFound));
	return vMsg;
}

bool UserCommand::Error(const std::vector<std::string>& msg, const SOCKET& sock) {
	this->mData.GetMutex().lock();
	if (this->AlreadyRegister(sock))
		return true;
	if (this->NeedMoreParams(msg, sock))
		return true;
	this->mData.GetMutex().unlock();
	return false;
}

bool UserCommand::AlreadyRegister(const SOCKET& sock) {
	if (this->mData.GetUser()[sock].IsConnect()) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_ALREADYREGISTER + " " + this->mData.GetUser()[sock].GetNick() + " :already registered", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool UserCommand::NeedMoreParams(const std::vector<std::string>& msg, const SOCKET& sock) {
	if (msg.size() < 4) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NEEDMOREPARAMS + " " + this->mData.GetUser()[sock].GetNick() + " USER :Not enough parameters", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}