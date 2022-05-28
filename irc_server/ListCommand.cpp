#include "ListCommand.h"

ListCommand::ListCommand() {

}

ListCommand::~ListCommand() {

}

void ListCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::map<std::string, Channel>::iterator it;

	this->mData.GetMutex().lock();
	for (it = this->mData.GetChannel().begin(); it != this->mData.GetChannel().end(); it++) {
		SendMsg(":" + this->mData.GetServName() + " " + RPL_LIST + " " + this->mData.GetUser()[sock].GetNick() + " " + it->first + " " + std::to_string(it->second.GetUser().size()) + " :" + it->second.GetTopic(), sock);
	}
	SendMsg(":" + this->mData.GetServName() + " " + RPL_LISTEND + " " + this->mData.GetUser()[sock].GetNick() + " :End of LIST", sock);
	this->mData.GetMutex().unlock();
}