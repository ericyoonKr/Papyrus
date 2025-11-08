//QTNote.cpp

#include "QTNote.h"
#include "CodeEditor.h" 
#include "TabManagerInterface.h"
#include "DocumentManager.h"
#include <QGuiApplication>
#include <QDir>
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
QTNote::QTNote(TabManagerInterface* tabManager, QMap<QString, CodeEditor*> *openFilesModel, QWidget *parent)
			   : QMainWindow(parent), m_tabManagerInterface(tabManager) 
{

	// template used
	// dynamic casting
	// Memory Management via Qt Object Tree
	// by making a virturl function, asWidget() inside TabManagerInterface,
	// QTNote gets QWidget*, by code below
	// QWidget *tabWidget = m_tabManager->asWidget();
	// if(tabWidget){tabWidget->setParent(this);}

	// check CodeEditorTab.cpp for details
	//qobject_cast<QWidget*>(m_tabManagerInterface)->setParent(this);
	m_docManager = new DocumentManager(m_tabManagerInterface, openFilesModel, this);	
	m_tabManagerInterface->asWidget()->setParent(this);
	
	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen) 
	{
		QRect available = screen->availableGeometry();
		const double aspect = 16.0 / 10.0;

		int w = int(available.width() * 0.75);
		int h = int(w / aspect);

		if( h > int(available.height() * 0.75)) 
		{
			h = int(available.height() * 0.75);
            w = int(h * aspect);
		}
		resize(w, h);
		move(
			 available.x() + (available.width() - w) / 2,
			 available.y() + (available.height() - h) / 2
		);
	}

	setWindowTitle("Papyrus");
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


    connect(m_buttonPanel, &ButtonPanel::loadTriggered, m_docManager, &DocumentManager::onLoadFromFileDialogRequested);
    connect(m_buttonPanel, &ButtonPanel::saveTriggered, m_docManager, &DocumentManager::onSaveCurrentFileRequested);
    connect(m_buttonPanel, &ButtonPanel::saveAsTriggered, m_docManager, &DocumentManager::onSaveAsCurrentFileRequested);
    connect(m_fileBrowser, &FileBrowser::fileSelected, m_docManager, &DocumentManager::onOpenFileRequested);

        connect(m_docManager, &DocumentManager::updateUINeeded, this, &QTNote::updateUIForTab);
    connect(m_docManager, &DocumentManager::criticalErrorOccurred, this, &QTNote::showCriticalError);

      auto* tabWidgetObject = m_tabManagerInterface->asWidget();
    connect(tabWidgetObject, SIGNAL(currentChanged(int)), this, SLOT(onCurrentTabChanged()));
    connect(tabWidgetObject, SIGNAL(tabModificationChanged(int,bool)), this, SLOT(onTabModificationChanged(int,bool)));
    connect(tabWidgetObject, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTabAttempt(int)));

	m_tabManagerInterface->addNewTab("", "");
	updateUIForTab(-1);
}

QTNote::~QTNote(){}

// slot area
void QTNote::onCloseTabAttempt(int index) 
{
    m_docManager->onCloseTabAttemptRequested(index);
}

void QTNote::onCurrentTabChanged() 
{
	updateUIForTab(m_tabManagerInterface->currentIndex());
}

void QTNote::onTabModificationChanged(int index, bool isModified) 
{
	updateUIForTab(index);
}

void QTNote::showCriticalError(const QString& title, const QString& message) 
{
    QMessageBox::critical(this, title, message);
}

void QTNote::updateUIForTab(int index) 
{
	// if true, get file path
	// if false, get an empty string
	QString path = (index != -1) ? m_tabManagerInterface->filePathFor(index) : "";
	bool modified = (index != -1) ? m_tabManagerInterface->isTabModified(index) : false;
	QString fileName = path.isEmpty() ? "Untitled" : QFileInfo(path).fileName();
	QString modifiedMarker = modified ? "*" : "";

	setWindowTitle(QString("Papyrus - %1 %2").arg(fileName, modifiedMarker));
	m_buttonPanel->updateFilePathLabel(path);
	if(index != -1) 
	{
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
