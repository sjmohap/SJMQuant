#pragma once

#include "ISJMDownloadManager.h"
#include "ISJMProgressBar.h"
#include "SJMHttpBundleCommonDef.h"

class DLL_SJMHTTPBUNDLE SJMDownloadManager : public QObject ,public ISJMDownloadManager/*, public QObject*/
{
	Q_OBJECT
public:
	SJMDownloadManager(QNetworkAccessManager& manager, ISJMProgressBar& progress_bar);
	static QUrl getUrlFromString(const QString& url);

protected:
	~SJMDownloadManager() {};
	bool doesFileExist(const QString& fileURL) override;
	QString getSaveFileName(const QString& url) const override;

	QString downloadAndGetFilePath(const QString& fileURL) override;
	void appendDownloadLinks(const QStringList& urls) override;

private:
	QString getSaveFileName(const QUrl& url) const;	
	void appendDownloadLink(const QString& urls);
	void downloadFile(const QUrl& url);

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
