#include "WhoIsCommand.h"

WhoIsCommand::WhoIsCommand() {

}

WhoIsCommand::~WhoIsCommand() {

}

void WhoIsCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::string target = "";
	size_t afterFound;
	size_t beforeFound;
	User* user;

	beforeFound = msg.find(" ");
	afterFound = msg.find(" ", beforeFound + 1);
	if (beforeFound != std::string::npos) {
		if (afterFound != std::string::npos)
			target = msg.substr(beforeFound + 1, afterFound - beforeFound - 1);
		else
			target = msg.substr(beforeFound + 1, msg.size() - beforeFound - 1);
	}

	if (this->Error(target, sock))
		return;

	this->mData.GetMutex().lock();
	user = this->mData.FindUser(target);
	SendMsg(":" + this->mData.GetServName() + " " + RPL_WHOISUSER + " " + this->mData.GetUser()[sock].GetNick() + " " + user->GetNick() + " " + user->GetName() + " " + user->GetHost() + " * :" + user->GetRealname(), sock);
	SendMsg(":" + this->mData.GetServName() + " " + RPL_WHOISSERVER + " " + this->mData.GetUser()[sock].GetNick() + " " + user->GetNick() + " " + this->mData.GetServName() + " :", sock);
	for (size_t i = 0; i < user->GetChannel().size(); i++)
		SendMsg(":" + this->mData.GetServName() + " " + RPL_WHOISCHANNELS + " " + this->mData.GetUser()[sock].GetNick() + " " + user->GetNick() + " :" + user->GetChannel()[i], sock);
	SendMsg(":" + this->mData.GetServName() + " " + RPL_ENDOFWHOIS + " " + this->mData.GetUser()[sock].GetNick() + " " + user->GetNick() + " :End of WHOIS list", sock);
	this->mData.GetMutex().unlock();
}

bool WhoIsCommand::Error(const std::string& target, const SOCKET& sock) {
	this->mData.GetMutex().lock();
	if (this->NoNicknameGiven(target, sock))
		return true;
	if (this->NoSuchNick(target, sock))
		return true;
	this->mData.GetMutex().unlock();
	return false;
}

bool WhoIsCommand::NoNicknameGiven(const std::string& target, const SOCKET& sock) {
	if (target == "") {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NONICKNAMEGIVEN + " " + this->mData.GetUser()[sock].GetNick() + " :No nickname given", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool WhoIsCommand::NoSuchNick(const std::string& target, const SOCKET& sock) {
	if (this->mData.FindUser(target) == NULL) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOSUCHNICK + " " + this->mData.GetUser()[sock].GetNick() + " " + target + " :No such nick/channel", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}