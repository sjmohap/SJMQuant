#include <QtCore/QCoreApplication>
#include "SJMHttpBundle/SJMDownloadManager.h"
#include <QtNetwork/QNetworkAccessManager>
#include "SJMHttpBundle/SJMCommandlineTextProgressBar.h"
#include "SJMHttpBundle/SJMDownloadTask.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	//QNetworkAccessManager networkAccessMgr;
	//SJMCommandlineTextProgressBar progressBar;
	//SJMDownloadManager obj(networkAccessMgr, progressBar);

	//auto str = obj.downloadAndGetFilePath("http://doc.qt.io/qt-5/threads-reentrancy.html");
	////qDebug()<< "the name of the file downloaded is " << str << '\n'; //"http://mirror2.internetdownloadmanager.com/idman632build1.exe?b=1&filename=idman632build1.exe"
	//obj.appendDownloadLinks({ "http://doc.qt.io/qt-5/threads-reentrancy.html",
	//"http://doc.qt.io/qt-5/threads-reentrancy.html",
	//"http://doc.qt.io/qt-5/threads-reentrancy.html", 
	//"http://doc.qt.io/qt-5/threads-reentrancy.html"});

	QStringList downloadFileList{ "http://doc.qt.io/qt-5/threads-reentrancy.html",
	"http://doc.qt.io/qt-5/threads-reentrancy.html"
	};
	
	SJMDownloadTaskController ks( downloadFileList , "D:\\work\\trading\\SJMQuant");
	return a.exec();
}
