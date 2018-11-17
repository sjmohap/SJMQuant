#include <SJMHttpBundle/SJMHttpResponseStatus.h>

SJMHttpResponseStatus::SJMHttpResponseStatus(unsigned response_status, const std::wstring& cs):
	_responseStatus(response_status),
	_responsMsg(cs)
{
}

bool SJMHttpResponseStatus::wasSuccess() const
{
	return (/*HTTP_STATUS_OK*/200 == _responseStatus || /*HTTP_STATUS_CREATED*/ 201 == _responseStatus);
}

unsigned SJMHttpResponseStatus::getStatusCode() const
{
	return _responseStatus;
}

std::wstring SJMHttpResponseStatus::getResponseMsg() const
{
	return _responsMsg;
}
