#include "Utils.h"

void SendMsg(std::string msg, SOCKET sock) {
	PER_IO_DATA ioInfo;

	memset(&ioInfo, 0, sizeof(ioInfo));

	msg.append("\r\n");
	memcpy(ioInfo.buffer, msg.c_str(), msg.size());

	std::cout << sock << " send msg : " << ioInfo.buffer << std::endl;

	ioInfo.wsaBuf.buf = ioInfo.buffer;
	ioInfo.wsaBuf.len = msg.size();
	ioInfo.rwMode = WRITE;
	WSASend(sock, &ioInfo.wsaBuf, 1, NULL, 0, &(ioInfo.overlapped), NULL);
}

bool IsSpecial(char c) {
	if ((c >= 91 && c <= 96) || (c >= 123 && c <= 125))
		return true;
	return false;
}

std::vector<std::string> ChannelParser(const std::string &msg) {
	size_t beforeFound, afterFound;
	std::string channel;
	std::vector<std::string> channelList;

	beforeFound = msg.find(" ") + 1;
	afterFound = msg.find(" ", beforeFound);

	channel = msg.substr(beforeFound, afterFound - beforeFound);
	beforeFound = 0;
	while (1) {
		afterFound = channel.find(",", beforeFound);
		if (afterFound == std::string::npos) {
			channelList.push_back(channel.substr(beforeFound, channel.size()));
			break;
		}
		channelList.push_back(channel.substr(beforeFound, afterFound - beforeFound));
		beforeFound = afterFound + 1;
	}
	return channelList;
}

std::string AllToUpper(const std::string msg) {
	std::string ret = "";

	for (size_t i = 0; i < msg.size(); i++) {
		ret += toupper(msg[i]);
	}
	return ret;
}

std::string DateToString() {
	struct tm tm;
	time_t rawTime;
	std::string date;

	time(&rawTime);
	localtime_s(&tm, &rawTime);
	date = std::to_string(tm.tm_year + 1900) + "." +
		std::to_string(tm.tm_mon + 1) + "." +
		std::to_string(tm.tm_mday) + " " +
		std::to_string(tm.tm_hour) + ":" +
		std::to_string(tm.tm_min) + ":" +
		std::to_string(tm.tm_sec);
	return date;
}