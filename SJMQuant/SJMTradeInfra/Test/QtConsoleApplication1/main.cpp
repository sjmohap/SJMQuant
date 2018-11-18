#include <QtCore/QCoreApplication>
#include "SJMHttpBundle/SJMDownloadManager.h"
#include <QtNetwork/QNetworkAccessManager>
#include "SJMHttpBundle/SJMCommandlineTextProgressBar.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QNetworkAccessManager networkAccessMgr;
	SJMCommandlineTextProgressBar progressBar;
	SJMDownloadManager obj(networkAccessMgr, progressBar);

	auto str = obj.downloadAndGetFilePath("http://doc.qt.io/qt-5/threads-reentrancy.html");
	//qDebug()<< "the name of the file downloaded is " << str << '\n'; //"https://www.scootersoftware.com/BCompare-4.2.8.23479.exe"
	obj.appendDownloadLinks({ "http://doc.qt.io/qt-5/threads-reentrancy.html",
	"http://doc.qt.io/qt-5/threads-reentrancy.html",
	"http://doc.qt.io/qt-5/threads-reentrancy.html", 
	"http://doc.qt.io/qt-5/threads-reentrancy.html"});

	return a.exec();
}
