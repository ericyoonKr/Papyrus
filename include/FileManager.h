#pragma once

#include <QString>
#include <QObject>

class FileManager : public QObject 
{

	Q_OBJECT

public:
	FileManager() = default;
	bool saveTextToFile(const QString& filePath, const QString& content);
	QString loadTextfromFile(const QString& filePath, bool& ok);

};

/*
 * Meahwhile, Gemini Recommends this header to be non-Q_OBJECT
 * However, this caused compile error before refactoring to current MVC structure
 */
