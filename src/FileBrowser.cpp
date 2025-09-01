// FileBrowser.cpp

#include "FileBrowser.h"
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QObject>

// constructor 
FileBrowser::FileBrowser(QWidget *parent){


	QVBoxLayout *layout = new QVBoxLayout(); 

	const QString rootPath = QDir::rootPath();

	m_fileSystemModel = new QFileSystemModel(this);
	m_fileSystemModel->setRootPath(rootPath);

	m_treeView = new QTreeView(this);
	m_treeView->setModel(m_fileSystemModel);
	
	m_treeView->setRootIndex(m_fileSystemModel->index(rootPath));

	m_fileBrowserLabel = new QLabel("FileBrowser",this);

	layout->addWidget(m_fileBrowserLabel);
	layout->addWidget(m_treeView);

	// missing part
	setLayout(layout);
	connect(m_treeView, &QTreeView::clicked, this, &FileBrowser::onTreeViewClicked);

		
}

QTreeView *FileBrowser::getTreeView() const {return m_treeView; }

void FileBrowser::onTreeViewClicked(const QModelIndex& index){

	if(m_fileSystemModel->fileInfo(index).isFile()){
		emit fileSelected(m_fileSystemModel->filePath(index));
	}
}
 


