#pragma once
#include "IMsgQ.h"
class DLL_SJMMsgUtils SJMNanoMsgQ :
	public IMsgQ
{
public:
	SJMNanoMsgQ(MSGQ_PROTOCOL protocol, std::string port, bool binding = true);
	~SJMNanoMsgQ();

	void sendmsg(const std::string& str) override;
	void sendmsg(const char* str) override;
	std::string recmsg(int blockingflags = 1) override;

private:
	int m_socket = -1;
	int m_eid = 0;
	std::string m_endpoint;
};

