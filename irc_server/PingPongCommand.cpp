#include "PingPongCommand.h"

PingPongCommand::PingPongCommand() {

}

PingPongCommand::~PingPongCommand() {

}

void PingPongCommand::Run(const std::string& msg, const SOCKET& sock) {
	long long pingTime;

	try {
		pingTime = stoll(msg.substr(msg.find(" ") + 1, msg.size() - msg.find(" ") - 1));
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	SendMsg(":" + this->mData.GetServName() + " PONG " + this->mData.GetServName() + " :" + std::to_string(pingTime), sock);
}