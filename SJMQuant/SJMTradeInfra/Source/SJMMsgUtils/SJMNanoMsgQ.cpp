#include <SJMMsgUtils/SJMMsgUtils_precompiled.h>
#include <SJMMsgUtils/SJMNanoMsgQ.h>

using  std::string;

SJMNanoMsgQ::SJMNanoMsgQ(MSGQ_PROTOCOL protocol, std::string port, bool binding) : IMsgQ(protocol, port)
{
#ifdef _DEBUG
	std::printf("nanomsg protocol: %d  port %s  binding: %d\n", protocol, port.c_str(), binding);
#endif
	if (m_protocol == MSGQ_PROTOCOL::PAIR)
	{
		m_socket = nn_socket(AF_SP, NN_PAIR);
		assert(m_socket >= 0);
		int to = 100;
		assert(nn_setsockopt(m_socket, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof(to)) >= 0);

		if (binding) {
			m_endpoint = "tcp://*:" + m_port;
			m_eid = nn_bind(m_socket, m_endpoint.c_str());
		}
		else {
			m_endpoint = "tcp://localhost:" + m_port;
			m_eid = nn_connect(m_socket, m_endpoint.c_str());
		}
	}
	else if (m_protocol == MSGQ_PROTOCOL::PUB) {
		m_socket = nn_socket(AF_SP, NN_PUB);
		assert(m_socket >= 0);

		m_endpoint = "tcp://*:" + m_port;
		m_eid = nn_bind(m_socket, m_endpoint.c_str());
	}
	else if (m_protocol == MSGQ_PROTOCOL::SUB) {
		m_socket = nn_socket(AF_SP, NN_SUB);
		assert(m_socket >= 0);
		int to = 100;
		assert(nn_setsockopt(m_socket, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof(to)) >= 0);
		nn_setsockopt(m_socket, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);		// subscribe to topic
		m_endpoint = "tcp://localhost:" + m_port;
		m_eid = nn_connect(m_socket, m_endpoint.c_str());
	}

	if (m_eid < 0 || m_socket < 0) {
		LOG_PRINT("ERROR:[%s,%d][%s]Unable to connect to message queue: %s,%d\n", __FILE__, __LINE__, __FUNCTION__, port.c_str(), binding);
	}
}

SJMNanoMsgQ::~SJMNanoMsgQ()
{
	nn_shutdown(m_socket, m_eid);
	nn_close(m_socket);
}

void SJMNanoMsgQ::sendmsg(const string& msg) {
	// if (!msg.empty()) 
	int bytes = nn_send(m_socket, msg.c_str(), msg.size() + 1, 0);			// TODO: size or size+1

																		/*if (bytes != msg.size()){
																		PRINT_TO_FILE("INFO:[%s,%d][%s]NANOMSG ERROR, %s\n", __FILE__, __LINE__, __FUNCTION__, msg.c_str());
																		}*/
}

void SJMNanoMsgQ::sendmsg(const char* msg) {
	int bytes = nn_send(m_socket, msg, strlen(msg) + 1, 0);
}

string SJMNanoMsgQ::recmsg(int blockingflags) {
	char* buf = nullptr;
	int bytes = nn_recv(m_socket, &buf, NN_MSG, blockingflags);		//NN_DONTWAIT

	if (bytes > 0 && buf != nullptr) {
		string msg(buf, bytes);
		buf != nullptr && nn_freemsg(buf);
		return msg;
	}
	else {
		return string();
	}
}
