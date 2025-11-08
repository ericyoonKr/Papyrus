// FileBrowser.cpp

#include "FileBrowser.h"
#include "FileActionHandler.h"
#include <QModelIndex>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QObject>
#include <QMessageBox>
#include <QHeaderView>
#include <qinputdialog.h>
#include <qpoint.h>

FileBrowser::FileBrowser(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	QString rootPath;

#ifdef Q_OS_WIN
	rootPath = QDir::rootPath();
#elif defined(Q_OS_LINUX)
	rootPath = "/home";
#elif defined(Q_OS_MACOS)
	rootPath = "/Users";
#endif

	//const QString rootPath = QDir::rootPath();

	m_fileSystemModel = new QFileSystemModel(this);
	m_fileSystemModel->setRootPath(rootPath);

	m_treeView = new QTreeView(this);
	m_treeView->setModel(m_fileSystemModel);

	
	m_treeView->setRootIndex(m_fileSystemModel->index(rootPath));

	m_treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	m_treeView->header()->setSectionResizeMode(1, QHeaderView::Interactive);
    m_treeView->header()->setSectionResizeMode(2, QHeaderView::Interactive);
    m_treeView->header()->setSectionResizeMode(3, QHeaderView::Interactive);

	for (int i = 1; i < m_fileSystemModel->columnCount(); ++i) 
	{
        m_treeView->resizeColumnToContents(i);
    }

	// enable treeView right click 
	m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);

	m_fileBrowserLabel = new QLabel("FileBrowser",this);

	layout->addWidget(m_fileBrowserLabel);
	layout->addWidget(m_treeView);

	/*
	 * DEPENDENCY INJECTION
	 */
	m_actionHandler = new FileActionHandler(m_treeView, m_fileSystemModel);

	connect(m_treeView, &QTreeView::clicked, 
		 	this, &FileBrowser::onTreeViewClicked);
	connect(m_treeView, &QWidget::customContextMenuRequested, 
		 	this, &FileBrowser::showContextMenu);
		
}

QTreeView *FileBrowser::getTreeView() const {return m_treeView; }

void FileBrowser::onTreeViewClicked(const QModelIndex& index)
{

	if(m_fileSystemModel->fileInfo(index).isFile())
	{
		emit fileSelected(m_fileSystemModel->filePath(index));
	}
}

void FileBrowser::showContextMenu(const QPoint &pos)
{
	m_actionHandler->setupContextMenu(pos);
}
