#pragma once

#include "ISJMDownloadManager.h"
#include "ISJMProgressBar.h"
#include "SJMHttpBundleCommonDef.h"

class DLL_SJMHTTPBUNDLE SJMDownloadManager : public QObject ,public ISJMDownloadManager
{
	Q_OBJECT
public:
	SJMDownloadManager(QNetworkAccessManager& manager, ISJMProgressBar& progress_bar);
	~SJMDownloadManager() {};

	static QUrl getUrlFromString(const QString& url);
	bool doesFileExist(const QString& fileURL) override;
	QString getSaveFileName(const QString& url) const override;

	QString downloadAndGetFilePath(const QString& fileURL) override;
	void downloadFilesToFolder(const QStringList& urls, const QString& folderPath) override;	
	void appendDownloadLinks(const QStringList& urls) override;

private:
	void appendDownloadLink(const QString& urls);
	QString getSaveFileName(const QUrl& url) const;		
	void downloadFile(const QUrl& url, const QString& saveFileName);

public:
	struct FileDownloadTask : QRunnable
	{
		virtual void run() override;
	};

signals:
	void finished();
	void fileWritten();

private slots:
	void startNextDownload();
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal) const;
	void downloadFinished();
	void downloadReadyRead();

private:
	bool isHttpRedirect() const;
	void reportRedirect() const;

	QNetworkAccessManager& manager;
	QQueue<QUrl> downloadQueue;
	QNetworkReply *currentDownload = nullptr;
	QFile output;
	QTime downloadTime;
	ISJMProgressBar& progressBar;

	int downloadedCount = 0;
	int totalCount = 0;
	QString lastSavedFileName;
};
