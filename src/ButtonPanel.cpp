//ButtonPanel.cpp

#include "ButtonPanel.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileInfo>
#include <QObject>

ButtonPanel::ButtonPanel(QWidget* parent)
{

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

	connect(m_buttonLoad, &QPushButton::clicked, 
			this, &ButtonPanel::onLoadButtonClicked);
	connect(m_buttonSave, &QPushButton::clicked, 
			this, &ButtonPanel::onSaveButtonClicked);
	connect(m_buttonSaveAs, &QPushButton::clicked, 
			this, &ButtonPanel::onSaveAsButtonClicked);
}

void ButtonPanel::updateFilePathLabel(const QString& filePath)
{

	if(filePath.isEmpty())
	{
		m_filePathLabel->setText("No File Loaded");
	} 
	else 
	{
		m_filePathLabel->setText(QFileInfo(filePath).fileName());
	}
}


void ButtonPanel::onLoadButtonClicked(){emit loadTriggered();}
void ButtonPanel::onSaveButtonClicked(){emit saveTriggered();}
void ButtonPanel::onSaveAsButtonClicked(){emit saveAsTriggered();}
