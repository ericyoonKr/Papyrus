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

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	QMap<QString, CodeEditor*> openFiles;
	auto* codeEditorTab = new CodeEditorTab();
	codeEditorTab->setModel(&openFiles);
	QTNote window(codeEditorTab, &openFiles);

	ButtonPanel *buttonPanel = window.getButtonPanel();
																			
	QObject::connect(buttonPanel, &ButtonPanel::loadTriggered, 
					 &window, &QTNote::doLoad);
	QObject::connect(buttonPanel, &ButtonPanel::saveTriggered, 
		             &window, &QTNote::doSave);
	QObject::connect(buttonPanel, &ButtonPanel::saveAsTriggered, 
					 &window, &QTNote::doSaveAs);
	
	FileBrowser *fileBrowser = window.getFileBrowser();
	
	QObject::connect(fileBrowser, &FileBrowser::fileSelected, 
			 	 	 &window, &QTNote::openFile);
	QObject::connect(codeEditorTab, &CodeEditorTab::currentChanged, 
					 &window, &QTNote::onCurrentTabChanged);
	QObject::connect(codeEditorTab, &CodeEditorTab::tabModificationChanged, 
					 &window, &QTNote::onTabModificationChanged);
    QObject::connect(codeEditorTab, &CodeEditorTab::closeTabAttempt, 
					 &window, &QTNote::onCloseTabAttempt);
	
	app.setWindowIcon(QIcon(":/papyrus_logo_1024x1024.png"));

	window.show();
	return app.exec();

}
