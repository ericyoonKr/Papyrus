#pragma once

#include <QWidget>
#include <QLabel>
#include <QTreeView>
#include <QFileSystemModel>
#include <QString>
#include <qtmetamacros.h>
#include <qwidget.h>

class QLabel;
class QTreeView;
class QFileSystemModel;
class QModelIndex;


class FileBrowser final : public QWidget {

	Q_OBJECT

public:

	explicit FileBrowser(QWidget* parent = nullptr);
	QTreeView *getTreeView() const;

public slots:
	// QTreeView's click signal receive and handling slot 
	void onTreeViewClicked(const QModelIndex& index);

	/* 
	 * right click 
	 * slot to connect 
	 * customContextMenuRequested signal on TreeView 
	 */ 
	void onTreeViewRightClicked(const QPoint &pos);


signals:

	void fileSelected(const QString& filePath);

private slots:


private:


	// functions to handle file / direction 
	// parentIndex : Location 
	// index : this exact target (the file or directory itself)
	void createNewFolder(const QModelIndex &parentIndex);
	void createNewFile(const QModelIndex &parentIndex);
	void deleteItem(const QModelIndex &index);
	void renameItem(const QModelIndex &index);

	QTreeView* m_treeView;
	QLabel* m_fileBrowserLabel;
	QFileSystemModel* m_fileSystemModel;
};
