//QTNote.cpp


#include "QTNote.h"
#include "CodeEditor.h" 
#include "TabManagerInterface.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QSplitter>
#include <QPainter>
#include <QTextBlock>
#include <QObject>
#include <QFont> 
#include <qnamespace.h>

// need to be addressed
QTNote::QTNote(TabManagerInterface* m_tabManager, QMap<QString, CodeEditor*> *openFilesModel, QWidget *parent)
			   : QMainWindow(parent), m_openFiles_ptr(openFilesModel), m_tabManagerInterface(m_tabManager) {

	// template used
	// dynamic casting
	// Memory Management via Qt Object Tree
	// by making a virturl function, asWidget() inside TabManagerInterface,
	// QTNote gets QWidget*, by code below
	// QWidget *tabWidget = m_tabManager->asWidget();
	// if(tabWidget){tabWidget->setParent(this);}

	// check CodeEditorTab.cpp for details
	//qobject_cast<QWidget*>(m_tabManagerInterface)->setParent(this);
	m_tabManagerInterface->asWidget()->setParent(this);
	
	
	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen) {
		QRect available = screen->availableGeometry();
		const double aspect = 16.0 / 10.0;

		int w = int(available.width() * 0.75);
		int h = int(w / aspect);

		if( h > int(available.height() * 0.75)) {
			h = int(available.height() * 0.75);
            w = int(h * aspect);
		}
		resize(w, h);
		move(
			 available.x() + (available.width() - w) / 2,
			 available.y() + (available.height() - h) / 2
		);
	}

	setWindowTitle("Main Window: QTNote");
	QWidget* centralWidget = new QWidget(this);
	centralWidget->setStyleSheet("background-color: white");
	setCentralWidget(centralWidget);

	
	m_fileBrowser = new FileBrowser(this);
	m_buttonPanel = new ButtonPanel(this);	

	QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
	QSplitter* middleSplitter = new QSplitter(Qt::Horizontal, this);

	mainLayout->addWidget(m_buttonPanel, 0);


	middleSplitter->addWidget(m_fileBrowser);
	middleSplitter->addWidget(m_tabManagerInterface->asWidget());


	
	middleSplitter->setStretchFactor(0,1);
	middleSplitter->setStretchFactor(1,5);

	mainLayout->addWidget(middleSplitter, 1);

	/*
	 * main layout, second argument 0 menas minimun, 1 means rest && all
	 */
	m_tabManagerInterface->addNewTab("", "");

	connect(m_fileBrowser, &FileBrowser::fileSelected, 
			this, &QTNote::openFile);

}

QTNote::~QTNote(){}


void QTNote::openFile(const QString& filePath){

	if(filePath.isEmpty()) return;
	if(m_openFiles_ptr->contains(filePath)){

		m_tabManagerInterface->setCurrentWidget(m_openFiles_ptr->value(filePath));
		return;
	}

	bool success = false;
	QString content = m_fileManager.loadTextfromFile(filePath, success);
	if(success){

		m_tabManagerInterface->addNewTab(filePath, content);
	} else {
		QMessageBox::critical(this, "Error", "Cannot open file " + filePath);
	}
}


void QTNote::doLoad(){

	const QString filePath = QFileDialog::getOpenFileName(this, "Open File", "~/", "Files(*.*)");
	openFile(filePath);
}


bool QTNote::doSave(){

	return saveFileLogic(m_tabManagerInterface->currentIndex());
}



bool QTNote::doSaveAs(){

	int currentIndex = m_tabManagerInterface->currentIndex();
	if(currentIndex == -1) return false;

	CodeEditor *editor = m_tabManagerInterface->editorFor(currentIndex);
	if(!editor) return false;

	const QString newFilePath = QFileDialog::getSaveFileName(this, "Save As", 
															 "QDir::homePath()", "Files(*.*)" );

	
	if(newFilePath.isEmpty()) return false;
	if(m_fileManager.saveTextToFile(newFilePath, editor->toPlainText())){
		m_tabManagerInterface->updateTabInfo(currentIndex, newFilePath);
		editor->document()->setModified(false);
	}
	QMessageBox::critical(this, "Error", "Failed to save file.");

	return false;
	// if is Empty, do nothing
}

bool QTNote::saveFileLogic(int index){

	if(index == -1) return false;
	QString filePath = m_tabManagerInterface->filePathFor(index);
	CodeEditor *editor = m_tabManagerInterface->editorFor(index);
	if(!editor) return false;

	if(filePath.isEmpty()) {return doSaveAs();}
	// Key-point of this logic
	if (m_fileManager.saveTextToFile(filePath, editor->toPlainText())){
		editor->document()->setModified(false);
		return true;
	}
	QMessageBox::critical(this, "Error", "Cannot save file.");
	return false;
}


// slot area

void QTNote::onCloseTabAttempt(int index){

	if(!m_tabManagerInterface->isTabModified(index)){
		m_tabManagerInterface->closeTab(index);
		return;
	}

	QString fileName = QFileInfo(m_tabManagerInterface->filePathFor(index)).fileName();
	if(fileName.isEmpty()) fileName = "Untitled";
	QMessageBox msgBox(this);
	msgBox.setText(QString("%1 File was edited.").arg(fileName));
	msgBox.setInformativeText("Save changes?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);

	int ret = msgBox.exec();
	if(ret == QMessageBox::Save) {
		if(saveFileLogic(index)) {m_tabManagerInterface->closeTab(index);}
	} else if (ret == QMessageBox::Discard){
		m_tabManagerInterface->closeTab(index);
	}
}

void QTNote::onCurrentTabChanged() {

	updateUIForTab(m_tabManagerInterface->currentIndex());
}

void QTNote::onTabModificationChanged(int index, bool isModified) {

	updateUIForTab(index);
}

void QTNote::updateUIForTab(int index) {

	// if true, get file path
	// if false, get an empty string
	QString path = (index != -1) ? m_tabManagerInterface->filePathFor(index) : "";
	bool modified = (index != -1) ? m_tabManagerInterface->isTabModified(index) : false;
	QString fileName = path.isEmpty() ? "Untitled" : QFileInfo(path).fileName();
	QString modifiedMarker = modified ? "*" : "";

	setWindowTitle(QString("Papyrus - %1 %2").arg(fileName, modifiedMarker));
	m_buttonPanel->updateFilePathLabel(path);
	if(index != -1) {
		m_tabManagerInterface->setTabText(index, fileName + modifiedMarker);
	}
}
ButtonPanel* QTNote::getButtonPanel() const
{
    return m_buttonPanel;
}

FileBrowser* QTNote::getFileBrowser() const
{
    return m_fileBrowser;
}