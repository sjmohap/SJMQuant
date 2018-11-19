#include "SJMHttpBundle/SJMDownloadTask.h"
#include <mutex>

SJMDownloadTaskController::SJMDownloadTaskController(QStringList& fileUrlList, const QString& folderAbsPath = "") : noOfFilesToBeDownloaded(fileUrlList.size())
{
	pool = new QThreadPool();
	pool->setMaxThreadCount(noOfFilesToBeDownloaded);
	downloadFilesAndGetFileNames(fileUrlList, folderAbsPath);
}

QStringList SJMDownloadTaskController::downloadFilesAndGetFileNames(const QStringList& fileUrlList, const QString& saveFileDir)
{
	QVector< SJMDownloadTask*> taskVector{};
	for(auto url : fileUrlList )
	{
		QString savedir = saveFileDir;
		QUrl dwUrl = QUrl::fromEncoded(url.toLocal8Bit());
		QNetworkRequest request(dwUrl);
		taskVector.push_back(new SJMDownloadTask(request, savedir));
	}

	for(auto &task : taskVector)
	{
		connect(task, SIGNAL(onFileDownloaded(QString&)), SLOT(onFileDownloaded(QString&)));
		pool->start(task);
	}

	QEventLoop loop;
	connect(this, SIGNAL(allFilesDownloaded()), &loop, SLOT(quit()));
	loop.exec();

	return mDownloadedFileList;
}

void SJMDownloadTaskController::onFileDownloaded(QString& savedFileAbsPath)
{
	mDownloadedFileList.push_back(savedFileAbsPath);
	if (mDownloadedFileList.size() == noOfFilesToBeDownloaded)
	{
		emit allFilesDownloaded();
	}
}

SJMDownloadTask::SJMDownloadTask(const QNetworkRequest request, QString& saveFileDir) :
	mRequest(request),
	saveFolderLocation(saveFileDir)
{
	
}

void SJMDownloadTask::run()
{
	beginDownload(mRequest);
	QEventLoop loop;
	connect(this, SIGNAL(onFileDownloaded(QString&)), &loop, SLOT(quit()));
	loop.exec();
}

void SJMDownloadTask::beginDownload(const QNetworkRequest& request)
{
	QNetworkAccessManager *mNwManager = new QNetworkAccessManager();
	connect(mNwManager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
	fprintf(stdout, "Download started URL: %s\n", request.url().toEncoded().constData());	
	QNetworkReply *reply = mNwManager->get(request);

#if QT_CONFIG(ssl)
	connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
		SLOT(sslErrors(QList<QSslError>)));
#endif

}

QString SJMDownloadTask::saveFileName(const QUrl& url) const
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();
	if (basename.isEmpty())
	{
		basename = "download";
	}
	QString absolutePath;
	if(saveFolderLocation != "")
	{
		QDir directory(saveFolderLocation);
		absolutePath = directory.absoluteFilePath(basename);
	}
	else
	{
		absolutePath = basename;
	}

	if (QFile::exists(absolutePath)) {
		int i = 0;
		absolutePath += '.';
		while (QFile::exists(absolutePath + QString::number(i)))
		{
			++i;
		}
		absolutePath += QString::number(i);
	}
	return absolutePath;
}

bool SJMDownloadTask::saveToDisk(const QString& filename, QIODevice* data) const
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		fprintf(stderr, "Could not open %s for writing: %s\n",
			qPrintable(filename),
			qPrintable(file.errorString()));
		return false;
	}

	file.write(data->readAll());
	file.close();
	return true;
}

bool SJMDownloadTask::isHttpRedirect(QNetworkReply* reply) const
{
	const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	return statusCode == 301 || statusCode == 302 || statusCode == 303
		|| statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void SJMDownloadTask::downloadFinished(QNetworkReply* reply)
{
	QUrl url = reply->url();
	QString filename = "";
	if (reply->error()) 
	{
		fprintf(stderr, "Download of %s failed: %s\n",
			url.toEncoded().constData(),
			qPrintable(reply->errorString()));
	}
	else 
	{
		if (isHttpRedirect(reply)) 
		{
			fputs("Request was redirected.\n", stderr);
		}
		else 
		{
			std::lock_guard<std::mutex> lck(dwldMutex);
			filename = saveFileName(url);
			if (saveToDisk(filename, reply)) 
			{
				printf("Download of %s succeeded (saved to %s)\n",
					url.toEncoded().constData(), qPrintable(filename));
			}
		}
	}
	emit onFileDownloaded(filename);
	reply->deleteLater();
}

void SJMDownloadTask::sslErrors(const QList<QSslError>& errors) const
{
#if QT_CONFIG(ssl)
	for (const QSslError &error : errors)
		fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
	Q_UNUSED(sslErrors);
#endif
}
