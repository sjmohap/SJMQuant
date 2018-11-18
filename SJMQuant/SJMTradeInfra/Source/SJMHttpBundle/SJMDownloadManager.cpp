#pragma once

#include <QtNetwork>
#include <QtCore>
#include <QObject>
#include "SJMHttpBundle/SJMDownloadManager.h"

SJMDownloadManager::SJMDownloadManager(QNetworkAccessManager& manager, ISJMProgressBar& progress_bar)
: manager(manager),
progressBar(progress_bar)
{
}

bool SJMDownloadManager::doesFileExist(const QString& fileURL)
{
	return false;
}

QString SJMDownloadManager::getSaveFileName(const QString& url) const
{
	QString basename = QFileInfo(url).fileName();

	if (basename.isEmpty())
		basename = "download";

	if (QFile::exists(basename)) {
		// already exists, don't overwrite
		int i = 0;
		basename += '.';
		while (QFile::exists(basename + QString::number(i)))
			++i;

		basename += QString::number(i);
	}
	return basename;
}

// not thread safe
QString SJMDownloadManager::downloadAndGetFilePath(const QString& fileURL)
{
	const auto url = getUrlFromString(fileURL);
	++totalCount;
	downloadFile(url);
	
	QEventLoop loop;
	connect(this, SIGNAL(fileWritten()), &loop, SLOT(quit()));
	loop.exec();
	return lastSavedFileName;
}

QString SJMDownloadManager::getSaveFileName(const QUrl& url) const
{
	const auto path = url.path();
	return getSaveFileName(path);
}

QUrl SJMDownloadManager::getUrlFromString(const QString& url)
{
	return QUrl::fromEncoded(url.toLocal8Bit());
}

void SJMDownloadManager::downloadFilesToFolder(const QStringList & urls, const QString & folderPath)
{
}

void SJMDownloadManager::appendDownloadLinks(const QStringList& urls)
{
	for (const QString &urlAsString : urls)
		appendDownloadLink(urlAsString);

	if (downloadQueue.isEmpty())
		QTimer::singleShot(0, this, SIGNAL(finished()));
}

void SJMDownloadManager::appendDownloadLink(const QString& urlStr)
{
	const QUrl downloadURL = getUrlFromString(urlStr);

	if (downloadQueue.isEmpty())
	{
		QTimer::singleShot(0, this, SLOT(startNextDownload()));
	}

	downloadQueue.enqueue(downloadURL);
	++totalCount;
}

void SJMDownloadManager::downloadFile(const QUrl& url)
{
	QString saveFileName = getSaveFileName(url);
	output.setFileName(saveFileName);

	if (!output.open(QIODevice::WriteOnly)) {
		fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n",
		        qPrintable(saveFileName), url.toEncoded().constData(),
		        qPrintable(output.errorString()));

		startNextDownload();
		return;                 // skip this download
	}

	const QNetworkRequest request(url);
	currentDownload = manager.get(request);

	connect(currentDownload, SIGNAL(downloadProgress(qint64, qint64)),
	                 SLOT(downloadProgress(qint64, qint64)));
	connect(currentDownload, SIGNAL(finished()),
	                 SLOT(downloadFinished()));
	connect(currentDownload, SIGNAL(readyRead()),
	                 SLOT(downloadReadyRead()));

	// prepare the output
	printf("Downloading %s...\n", url.toEncoded().constData());
	downloadTime.start();
}

void SJMDownloadManager::startNextDownload()
{
	if (downloadQueue.isEmpty()) {
		printf("%d/%d files downloaded successfully\n", downloadedCount, totalCount);
		emit finished();
		return;
	}

	QUrl url = downloadQueue.dequeue();
	downloadFile(url);
}

void SJMDownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) const
{
	progressBar.setStatus(bytesReceived, bytesTotal);

	// calculate the download speed
	double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
	QString unit;
	if (speed < 1024) {
		unit = "bytes/sec";
	}
	else if (speed < 1024 * 1024) {
		speed /= 1024;
		unit = "kB/s";
	}
	else {
		speed /= 1024 * 1024;
		unit = "MB/s";
	}

	progressBar.setMessage(QString::fromLatin1("%1 %2")
		.arg(speed, 3, 'f', 1).arg(unit));
	progressBar.update();
}

void SJMDownloadManager::downloadFinished()
{
	progressBar.clear();
	lastSavedFileName = output.fileName();
	emit fileWritten();
	output.close();

	if (currentDownload->error()) {
		// download failed
		fprintf(stderr, "Failed: %s\n", qPrintable(currentDownload->errorString()));
		output.remove();
	}
	else {
		// let's check if it was actually a redirect
		if (isHttpRedirect()) {
			reportRedirect();
			output.remove();
		}
		else {
			printf("Download Succeeded.\n");
			++downloadedCount;
		}
	}

	currentDownload->deleteLater();
	startNextDownload();
}

void SJMDownloadManager::downloadReadyRead()
{
	output.write(currentDownload->readAll());
}

bool SJMDownloadManager::isHttpRedirect() const
{
	const auto statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	return statusCode == 301 || statusCode == 302 || statusCode == 303
		|| statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void SJMDownloadManager::reportRedirect() const
{
	const auto statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	auto requestUrl = currentDownload->request().url();
	QTextStream(stderr) << "Request: " << requestUrl.toDisplayString()
		<< " was redirected with code: " << statusCode
		<< '\n';

	QVariant target = currentDownload->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (!target.isValid())
		return;
	QUrl redirectUrl = target.toUrl();
	if (redirectUrl.isRelative())
		redirectUrl = requestUrl.resolved(redirectUrl);
	QTextStream(stderr) << "Redirected to: " << redirectUrl.toDisplayString()
		<< '\n';
}

