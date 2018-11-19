// sampleTesterApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "SJMHttpBundle/SJMJHttpRestCommunication.h"
#include "SJMHttpBundle/SJMDownloadManager.h"
#include <QtNetwork/QNetworkAccessManager>
#include "SJMHttpBundle/SJMCommandlineTextProgressBar.h"

/* Driver program to test above functions*/
int main()
{
	QNetworkAccessManager networkAccessMgr;
	SJMCommandlineTextProgressBar progressBar;
	SJMDownloadManager obj(networkAccessMgr, progressBar);

	//auto str = obj.downloadAndGetFilePath("https://www.scootersoftware.com/BCompare-4.2.8.23479.exe");
	obj.appendDownloadLinks({ "https://www.scootersoftware.com/BCompare-4.2.8.23479.exe" });
	return 0;
}