#include "stdafx.h"
#include "CppUnitTest.h"
#include "SJMHttpBundle/SJMJHttpRestCommunication.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SJMHttpBundle_uTest
{		
	TEST_CLASS(SJMJHttpRestCommunication_uTest)
	{
	public:
		
		TEST_METHOD(downloadingFile)
		{
			// TODO: Your test code here
			//ISJMResponseHandlerPtr fire_web_request(std::wstring uri_string) override;
			//void downloadFile(const std::wstring& fileURI, const std::wstring& localStorageLocation, bool ignoreCertificateErr = false);
			SJMJHttpRestCommunication restCommunicator;
			restCommunicator.downloadFile(L"https://www.nseindia.com/content/nsccl/nsccl_ann19.zip", L"mydownloadedfile.zip");
		}

	};
}