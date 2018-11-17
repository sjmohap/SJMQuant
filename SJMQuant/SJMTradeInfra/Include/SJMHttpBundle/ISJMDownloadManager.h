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

class sjmDownloadManager : public ISJMDownloadManager
{
protected:
	~sjmDownloadManager() override;
	bool doesFileExist(const QString& fileURL) override;
	QString getSaveFileName(const QString& url) const override;
	QString downloadAndGetFilePath(const QString& fileURL) override;
	void appendDownloadLinks(const QStringList& urls) override;
public:

};
