#pragma once
#include <string>
#include "ISJMHttpRestCommunication.h"
#include <functional>
#include <cpprest/http_msg.h>
#include <cpprest/http_client.h>

using FireWebRequestFun =  std::function <web::http::http_response(web::http::client::http_client & httpClient)>;


class DLL_SJMHTTPBUNDLE SJMJHttpRestCommunication : public ISJMRestCommunication
{
public:
	SJMJHttpRestCommunication();
	ISJMResponseHandlerPtr fire_web_request(std::wstring uri_string) override;
	void downloadFile(const std::wstring& fileURI, const std::wstring& localStorageLocation, bool ignoreCertificateErr = false);
private:

	template <typename IResponseHandler, typename ResponseHandler>
	IResponseHandler triggerRequest(const std::wstring& uri_str, FireWebRequestFun fn, long httpTimeOutInSeconds = 0);

	web::http::http_response fireWebRequestToServer(const std::wstring& uri_string, FireWebRequestFun fn, long httpTimeOutInSeconds = 0);
};

