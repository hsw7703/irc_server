#include "UserHostCommand.h"

UserHostCommand::UserHostCommand() {

}

UserHostCommand::~UserHostCommand() {

}

void UserHostCommand::Run(const std::string& msg, const SOCKET& sock) {
	size_t beforeFound;
	size_t afterFound;
	std::vector<std::string> userList;

	if (this->NeedMoreParams(msg, sock))
		return;

	beforeFound = msg.find(" ") + 1;
	while (1) {
		afterFound = msg.find(" ", beforeFound);
		if (afterFound == std::string::npos) {
			userList.push_back(msg.substr(beforeFound, msg.size() - beforeFound));
			break;
		}
		userList.push_back(msg.substr(beforeFound, afterFound - beforeFound));
		beforeFound = afterFound + 1;
	}
	this->mData.GetMutex().lock();
	User* user;
	std::string sendMsg = ":" + this->mData.GetServName() + " " + RPL_USERHOST + " " + this->mData.GetUser()[sock].GetNick();
	for (size_t i = 0; i < userList.size() && i < 5; i++) {
		if ((user = this->mData.FindUser(userList[i])) != NULL) {
			sendMsg += " " + user->GetNick() + "=" + user->GetName() + "@" + user->GetHost();
		}
	}
	this->mData.GetMutex().unlock();
	SendMsg(sendMsg, sock);
}

bool UserHostCommand::NeedMoreParams(const std::string& msg, const SOCKET& sock) {
	if (AllToUpper(msg) == "USERHOST") {
		this->mData.GetMutex().lock();
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NEEDMOREPARAMS + " " + this->mData.GetUser()[sock].GetNick() + " USERHOST :Not enough parameters", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}