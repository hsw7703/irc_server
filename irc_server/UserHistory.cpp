#include "UserHistory.h"

UserHistory::UserHistory() {

}

UserHistory::UserHistory(const std::string& name, const std::string& realname, const std::string& host, const std::string& nick)
	: mName(name), mRealname(realname), mHost(host), mNick(nick){
	this->mDate = DateToString();
}


UserHistory::~UserHistory() {
	
}

const std::string& UserHistory::GetName() const {
	return this->mName;
}

const std::string& UserHistory::GetRealname() const {
	return this->mRealname;
}

const std::string& UserHistory::GetHost() const {
	return this->mHost;
}

const std::string& UserHistory::GetNick() const {
	return this->mNick;
}

const std::string& UserHistory::GetDate() const {
	return this->mDate;
}