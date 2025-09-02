// FileBrowser.cpp

#include "FileBrowser.h"
#include <QModelIndex>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QObject>
#include <QMessageBox>
#include <qinputdialog.h>
#include <qpoint.h>

FileBrowser::FileBrowser(QWidget *parent){

	QVBoxLayout *layout = new QVBoxLayout();

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

	// enable treeView right click 
	m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);

	m_fileBrowserLabel = new QLabel("FileBrowser",this);

	layout->addWidget(m_fileBrowserLabel);
	layout->addWidget(m_treeView);

	// missing part
	setLayout(layout);
	connect(m_treeView, &QTreeView::clicked, 
		 	this, &FileBrowser::onTreeViewClicked);
	connect(m_treeView, &QWidget::customContextMenuRequested, 
		 	this, &FileBrowser::onTreeViewRightClicked);
		
}

QTreeView *FileBrowser::getTreeView() const {return m_treeView; }

void FileBrowser::onTreeViewClicked(const QModelIndex& index){

	if(m_fileSystemModel->fileInfo(index).isFile()){
		emit fileSelected(m_fileSystemModel->filePath(index));
	}
}



/*
 * Logic of this function :
 *    
 *    1.   Check right click position, using index.isValid() 
 *    2.   Compose Menu by Multiple Situations
 *    2-1.     Right Click above Directory
 *    2-2.     Right Click above File 
 *    2-3.     Right Click above a Vacant Space  
 *    3.   Connect each action && feature [Lambda Funcion]
 *    
 */

void FileBrowser::onTreeViewRightClicked(const QPoint &pos){

	// QModelIndex ...
	// identify specific item on object
	QModelIndex index = m_treeView->indexAt(pos);
	QMenu menu(this); // Stack Allocation , delete when function is dead
	// QMenu *menu = new QMenu(this); <- Heap Allocation , delete when object is dead 

	if(index.isValid()){
		
		// Right Click on item 
		QString path = m_fileSystemModel->filePath(index);
		QFileInfo fileInfo(path);

		if(fileInfo.isDir()){

			QAction *newFolderAction = menu.addAction("New Folder");
			QAction *newFileAction = menu.addAction("Add New File");
			menu.addSeparator();
			QAction *renameAction = menu.addAction("Change Name");
			QAction *deleteAction = menu.addAction("Delete");

			// Connect Action and Lambda Function 
			connect(newFolderAction, &QAction::triggered,
		   			[this, index](){createNewFolder(index);});
			connect(newFileAction, &QAction::triggered,
		   			[this, index](){createNewFile(index);});
			connect(renameAction, &QAction::triggered,
		   			[this, index](){renameItem(index);});
			connect(deleteAction, &QAction::triggered,
		   			[this, index](){deleteItem(index);});

		} else if(fileInfo.isFile()){

			QAction *newFileAction = menu.addAction("New Item");
			QAction *renameAction = menu.addAction("Change Name");
			menu.addSeparator();
			QAction *deleteAction = menu.addAction("Delete");
			connect(newFileAction, &QAction::triggered,
		   			[this, index](){createNewFile(index);});
			connect(renameAction, &QAction::triggered,
		   			[this, index](){renameItem(index);});
			connect(deleteAction, &QAction::triggered,
		   			[this, index](){deleteItem(index);});

		} 
	}else {

		QModelIndex parentIndex = m_treeView->rootIndex();

		QAction *newFolderAction = menu.addAction("New Folder");
		QAction *newFileAction = menu.addAction("New Item"); 

		connect(newFolderAction, &QAction::triggered,
	   			[this, parentIndex](){createNewFolder(parentIndex);});
		connect(newFileAction, &QAction::triggered,
	   			[this, parentIndex](){createNewFile(parentIndex);});
		}
	menu.exec(m_treeView->viewport()->mapToGlobal(pos));
}


void FileBrowser::createNewFolder(const QModelIndex &parentIndex){

	QString parentPath = m_fileSystemModel->filePath(parentIndex);
	if(!QFileInfo(parentPath).isDir()) {

		parentPath = m_fileSystemModel->filePath(parentIndex.parent());
	}
	bool ok;
	QString folderName = QInputDialog::getText(this, "New Folder", 
											   "Folder Name : ", QLineEdit::Normal,
											   "New Folder", &ok);
	if(ok && !folderName.isEmpty()){
		
		QDir dir(parentPath);
		if(!dir.mkdir(folderName)){
			QMessageBox::warning(this, "Error", "Cannot make new folder");
		}
	}
}

void FileBrowser::createNewFile(const QModelIndex &parentIndex){

	QString parentPath = m_fileSystemModel->filePath(parentIndex);
	if(!QFileInfo(parentPath).isDir()) {
		parentPath = m_fileSystemModel->filePath(parentIndex.parent());
	}
	bool ok;

	QString fileName = QInputDialog::getText(this, "New File", 
										     "File Name", QLineEdit::Normal,
										     "NewFile.*", &ok);
	if(ok && !fileName.isEmpty()){

		QFile file(parentPath + "/" + fileName);
		if(!file.open(QIODevice::WriteOnly)){
			QMessageBox::warning(this, "Erorr", "Cannot make new file");
		} else {

			file.close();
		}
	}
}


void FileBrowser::deleteItem(const QModelIndex &index){

	QString path = m_fileSystemModel->filePath(index);
	QFileInfo fileInfo(path);
	QString itemType = fileInfo.isDir() ? "Folder" : "File";

	bool success = fileInfo.isDir() ? QDir(path).removeRecursively() : QFile::remove(path);

	if(!success) {

		QMessageBox::warning(this, "Error", QString("Cannot delete %1.").arg(itemType));
	}
}

void FileBrowser::renameItem(const QModelIndex &index){

	QString oldPath = m_fileSystemModel->filePath(index);
	QFileInfo fileInfo(oldPath);

	bool ok;
	QString newName = QInputDialog::getText(this, "Change Name", 
										    "New Name", QLineEdit::Normal, 
										    fileInfo.fileName(), &ok);

	if(ok && !newName.isEmpty() && newName != fileInfo.fileName()){
		
		QString newPath = fileInfo.absolutePath() + "/" + newName;
		if(!QFile::rename(oldPath, newPath)) {
			QMessageBox::warning(this, "Error", "Cannot change name.");
		}
	}

}
