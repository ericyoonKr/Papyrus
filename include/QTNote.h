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
#include "DocumentManager.h"

using namespace std;
class TabManagerInterface;
class CodeEditor;
class ButtonPanel;
class FileBrowser;
class DocumentManager;
class QTNote : public QMainWindow 
{
	
	Q_OBJECT

public:
	explicit QTNote(TabManagerInterface* tabManager, QMap<QString, CodeEditor*>* openFilesMode, 
					QWidget* parent = nullptr);
	~QTNote() override;
	ButtonPanel* getButtonPanel() const;
	FileBrowser* getFileBrowser() const;

public slots:
	//bool doSave();
	//bool doSaveAs();
	//void doLoad();
	//void openFile(const QString& filePath);

private slots:
	void onCurrentTabChanged();
	void onTabModificationChanged(int index, bool isModified);
	void showCriticalError(const QString& title, const QString& message);
	void onCloseTabAttempt(int index);

private: 

	void updateUIForTab(int index);
	//bool saveFileLogic(int index);

	//QMap <QString, CodeEditor*> *m_openFiles_ptr;
	TabManagerInterface* m_tabManagerInterface;
	ButtonPanel* m_buttonPanel;
	FileBrowser* m_fileBrowser;
	//FileManager m_fileManager;
	DocumentManager *m_docManager;
};
