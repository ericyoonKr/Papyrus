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


signals:

	void fileSelected(const QString& filePath);

private slots:

private:

	QTreeView* m_treeView;
	QLabel* m_fileBrowserLabel;
	QFileSystemModel* m_fileSystemModel;
};
