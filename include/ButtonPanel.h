// ButtonPanel.h

#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <qtmetamacros.h>

class QPushButton;
class QLabel;

class ButtonPanel final : public QWidget {

	Q_OBJECT

public:
	explicit ButtonPanel(QWidget* parent = nullptr);

	QPushButton* getLoadButton() const;
	QPushButton* getSaveButton() const;
	QPushButton* getSaveAsButton() const;

public slots:
	void updateFilePathLabel(const QString& filePath);

signals:
	void loadTriggered();
	void saveTriggered();
	void saveAsTriggered();

private:
	QPushButton* m_buttonSave;
	QPushButton* m_buttonSaveAs;
	QPushButton* m_buttonLoad;
	QLabel* m_filePathLabel;
};
