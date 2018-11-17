#pragma once

#include <QtNetwork>
#include <QtCore>


class ISJMDownloadManager
{
protected:
	virtual ~ISJMDownloadManager(){}
	virtual bool doesFileExist(const QString& fileURL) = 0;
	virtual QString getSaveFileName(const QString& url) const = 0;
	virtual QString downloadAndGetFilePath(const QString& fileURL) = 0;
	virtual void appendDownloadLinks(const QStringList &urls) = 0;
};

