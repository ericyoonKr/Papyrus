// QTNote.h 

#pragma once 

#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include "TabManagerInterface.h"
#include "FileManager.h"
#include "FileBrowser.h"
#include "ButtonPanel.h"

using namespace std;

class QTNote : public QMainWindow {
	
	Q_OBJECT

public:
	explicit QTNote(TabManagerInterface* tabManager, QMap<QString, CodeEditor*>* openFilesMode, 
					QWidget* parent = nullptr);
	~QTNote() override;
	ButtonPanel* getButtonPanel() const;
	FileBrowser* getFileBrowser() const;

public slots:
	bool doSave();
	bool doSaveAs();
	void doLoad();
	void openFile(const QString& filePath);
	void onCurrentTabChanged();
	void onTabModificationChanged(int index, bool isModified);
	void onCloseTabAttempt(int index);

// private slots:

	

private: 

	void updateUIForTab(int index);
	bool saveFileLogic(int index);

	QMap <QString, CodeEditor*> *m_openFiles_ptr;
	TabManagerInterface* m_tabManagerInterface;
	ButtonPanel* m_buttonPanel;
	FileBrowser* m_fileBrowser;
	FileManager m_fileManager;

};
