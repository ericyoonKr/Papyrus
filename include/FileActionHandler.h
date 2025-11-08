// FileActionHandler.h 

#pragma once 

#include <qfilesystemmodel.h>
#include <qtreeview.h>

class QTreeView;
class QFileSystemModel;
class QPoint;
class QModelIndex;


class FileActionHandler final 
{

public:

	explicit FileActionHandler(QTreeView *treeView, QFileSystemModel *model);
	// Public interface that FileBrowser would call 
	// only entry point 
	void setupContextMenu(const QPoint &pos);

private:

	void createNewFolder(const QModelIndex &parentIndex);
	void createNewFile(const QModelIndex &parentIndex);
	void renameItem(const QModelIndex &index);
	void deleteItem(const QModelIndex &index);

	QTreeView *m_treeView;
	QFileSystemModel *m_model;

};
