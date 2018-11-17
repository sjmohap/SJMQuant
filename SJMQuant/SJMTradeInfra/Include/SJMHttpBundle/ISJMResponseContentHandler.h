#pragma once

#include <map>
class QString;

class ISJMResponseContentHandler
{
public:
	ISJMResponseContentHandler() = default;
	virtual ~ISJMResponseContentHandler() = default;
	virtual bool handle_content(const QString& key, const std::map<QString, QString>& map) = 0;
};

using ISJMResponseContentHandlerPtr = std::shared_ptr<ISJMResponseContentHandler>;