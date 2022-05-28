#include "Channel.h"

Channel::Channel() {

}

Channel::~Channel() {

}

void Channel::SetAdmin(std::string admin) {
	this->mAdmin = admin;
}

std::string Channel::GetAdmin() {
	return this->mAdmin;
}

std::vector<User*> Channel::GetUser() {
	return this->mUser;
}

void Channel::SetTopic(std::string topic) {
	this->mDate = DateToString();
}

std::string Channel::GetTopic() {
	return this->mTopic;
}

void Channel::AddUser(User* user) {
	this->mUser.push_back(user);
}

void Channel::EraseUser(const User* user) {
	std::vector<User*>::iterator it;

	if (user->GetNick() == this->mAdmin)
		this->mAdmin = "";

	for (it = this->mUser.begin(); it != this->mUser.end(); it++) {
		if (*it == user) {
			this->mUser.erase(it);
			break;
		}
	}
}