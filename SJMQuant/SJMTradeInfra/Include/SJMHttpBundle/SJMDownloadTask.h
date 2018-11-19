#pragma once
#include <qrunnable.h>
#include <QtNetwork>
#include <mutex>
#include "SJMHttpBundleCommonDef.h"

class DLL_SJMHTTPBUNDLE SJMDownloadTaskController : public QObject
{
	Q_OBJECT
public:
	explicit SJMDownloadTaskController(QStringList& fileUrlList, const QString& folderAbsPath);
	virtual ~SJMDownloadTaskController() = default;

private:
	QStringList downloadFilesAndGetFileNames(const QStringList& fileUrlList, const QString& saveFileDir);

public slots:
	void onFileDownloaded(QString& savedFileAbsPath);

	signals:
		void allFilesDownloaded();

private:
	QThreadPool *pool;
	int noOfFilesToBeDownloaded;
	QStringList mDownloadedFileList;
};


class DLL_SJMHTTPBUNDLE SJMDownloadTask : public QObject, public QRunnable
{
	Q_OBJECT
public:
	SJMDownloadTask(const QNetworkRequest request, QString& saveFileDir);
	virtual void run() override;

private:
	void beginDownload(const QNetworkRequest& request);
	QString saveFileName(const QUrl &url) const;
	bool saveToDisk(const QString &filename, QIODevice *data) const;
	bool isHttpRedirect(QNetworkReply *reply) const;

public slots:

	void downloadFinished(QNetworkReply *reply);
	void sslErrors(const QList<QSslError> &errors) const;

	signals:
		void onFileDownloaded(QString& saveFileName);
private:
	const QNetworkRequest mRequest;
	
	//QNetworkReply *reply;
	const QString& saveFolderLocation;
	std::function<QNetworkReply*()> urlHandler;
	std::mutex dwldMutex;	
};

