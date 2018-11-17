#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include "SJMHttpBundle/ISJMResponseHandler.h"
#include "SJMHttpBundle/SJMJHttpRestCommunication.h"
#include "SJMHttpBundle/SJMJsonResponseHandler.h"
#include "..\..\Include\SJMHttpBundle\SJMJHttpRestCommunication.h"


SJMJHttpRestCommunication::SJMJHttpRestCommunication()
{
}

ISJMResponseHandlerPtr SJMJHttpRestCommunication::fire_web_request(std::wstring uri_string)
{
	return triggerRequest<ISJMResponseHandlerPtr, SJMJsonResponseHandler >(uri_string, [](web::http::client::http_client& httpClient)
	{
		return httpClient.request(web::http::methods::GET).get();
	});
}

void SJMJHttpRestCommunication::downloadFile(const std::wstring & fileURI, const std::wstring & localStorageLocation, bool ignoreCertificateErr)
{
	auto fileBuffer = std::make_shared < Concurrency::streams::streambuf<uint8_t>>();
	utility::string_t filePath(localStorageLocation);

	try
	{
		web::uri endPointURI(fileURI);
		web::http::client::http_client_config config;
		const web::web_proxy wpDisable(web::web_proxy::web_proxy_mode::disabled);

		config.set_proxy(wpDisable);
		if(ignoreCertificateErr)
		{
			config.set_validate_certificates(false);
		}

		web::http::client::http_client httpClient(fileURI, config);
		try
		{
			auto httpRequest = httpClient.request(web::http::methods::GET);
			httpRequest.then([&](web::http::http_response response)
			{
				if(response.status_code() == web::http::status_codes::OK)
				{
					auto fileBufferStream = concurrency::streams::file_buffer<uint8_t>::open(filePath, std::ios::out);
					concurrency::streams::streambuf<uint8_t> outFile = fileBufferStream.get();
					*fileBuffer = outFile;

					auto readResponse = response.body().read_to_end(*fileBuffer);
					size_t dataRead = readResponse.get();

					fileBuffer->close();
				}
				else
				{
					
				}
			}).wait();
		}
		catch(web::http::http_exception&)
		{
			
		}
	}
	catch(std::invalid_argument&)
	{
		
	}

	catch(const std::system_error& e)
	{
		
	}
}


template <typename IResponseHandler, typename ResponseHandler>
IResponseHandler SJMJHttpRestCommunication::triggerRequest(const std::wstring& uri_str, FireWebRequestFun fn,
	long httpTimeOutInSeconds)
{
	web::http::http_response response = fireWebRequestToServer(uri_str, fn, httpTimeOutInSeconds);
	IResponseHandler responseHandler = std::make_shared<ResponseHandler>(response);

	return responseHandler;
}


web::http::http_response SJMJHttpRestCommunication::fireWebRequestToServer(const std::wstring & uri_string, FireWebRequestFun fn, long httpTimeOutInSeconds)
{
	web::http::http_response response(web::http::status_codes::BadRequest);
	try
	{
		const web::uri endpointUri(uri_string);
		web::http::client::http_client_config config;
		config.set_timeout(utility::seconds(httpTimeOutInSeconds));

		web::http::client::http_client httpClient(endpointUri, config);
		response = fn(httpClient);
	}
	catch (web::http::http_exception& http_e_exception)
	{

	}

	catch (std::invalid_argument& http_e_exception)
	{

	}

	catch (std::system_error& http_e_exception)
	{

	}

	return response;
}
