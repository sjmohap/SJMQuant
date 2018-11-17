#include <SJMMsgUtils/SJMMsgUtils_precompiled.h>
#include <SJMMsgUtils/SJMZmQ.h>


using  std::string;


SJMZmQ::SJMZmQ(MSGQ_PROTOCOL protocol, string port, bool binding)
	: IMsgQ(protocol, port) {
#ifdef _DEBUG
	std::printf("zmq protocol: %d  port %s  binding: %d\n", protocol, port.c_str(), binding);
#endif
	if (m_protocol == MSGQ_PROTOCOL::PAIR)
	{
		m_context = zmq_ctx_new();
		m_socket = zmq_socket(m_context, ZMQ_PAIR);

		if (binding) {
			m_endpoint = "tcp://*:" + port;
			m_rc = zmq_bind(m_socket, m_endpoint.c_str());
		}
		else {
			m_endpoint = "tcp://localhost:" + port;
			m_rc = zmq_connect(m_socket, m_endpoint.c_str());
		}
	}
	else if (m_protocol == MSGQ_PROTOCOL::PUB) {
		m_context = zmq_ctx_new();
		m_socket = zmq_socket(m_context, ZMQ_PUB);

		m_endpoint = "tcp://*:" + port;
		m_rc = zmq_bind(m_socket, m_endpoint.c_str());
	}
	else if (m_protocol == MSGQ_PROTOCOL::SUB) {
		m_context = zmq_ctx_new();
		m_socket = zmq_socket(m_context, ZMQ_SUB);

		m_endpoint = "tcp://localhost:" + port;
		m_rc = zmq_connect(m_socket, m_endpoint.c_str());
	}
}

SJMZmQ::~SJMZmQ()
{
	m_rc = zmq_unbind(m_socket, m_endpoint.c_str());
	zmq_close(m_socket);
	zmq_ctx_shutdown(m_context);
	zmq_term(m_context);
	zmq_ctx_destroy(m_context);
}

// zmq doesn't have global nn_term(), has to be set non-blocking, ZMQ_DONTWAIT
void SJMZmQ::sendmsg(const string& msg) {
	int bytes = zmq_send(m_socket, msg.c_str(), msg.size() + 1, 0);		// TODO: size or size+1
}

void SJMZmQ::sendmsg(const char* msg) {
	int bytes = zmq_send(m_socket, msg, strlen(msg) + 1, 0);
}

string SJMZmQ::recmsg(int blockingflags) {
	int bytes = zmq_recv(m_socket, m_buf, 1024, blockingflags);		//ZMQ_NOBLOCK

	if (bytes > 0) {
		m_buf[bytes] = '\0';
		string msg(m_buf);
		return msg;
	}
	else {
		return string();
	}
}



