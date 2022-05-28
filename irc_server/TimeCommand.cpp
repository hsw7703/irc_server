#include "TimeCommand.h"

TimeCommand::TimeCommand() {

}

TimeCommand::~TimeCommand() {

}

void TimeCommand::Run(const SOCKET& sock) {
	SendMsg(":" + this->mData.GetServName() + " " + RPL_TIME + " " + this->mData.GetUser()[sock].GetNick() + " " + this->mData.GetServName() + " :" + DateToString(), sock);
}