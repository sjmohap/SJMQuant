#pragma once
#include <QString>
#include "SJMHttpBundleCommonDef.h"

class DLL_SJMHTTPBUNDLE ISJMProgressBar
{
public:
	virtual void clear() = 0;
	virtual void update() = 0;
	virtual void setMessage(const QString &message) = 0;
	virtual void setStatus(qint64 value, qint64 maximum) = 0;
	virtual ~ISJMProgressBar() = default;
};

