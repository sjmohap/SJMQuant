#pragma once
#include <SJMHttpBundle/SJMHttpResponseStatus.h>
#include <SJMHttpBundle/ISJMResponseContentHandler.h>
#include <QJsonValue>

class ISJMResponseHandler
{
public:
	virtual ~ISJMResponseHandler() = default;

	virtual bool parserResponse(const ISJMResponseContentHandlerPtr p_c_content_handler) = 0;
	virtual std::string serializeResponse() = 0;
	virtual QJsonValue get_json_v_value() = 0;
	virtual SJMHttpResponseStatus getResponseStatus() = 0;
};

using ISJMResponseHandlerPtr = std::shared_ptr<ISJMResponseHandler>;
