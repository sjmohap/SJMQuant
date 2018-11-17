#pragma once
#include "SJMHttpBundle/ISJMResponseHandler.h"
#include <cpprest/http_msg.h>

class DLL_SJMHTTPBUNDLE SJMJsonResponseHandler : public ISJMResponseHandler
{
public:
	SJMJsonResponseHandler(const web::http::http_response& response);

	bool parserResponse(const ISJMResponseContentHandlerPtr p_c_content_handler) override;
	std::string serializeResponse() override;
	QJsonValue get_json_v_value() override;
	SJMHttpResponseStatus getResponseStatus() override;

private:
	web::http::http_response _httpResponse;
	ISJMResponseContentHandlerPtr _responseContentHandler;
	SJMHttpResponseStatus _responseStatus;
	web::json::value mObjResponse;

private:
	bool handleObject(const QString& key, web::json::object& jsonObj);
	bool handleArray(const QString& key, web::json::array& jsonObj);
	bool handleString(const QString& key, QString& jsonObj);
};
