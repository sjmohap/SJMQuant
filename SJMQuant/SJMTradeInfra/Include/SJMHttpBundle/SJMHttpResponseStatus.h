#pragma once
#include <string>
#include <SJMHttpBundle/SJMHttpBundleCommonDef.h>

class DLL_SJMHTTPBUNDLE SJMHttpResponseStatus
{
public:

	SJMHttpResponseStatus(unsigned response_status, const std::wstring& cs);
	SJMHttpResponseStatus() = default;

	bool wasSuccess() const;
	unsigned getStatusCode() const;
	std::wstring getResponseMsg() const;
private:
	unsigned int _responseStatus = 0;
	std::wstring _responsMsg;
};
