#include "Server.h"

Server::Server() {
	WSADATA wsaData;
	HANDLE comPort;

	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET servSock;
	SOCKADDR_IN servAdr;
	DWORD recvBytes, flags = 0;
	int i;

	std::vector<std::thread> vThread;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		this->ErrorHandling("WSAStartup() error");

	comPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	for (i = 0; i < MAX_THREAD; i++)
		vThread.push_back(std::thread(&Server::ThreadMain, this, comPort));

	servSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = PF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(PORT);

	if (bind(servSock, reinterpret_cast<SOCKADDR*>(&servAdr), sizeof(servAdr)) == SOCKET_ERROR)
		this->ErrorHandling("bind() error");

	if (listen(servSock, 10))
		this->ErrorHandling("listen() error");

	while (1) {
		SOCKET clntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		clntSock = accept(servSock, reinterpret_cast<SOCKADDR*>(&clntAdr), &addrLen);
		if (clntSock == INVALID_SOCKET)
			this->ErrorHandling("accept() error");
		handleInfo = new PER_HANDLE_DATA;
		handleInfo->hClntSock = clntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		if (clntSock != 0) {
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(clntSock), comPort, reinterpret_cast<DWORD>(handleInfo), 0);
			this->mData.GetMutex().lock();
			this->mData.GetUser()[clntSock] = User(clntSock);
			this->mData.GetUser()[clntSock].SetHost("host");
			this->mData.GetMutex().unlock();
		}

		ioInfo = new PER_IO_DATA;
		memset(ioInfo, 0, sizeof(PER_IO_DATA));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);
	}
}

Server::~Server() {

}

void Server::ErrorHandling(std::string msg) {
	std::cerr << msg << std::endl;
	exit(1);
}

void Server::ThreadMain(HANDLE pComPort) {
	HANDLE hComPort = reinterpret_cast<HANDLE>(pComPort);
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;
	std::string msg, sendMsg;

	Command command;

	size_t beforeFound, afterFound;
	while (1) {
		GetQueuedCompletionStatus(hComPort, &bytesTrans, reinterpret_cast<LPDWORD>(&handleInfo), reinterpret_cast<LPOVERLAPPED*>(&ioInfo), INFINITE);
		sock = handleInfo->hClntSock;
		if (ioInfo->rwMode == READ) {
			if (bytesTrans == 0) {
				this->mData.GetMutex().lock();
				this->mData.AddUserHistory(this->mData.GetUser()[sock]);
				std::vector<std::string> ch = this->mData.GetUser()[sock].GetChannel();

				for (size_t i = 0; i < ch.size(); i++) {
					this->mData.GetChannel()[ch[i]].EraseUser(&this->mData.GetUser()[sock]);
					if (this->mData.GetChannel()[ch[i]].GetUser().empty())
						this->mData.GetChannel().erase(ch[i]);
				}
				this->mData.GetUser().erase(sock);
				this->mData.GetMutex().unlock();
				closesocket(sock);
				delete handleInfo;
				delete ioInfo;
				continue;
			}
			// irc ¸í·É¾î
			msg = ioInfo->wsaBuf.buf;
			delete ioInfo;
			afterFound = msg.find("\r\n");
			beforeFound = 0;
			while (afterFound != std::string::npos) {
				command.Run(msg.substr(beforeFound, afterFound - beforeFound), sock);
				beforeFound = afterFound + 2;
				afterFound = msg.find("\r\n", beforeFound);
			}

			ioInfo = new PER_IO_DATA;
			memset(ioInfo, 0, sizeof(PER_IO_DATA));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
	}
}