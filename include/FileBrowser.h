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
class FileActionHandler;

class FileBrowser final : public QWidget {

	Q_OBJECT

public:

	explicit FileBrowser(QWidget* parent = nullptr);
	QTreeView *getTreeView() const;

public slots:

signals:

	void fileSelected(const QString& filePath);

private slots:
	void onTreeViewClicked(const QModelIndex& index);
	void showContextMenu(const QPoint &pos);

private:
	QTreeView* m_treeView;
	QLabel* m_fileBrowserLabel;
	QFileSystemModel* m_fileSystemModel;

	FileActionHandler *m_actionHandler;
};
