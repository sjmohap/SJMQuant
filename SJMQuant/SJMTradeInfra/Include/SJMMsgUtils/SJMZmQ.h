#pragma once
#include "IMsgQ.h"
class DLL_SJMMsgUtils SJMZmQ :
	public IMsgQ
{
public:
	SJMZmQ(MSGQ_PROTOCOL protocol, std::string port, bool binding = true);
	~SJMZmQ();

	void sendmsg(const std::string& str) override;
	void sendmsg(const char* str) override;
	std::string recmsg(int blockingflags = 1) override;

private:
	void* m_context;
	void* m_socket;
	std::string m_endpoint;
	int m_rc;
	char m_buf[256];
};

