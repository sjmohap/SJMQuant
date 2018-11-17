#pragma once
#include "ISJMResponseHandler.h"

class ISJMRestCommunication
{
public:
	virtual ~ISJMRestCommunication() = default;
	virtual ISJMResponseHandlerPtr fire_web_request(std::wstring uri_string) = 0;
};

