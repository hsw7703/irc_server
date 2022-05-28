#include "InviteCommand.h"

InviteCommand::InviteCommand() {

}

InviteCommand::~InviteCommand() {

}

void InviteCommand::Run(const std::string& msg, const SOCKET& sock) {
	size_t beforeFound;
	size_t afterFound;
	std::string nick;
	std::string ch;

	beforeFound = msg.find(" ");
	afterFound = msg.find(" ", beforeFound + 1);
	nick = msg.substr(beforeFound + 1, afterFound - beforeFound - 1);
	ch = msg.substr(afterFound + 1, msg.size() - afterFound - 1);

	if (this->Error(nick, ch, sock))
		return;

	this->mData.GetMutex().lock();
	SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " INVITE " + this->mData.FindUser(nick)->GetNick() + " " + ch, this->mData.FindUser(nick)->GetSocket());
	SendMsg(":" + this->mData.GetServName() + " " + RPL_INVITING + " " + ch + " " + nick, sock);
	this->mData.GetMutex().unlock();
}

bool InviteCommand::Error(const std::string& nick, const std::string& ch, const SOCKET& sock) {
	this->mData.GetMutex().lock();
	if (this->NoSuchNick(nick, sock))
		return true;
	if (this->NotOnChannel(ch, sock))
		return true;
	if (this->UserOnChannel(nick, ch, sock))
		return true;
	if (this->NotChannelOperator(ch, sock))
		return true;
	this->mData.GetMutex().unlock();
	return false;
}

bool InviteCommand::NoSuchNick(const std::string& nick, const SOCKET& sock) {
	if (this->mData.FindUser(nick) == NULL) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOSUCHNICK + " " + this->mData.GetUser()[sock].GetNick() + " " + nick + " :No such nick/channel", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool InviteCommand::NotOnChannel(const std::string& ch, const SOCKET& sock) {
	if (!this->mData.GetUser()[sock].FindChannel(ch)) {
		this->mData.GetMutex().unlock();
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NOTONCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + ch + " :You're not on that channel", sock);
		return true;
	}
	return false;
}

bool InviteCommand::UserOnChannel(const std::string& nick, const std::string& ch, const SOCKET& sock) {
	if (this->mData.FindUser(nick)->FindChannel(ch)) {
		this->mData.GetMutex().unlock();
		SendMsg(":" + this->mData.GetServName() + " " + ERR_USERONCHANNEL + " " + this->mData.GetUser()[sock].GetNick() + " " + nick + " " + ch + " :is already on channel", sock);
		return true;
	}
	return false;
}

bool InviteCommand::NotChannelOperator(const std::string& ch, const SOCKET& sock) {
	if (this->mData.GetChannel().find(ch)->second.GetAdmin() != this->mData.GetUser()[sock].GetNick()) {
		this->mData.GetMutex().unlock();
		SendMsg(":" + this->mData.GetServName() + " " + ERR_CHANOPRIVSNEEDED + " " + this->mData.GetUser()[sock].GetNick() + " " + ch + " :You're not channel operator", sock);
		return true;
	}
	return false;
}