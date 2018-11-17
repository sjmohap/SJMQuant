#pragma once
#include "ISJMProgressBar.h"

class SJMCommandlineTextProgressBar : public ISJMProgressBar
{
public:
	void clear() override;
	void update() override;
	void setMessage(const QString& message) override;
	void setStatus(qint64 value, qint64 maximum) override;
private:
	QString message;
	qint64 value = 0;
	qint64 maximum = -1;
	int iteration = 0;
};
