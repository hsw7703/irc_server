#include "QuitCommand.h"

QuitCommand::QuitCommand() {

}

QuitCommand::~QuitCommand() {
}

void QuitCommand::Run(std::string msg, SOCKET sock) {
	std::string sendMsg;
	std::vector<std::string> ch;
	std::vector<User*> user;

	sendMsg = msg.substr(msg.find(":"), msg.size() - msg.find(":"));

	this->mData.GetMutex().lock();
	ch = this->mData.GetUser()[sock].GetChannel();
	for (size_t i = 0; i < ch.size(); i++) {
		this->mData.GetChannel()[ch[i]].EraseUser(&this->mData.GetUser()[sock]);
		user = this->mData.GetChannel()[ch[i]].GetUser();
		for (size_t j = 0; j < user.size(); j++) {
			if (user[j]->GetNick() != this->mData.GetUser()[sock].GetNick()) {
				SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " QUIT " + sendMsg, user[j]->GetSocket());
			}
		}
		if (user.empty())
			this->mData.GetChannel().erase(ch[i]);
	}
	this->mData.GetMutex().unlock();
	SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " QUIT " + sendMsg, sock);
}