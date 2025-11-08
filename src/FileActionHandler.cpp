#include "FileActionHandler.h"
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile> 
#include <QTreeView> 
#include <QTreeView>
#include <QFileSystemModel>


/*
 * Note that FileActionHandler deos not have Q_OBJECT
 *
 * REFACTORING
 *
 * This class does not connect FileActionHandler's slot
 * Instead, using lambda function
 */
FileActionHandler::FileActionHandler(QTreeView *treeView, QFileSystemModel *model) 
									: m_treeView(treeView), m_model(model){}

void FileActionHandler::setupContextMenu(const QPoint &pos){

	QModelIndex index = m_treeView->indexAt(pos);  
	QMenu menu(m_treeView);

	if(index.isValid())
	{
		QString path = m_model -> filePath(index);
		QFileInfo fileInfo(path);

		if (fileInfo.isDir())
		{
			QAction *newFolderAction = menu.addAction("New Folder");
			QAction *newFileAction = menu.addAction("New File");
			menu.addSeparator();
			QAction *renameAction = menu.addAction("Change Name");
			QAction *deleteAction = menu.addAction("Delete");
			
			// lambda calls FileActionHandler's member function 
			// capture [this] to access member function and memeber variants
			// connect is QObject static.. so QObject is inevitable
			QObject::connect(newFolderAction, &QAction::triggered, [this, index]() 
		   			{createNewFolder(index);});
			QObject::connect(newFileAction, &QAction::triggered, [this, index]() 
		   			{createNewFile(index);});
			QObject::connect(renameAction, &QAction::triggered, [this, index]() 
		   			{renameItem(index);});
			QObject::connect(newFolderAction, &QAction::triggered, [this, index]() 
		   			{deleteItem(index);});
		}
		else if (fileInfo.isFile())
		{
			QAction *newFileAction = menu.addAction("New File");
			QAction *renameAction = menu.addAction("Change Name");
			menu.addSeparator();
			QAction *deleteAction = menu.addAction("Delete");

			QObject::connect(newFileAction, &QAction::triggered, [this, index]() 
		   			{createNewFile(index);});
			QObject::connect(renameAction, &QAction::triggered, [this, index]() 
		   			{renameItem(index);});
			QObject::connect(deleteAction, &QAction::triggered, [this, index]() 
		   			{deleteItem(index);});
		}
		else 
		{
			QModelIndex parentIndex = m_treeView->rootIndex();

			QAction *newFolderAction = menu.addAction("New Folder");
			QAction *newFileAction = menu.addAction("New File");

			QObject::connect(newFolderAction, &QAction::triggered, [this, index]() 
		   			{createNewFolder(index);});
			QObject::connect(newFileAction, &QAction::triggered, [this, index]() 
		   			{createNewFile(index);});

		}

		menu.exec(m_treeView->viewport()->mapToGlobal(pos));
	}
}

void FileActionHandler::createNewFolder(const QModelIndex &parentIndex)
{
	QString parentPath = m_model->filePath(parentIndex);
    if (!QFileInfo(parentPath).isDir()) {
        parentPath = m_model->filePath(parentIndex.parent());
    }
    bool ok;
    QString folderName = QInputDialog::getText(m_treeView, "New Folder",
                                               "Folder Name : ", QLineEdit::Normal,
                                               "New Folder", &ok);
    if (ok && !folderName.isEmpty()) {
        QDir dir(parentPath);
        if (!dir.mkdir(folderName)) {
            QMessageBox::warning(m_treeView, "Error", "Cannot make new folder");
        }
    }

}

void FileActionHandler::createNewFile(const QModelIndex &parentIndex){

	QString parentPath = m_model->filePath(parentIndex);
    if (!QFileInfo(parentPath).isDir()) {
        parentPath = m_model->filePath(parentIndex.parent());
    }
    bool ok;

    QString fileName = QInputDialog::getText(m_treeView, "New File",
                                             "File Name", QLineEdit::Normal,
                                             "Untitled.txt", &ok);
	if (ok && !fileName.isEmpty()) {
        QFile file(parentPath + "/" + fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(m_treeView, "Error", "Cannot make new file");
        } else {
            file.close();
        }
    }
} 

void FileActionHandler::renameItem(const QModelIndex &index){
	QString oldPath = m_model->filePath(index);
    QFileInfo fileInfo(oldPath);

    bool ok;
    QString newName = QInputDialog::getText(m_treeView, "Change Name",
                                            "New Name", QLineEdit::Normal,
                                            fileInfo.fileName(), &ok);

    if (ok && !newName.isEmpty() && newName != fileInfo.fileName()) {
        QString newPath = fileInfo.absolutePath() + "/" + newName;
        if (!QFile::rename(oldPath, newPath)) {
            QMessageBox::warning(m_treeView, "Error", "Cannot change name.");
        }
    }
}

void FileActionHandler::deleteItem(const QModelIndex &index){
 QString path = m_model->filePath(index);
    QFileInfo fileInfo(path);
    QString itemType = fileInfo.isDir() ? "Folder" : "File";
    
    auto reply = QMessageBox::question(m_treeView, "Confirm Delete", 
                                       QString("Are you sure you want to permanently delete '%1'?").arg(fileInfo.fileName()),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    bool success = fileInfo.isDir() ? QDir(path).removeRecursively() : QFile::remove(path);

    if (!success) {
        QMessageBox::warning(m_treeView, "Error", QString("Cannot delete %1.").arg(itemType));
    }
}
