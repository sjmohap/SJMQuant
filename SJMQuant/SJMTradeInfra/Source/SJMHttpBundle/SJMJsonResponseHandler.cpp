#include "SJMHttpBundle/SJMJsonResponseHandler.h"
#include <QString>
const QString SP_ROOT = "sp_Root";

SJMJsonResponseHandler::SJMJsonResponseHandler(const web::http::http_response & response) :
	_httpResponse(response),
	_responseStatus(response.status_code(), response.reason_phrase())
{

}

bool SJMJsonResponseHandler::parserResponse(const ISJMResponseContentHandlerPtr p_c_content_handler)
{
	if(mObjResponse.is_null())
	{
		try
		{
			mObjResponse = _httpResponse.extract_json().get();
		}
		catch(std::exception& ex)
		{
			return false;
		}
		catch(...)
		{
			return false;
		}

		_responseContentHandler = p_c_content_handler;
		if(mObjResponse.is_array())
		{
			return handleArray(SP_ROOT, mObjResponse.as_array());
		}
		else if(mObjResponse.is_object())
		{
			return handleObject(SP_ROOT, mObjResponse.as_object());
		}
		else if(mObjResponse.is_string())
		{
			QString value = QString::fromStdWString(mObjResponse.as_string().c_str());
			return handleString(SP_ROOT, value);
		}
		
		
	}return false;
}

std::string SJMJsonResponseHandler::serializeResponse()
{
	if(mObjResponse.is_null())
	{
		try
		{
			mObjResponse = _httpResponse.extract_json().get();
		}
		catch(std::exception&)
		{
			
		}
		catch(...)
		{
			
		}
	}
	utility::string_t strResponse = mObjResponse.serialize();
	return QString::fromStdWString(strResponse.c_str()).toStdString();
}

QJsonValue SJMJsonResponseHandler::get_json_v_value()
{
	return QJsonValue();
}

SJMHttpResponseStatus SJMJsonResponseHandler::getResponseStatus()
{
	return SJMHttpResponseStatus();
}

bool SJMJsonResponseHandler::handleObject(const QString& key, web::json::object& jsonObj)
{
	return false;
}

bool SJMJsonResponseHandler::handleArray(const QString & key, web::json::array & jsonObj)
{
	return false;
}

bool SJMJsonResponseHandler::handleString(const QString & key, QString& jsonObj)
{
	return false;
}
