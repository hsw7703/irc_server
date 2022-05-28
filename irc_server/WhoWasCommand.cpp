#include "WhoWasCommand.h"

WhoWasCommand::WhoWasCommand() {

}

WhoWasCommand::~WhoWasCommand() {

}

void WhoWasCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::string nick;
	std::vector<std::string> nickList;
	int count = -1;
	size_t afterFound;
	size_t beforeFound;

	if (this->NoNicknameGiven(msg, sock))
		return;

	beforeFound = msg.find(" ");
	afterFound = msg.find(" ", beforeFound + 1);

	if (afterFound == std::string::npos) {
		nick = msg.substr(beforeFound + 1, msg.size() - beforeFound);
	}
	else {
		nick = msg.substr(beforeFound + 1, afterFound - beforeFound - 1);
		try {
			count = stoi(msg.substr(afterFound + 1, msg.size() - afterFound - 1));
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
			count = -1;
		}
	}
	std::cout << "nick : " << nick << std::endl;
	
	beforeFound = 0;
	while (1) {
		afterFound = nick.find(",", beforeFound);
		if (afterFound == std::string::npos) {
			nickList.push_back(nick.substr(beforeFound, nick.size() - beforeFound));
			break;
		}
		nickList.push_back(nick.substr(beforeFound, afterFound - beforeFound));
		beforeFound = afterFound + 1;
	}

	std::vector<UserHistory> userHistory;

	this->mData.GetMutex().lock();
	for (size_t i = 0; i < nickList.size(); i++) {
		if (this->WasNoSuchNick(nickList[i], sock))
			continue;
		userHistory = this->mData.FindUserHistory(nickList[i]);
		for (size_t j = 0; j < userHistory.size() && (count < 0 || count > static_cast<int>(j)); j++) {
			SendMsg(":" + this->mData.GetServName() + " " + RPL_WHOWASUSER + " " + this->mData.GetUser()[sock].GetNick() + " " + nickList[i] + " " + userHistory[j].GetName() + " " + userHistory[j].GetHost() + " * :" + userHistory[j].GetRealname(), sock);
			SendMsg(":" + this->mData.GetServName() + " " + RPL_WHOISSERVER + " " + this->mData.GetUser()[sock].GetNick() + " " + nickList[i] + " " + this->mData.GetServName() + " :" + userHistory[j].GetDate(), sock);
		}
		userHistory.clear();
	}
	SendMsg(":" + this->mData.GetServName() + " " + RPL_ENDOFWHOWAS + " " + this->mData.GetUser()[sock].GetNick() + " " + nick + " :End of WHOWAS", sock);
	this->mData.GetMutex().unlock();
}

bool WhoWasCommand::NoNicknameGiven(const std::string& msg, const SOCKET& sock) {
	this->mData.GetMutex().lock();
	if (AllToUpper(msg) == "WHOWAS") {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NONICKNAMEGIVEN + " " + this->mData.GetUser()[sock].GetNick() + " :No nickname given", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	this->mData.GetMutex().unlock();
	return false;
}

bool WhoWasCommand::WasNoSuchNick(const std::string& nick, const SOCKET& sock) {
	if (!this->mData.FindUser(nick)) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_WASNOSUCHNICK + " " + this->mData.GetUser()[sock].GetNick() + " " + nick + " :There was no such nickname", sock);
		return true;
	}
	return false;
}
