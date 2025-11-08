// main.cpp

#include "QTNote.h"
#include "TabManagerInterface.h"
#include "CodeEditorTab.h"
#include "FileBrowser.h"
#include "ButtonPanel.h"
#include "CodeEditorTab.h"
#include <QtWidgets/QApplication>
#include <QIcon>
#include <QMap>
#include <QTreeView>
#include <QPushButton>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QMap<QString, CodeEditor*> openFiles;
	auto* codeEditorTab = new CodeEditorTab();
	codeEditorTab->setModel(&openFiles);
	QTNote window(codeEditorTab, &openFiles);
	
	app.setWindowIcon(QIcon(":/papyrus_logo_1024x1024.png"));

	window.show();
	return app.exec();
}
