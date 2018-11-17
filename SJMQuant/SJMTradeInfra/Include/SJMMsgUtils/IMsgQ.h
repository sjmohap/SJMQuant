#pragma once
#include "SJMMsgUtils_CommonDef.h"

enum class MSGQ : uint8_t {
	NANOMSG = 0, ZMQ, WEBSOCKET
};

enum class MSGQ_PROTOCOL : uint8_t {
	PAIR = 0, REQ, REP, PUB, SUB, PIPELINE
};


class IMsgQ {
protected:
	MSGQ_PROTOCOL m_protocol;
	std::string m_port;
public:
	IMsgQ(MSGQ_PROTOCOL protocol, std::string port) : m_protocol(protocol), m_port(port) {}
	virtual void sendmsg(const std::string& str) = 0;
	virtual void sendmsg(const char* str) = 0;
	virtual std::string recmsg(int blockingflags = 1) = 0;
};