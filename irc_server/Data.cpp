#include "Data.h"

Data& Data::GetInstance() {
	static Data data;
	return data;
}

std::map<SOCKET, User>& Data::GetUser() {
	return this->mUser;
}
std::map<std::string, Channel>& Data::GetChannel() {
	return this->mChannel;
}

const std::string& Data::GetDate() const {
	return this->mDate;
}

const std::string& Data::GetServName() const {
	return this->mServName;
}

const std::string& Data::GetPassword() const {
	return this->mPassword;
}

const std::string& Data::GetVersion() const {
	return this->mVersion;
}

std::mutex& Data::GetMutex() {
	return this->mMutex;
}

void Data::AddUserHistory(const User& user) {
	this->mUserHistory.push_back(UserHistory(user.GetName(), user.GetRealname(), user.GetHost(), user.GetNick()));
}

std::vector<UserHistory> Data::FindUserHistory(std::string nick) {
	std::vector<UserHistory>::reverse_iterator it;
	std::vector<UserHistory> ret;

	for (it = this->mUserHistory.rbegin(); it != this->mUserHistory.rend(); it++) {
		if (it->GetNick() == nick)
			ret.push_back(*it);
	}
	return ret;
}

User* Data::FindUser(const std::string& nick) {
	std::map<SOCKET, User>::iterator it;

	for (it = this->mUser.begin(); it != this->mUser.end(); it++) {
		if (it->second.GetNick() == nick) {
			return &it->second;
		}
	}
	return NULL;
}

Data::Data() {
	this->mDate = DateToString();
}

Data::~Data() {

}

Data::Data(const Data& data) {
}

Data& Data::operator=(const Data& data) {
	return *this;
}