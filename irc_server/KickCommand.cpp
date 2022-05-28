#include "KickCommand.h"

KickCommand::KickCommand() {

}

KickCommand::~KickCommand() {

}

void KickCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::string ch;
	std::string nick;
	std::string comment = "";
	size_t beforeFound = 0;
	size_t afterFound;

	beforeFound = msg.find(" ") + 1;
	afterFound = msg.find(" ", beforeFound);
	ch = msg.substr(beforeFound, afterFound - beforeFound);
	beforeFound = afterFound + 1;
	afterFound = msg.find(" ", beforeFound);
	if (afterFound == std::string::npos) {
		nick = msg.substr(beforeFound, msg.size() - beforeFound);
	}
	else {
		nick = msg.substr(beforeFound, afterFound - beforeFound);
		comment = msg.substr(afterFound + 1, msg.size() - afterFound - 1);
	}

	if (this->Error(ch, nick, sock))
		return;

	std::map<SOCKET, User>::iterator it;

	this->mData.GetMutex().lock();
	for (it = this->mData.GetUser().begin(); it != this->mData.GetUser().end(); it++) {
		SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " KICK " + ch + " " + nick + " :" + comment, it->first);
	}
	this->mData.GetChannel()[ch].EraseUser(this->mData.FindUser(nick));
	if (this->mData.GetChannel()[ch].GetUser().size() == 0)
		this->mData.GetChannel().erase(ch);
	this->mData.FindUser(nick)->EraseChannel(ch);
	this->mData.GetMutex().unlock();
}

bool KickCommand::Error(const std::string& ch, const std::string& nick, const SOCKET& sock) {
	this->mData.GetMutex().lock();
	if (this->NoSuchChannel(ch, sock))
		return true;
	if (this->NotChannelOperator(ch, sock))
		return true;
	if (this->UserNotInChannel(ch, nick, sock))
		return true;
	if (this->NotOnChannel(ch, sock))
		return true;
	this->mData.GetMutex().unlock();
	return false;
}

bool KickCommand::NoSuchChannel(const std::string& ch, const SOCKET& sock) {
	if (this->mData.GetChannel().find(ch) == this->mData.GetChannel().end()) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOSUCHCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + ch + " :No such channel", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool KickCommand::NotChannelOperator(const std::string& ch, const SOCKET& sock) {
	if (this->mData.GetChannel().find(ch)->second.GetAdmin() != this->mData.GetUser()[sock].GetNick()) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_CHANOPRIVSNEEDED + " " + this->mData.GetUser()[sock].GetNick() + " " + ch + " :You're not channel operator", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool KickCommand::UserNotInChannel(const std::string& ch, const std::string& nick, const SOCKET& sock) {
	if (!this->mData.FindUser(nick)->FindChannel(ch)) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_USERNOTINCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + nick + " " + ch + " :They aren't on that channel", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool KickCommand::NotOnChannel(const std::string& ch, const SOCKET& sock) {
	if (!this->mData.GetUser()[sock].FindChannel(ch)) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOTONCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + ch + " :You're not on that channel", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}