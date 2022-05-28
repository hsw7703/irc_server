#include "NickCommand.h"

NickCommand::NickCommand(){

}

NickCommand::~NickCommand() {
	
}

void NickCommand::Run(const std::string& msg, const SOCKET& sock) {
	std::string nick;
	std::vector<std::string> ch;
	std::vector<User*> user;

	if (this->Error(msg, nick, sock))
		return;

	this->mData.GetMutex().lock();
	if (this->mData.GetUser()[sock].IsConnect()) {
		ch = this->mData.GetUser()[sock].GetChannel();

		for (size_t i = 0; i < ch.size(); i++) {
			user = this->mData.GetChannel()[ch[i]].GetUser();
			for (size_t j = 0; j < user.size(); j++) {
				if (user[j]->GetNick() != this->mData.GetUser()[sock].GetNick()) {
					SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " NICK " + nick, user[j]->GetSocket());
				}
			}
			if (this->mData.GetChannel()[ch[i]].GetAdmin() == this->mData.GetUser()[sock].GetNick())
				this->mData.GetChannel()[ch[i]].SetAdmin(nick);
		}
		SendMsg(":" + this->mData.GetUser()[sock].GetMask() + " NICK " + nick, sock);
		this->mData.AddUserHistory(this->mData.GetUser()[sock]);
		this->mData.GetUser()[sock].SetNick(nick);
		this->mData.GetMutex().unlock();
		return;
	}
	this->mData.GetUser()[sock].SetNick(nick);
	if (this->mData.GetUser()[sock].IsLogin()) {
		this->mData.GetUser()[sock].Login(this->mData.GetServName(), this->mData.GetDate(), this->mData.GetVersion());
	}
	this->mData.GetMutex().unlock();
}

bool NickCommand::Error(const std::string& msg, std::string& nick, const SOCKET& sock) {
	size_t found;

	found = msg.find(" ") + 1;
	if (msg.find(" ", found) == std::string::npos)
		nick = msg.substr(found, msg.size() - found);
	else
		nick = msg.substr(found, msg.find(" ", found) - found);
	
	this->mData.GetMutex().lock();
	if (this->NoNicknameGiven(msg, sock))
		return true;
	if (this->ErroneousNickname(nick, sock))
		return true;
	if (this->NicknameInuse(nick, sock))
		return true;
	this->mData.GetMutex().unlock();
	return false;
}

bool NickCommand::NoNicknameGiven(const std::string& msg, const SOCKET& sock) {
	if (msg == "NICK") {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_NONICKNAMEGIVEN + " " + this->mData.GetUser()[sock].GetNick() + " :No nickname given", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	return false;
}

bool NickCommand::ErroneousNickname(const std::string& nick, const SOCKET& sock) {
	if (!isalpha(nick[0]) && !IsSpecial(nick[0])) {
		SendMsg(":" + this->mData.GetServName() + " " + ERR_ERRONEOUSNICKNAME + " " + this->mData.GetUser()[sock].GetNick() + " " + nick + " :Erroneous nickname", sock);
		this->mData.GetMutex().unlock();
		return true;
	}
	for (size_t i = 1; i < nick.size(); i++) {
		if (!isalpha(nick[i]) && !isdigit(nick[i]) && !IsSpecial(nick[i]) && nick[i] != '-') {
			SendMsg(":" + this->mData.GetServName() + " " + ERR_ERRONEOUSNICKNAME + " " + this->mData.GetUser()[sock].GetNick() + " " + nick + " :Erroneous nickname", sock);
			this->mData.GetMutex().unlock();
			return true;
		}
	}
	return false;
}

bool NickCommand::NicknameInuse(const std::string& nick, const SOCKET& sock) {
	std::map<SOCKET, User>::iterator it;

	for (it = this->mData.GetUser().begin(); it != this->mData.GetUser().end(); it++) {
		if (it->second.GetNick() == nick) {
			SendMsg(":" + this->mData.GetServName() + " " + ERR_NICKNAMEINUSE + " " + this->mData.GetUser()[sock].GetNick() + " " + nick + " :Nickname is already in use", sock);
			this->mData.GetMutex().unlock();
			return true;
		}
	}
	return false;
}