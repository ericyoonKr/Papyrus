//ButtonPanel.cpp

#include "ButtonPanel.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileInfo>
#include <QObject>

ButtonPanel::ButtonPanel(QWidget* parent){

	QGridLayout* layout = new QGridLayout(this);

	m_buttonLoad = new QPushButton("Load", this);
	m_buttonSave = new QPushButton("Save", this);
	m_buttonSaveAs = new QPushButton("Save As", this);
	m_filePathLabel = new QLabel("No file loaded", this);
	m_filePathLabel->setAlignment(Qt::AlignCenter);

	QHBoxLayout* leftBoxLayout = new QHBoxLayout();
	leftBoxLayout->addWidget(m_buttonSave);
	leftBoxLayout->addWidget(m_buttonSaveAs);
	leftBoxLayout->addStretch();

	layout->addLayout(leftBoxLayout, 0, 0);
	layout->addWidget(m_filePathLabel, 0, 1);
	layout->addWidget(m_buttonLoad,0, 2, Qt::AlignRight);

	layout->setColumnStretch(1,1);

}

void ButtonPanel::updateFilePathLabel(const QString& filePath){

	if(filePath.isEmpty()){
		m_filePathLabel->setText("No File Loaded");
	} else {
		m_filePathLabel->setText(QFileInfo(filePath).fileName());
	}
}

QPushButton* ButtonPanel::getLoadButton() const {return m_buttonLoad;}
QPushButton* ButtonPanel::getSaveButton() const {return m_buttonSave;}
QPushButton* ButtonPanel::getSaveAsButton() const {return m_buttonSaveAs;}