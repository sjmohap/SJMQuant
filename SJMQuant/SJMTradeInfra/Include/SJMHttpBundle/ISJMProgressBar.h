#pragma once
#include <QString>

class ISJMProgressBar
{
public:
	virtual void clear();
	virtual void update();
	virtual void setMessage(const QString &message);
	virtual void setStatus(qint64 value, qint64 maximum);
	virtual ~ISJMProgressBar() = default;
};
