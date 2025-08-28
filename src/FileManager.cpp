#include "FileManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QObject>
#include <qfloat16.h>
#include <qimage.h>

bool FileManager::saveTextToFile(const QString& filePath, const QString& content) {

	QFile file(filePath);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
		qWarning() << "Could not open file for writing: " << file.errorString();
		return false;
	}
	QTextStream out(&file);
	out << content;
	file.close();
	return true;
}

QString FileManager::loadTextfromFile(const QString& filePath, bool& ok) {

	QFile file(filePath);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

		qWarning() << "Could not open file for reading." << file.errorString();
		ok = false;
		return QString();
	}
	QTextStream in(&file);
	QString content = in.readAll();
	file.close();
	ok = true;
	return content;
}
