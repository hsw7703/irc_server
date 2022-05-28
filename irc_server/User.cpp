#include "User.h"

User::User() : mSock(NULL) {
}

User::User(SOCKET sock) : mSock(sock) {
}

User::~User() {
}

User::User(const User& user) {
	this->mSock = user.mSock;
	this->mNick = user.mNick;
	this->mName = user.mName;
	this->mRealname = user.mRealname;
	this->mHost = user.mHost;
}

User &User::operator=(const User& user) {
	this->mSock = user.mSock;
	this->mNick = user.mNick;
	this->mName = user.mName;
	this->mRealname = user.mRealname;
	this->mHost = user.mHost;
	return *this;
}

const SOCKET& User::GetSocket() const {
	return this->mSock;
}

void User::SetNick(const std::string& nick) {
	this->mNick = nick;
}

const std::string& User::GetNick() const {
	return this->mNick;
}

void User::SetName(const std::string& name) {
	this->mName = name;
}

const std::string& User::GetName() const {
	return this->mName;
}

void User::SetRealname(const std::string& realname) {
	this->mRealname = realname;
}

const std::string& User::GetRealname() const {
	return this->mRealname;
}

void User::SetHost(const std::string& host) {
	this->mHost = host;
}

const std::string& User::GetHost() const {
	return this->mHost;
}

void User::SetPass() {
	this->mbPass = true;
}

const bool& User::GetPass() const {
	return this->mbPass;
}

void User::SetConnect() {
	this->mbConnect = true;
}

const std::vector<std::string>& User::GetChannel() const {
	return this->mChannel;
}

void User::AddChannel(std::string ch) {
	mChannel.push_back(ch);
}

const bool User::FindChannel(std::string ch) const {
	for (size_t i = 0; i < this->mChannel.size(); i++) {
		if (this->mChannel[i] == ch) {
			return true;
		}
	}
	return false;
}

void User::EraseChannel(std::string ch) {
	std::vector<std::string>::iterator it;

	for (it = this->mChannel.begin(); it != this->mChannel.end(); it++) {
		if (*it == ch) {
			this->mChannel.erase(it);
			return;
		}
	}
}

const std::string User::GetMask() const {
	return this->mNick + "!" + this->mName + "@" + this->mHost;
}

bool User::IsLogin() {
	if (this->mNick != "" && this->mName != "" && this->mRealname != "" && this->mbPass)
		return true;
	return false;
}

bool User::IsConnect() {
	return mbConnect;
}

void User::Login(std::string servName, std::string date, std::string ver) {
	this->SetConnect();
	SendMsg(":" + servName + " " + RPL_WELCOME + " " + this->mNick + " :Welcome to the Internet Relay Network " + this->GetMask(), this->mSock);
	SendMsg(":" + servName + " " + RPL_YOURHOST + " " + this->mNick + " :Your host is " + servName + ", running version " + ver, this->mSock);
	SendMsg(":" + servName + " " + RPL_CREATED + " " + this->mNick + " :This server was created " + date, this->mSock);
	SendMsg(":" + servName + " " + RPL_MYINFO + " " + this->mNick + " :" + servName + " " + ver + " 0 0", this->mSock);
}