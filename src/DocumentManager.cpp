#include "DocumentManager.h"
#include "CodeEditor.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

DocumentManager::DocumentManager(TabManagerInterface* tabManager, QMap<QString, CodeEditor*>* openFiles, QObject* parent)
    : QObject(parent), m_tabManager(tabManager), m_openFiles(openFiles)
{
    
    m_parentWidget = m_tabManager->asWidget();
}

void DocumentManager::onOpenFileRequested(const QString& filePath)
{
    if (filePath.isEmpty()) return;
    if (m_openFiles->contains(filePath)) {
        m_tabManager->setCurrentWidget(m_openFiles->value(filePath));
        return;
    }

    bool success = false;
    QString content = m_fileManager.loadTextfromFile(filePath, success);
    if (success) 
	{
        m_tabManager->addNewTab(filePath, content);
    } 
	else 
	{
        emit criticalErrorOccurred("Error", "Cannot open file " + filePath);
    }
}

void DocumentManager::onLoadFromFileDialogRequested()
{
    const QString filePath = QFileDialog::getOpenFileName(m_parentWidget, "Open File", QDir::homePath(), "All Files (*.*)");
    onOpenFileRequested(filePath);
}

void DocumentManager::onSaveCurrentFileRequested()
{
    saveFileLogic(m_tabManager->currentIndex());
}

void DocumentManager::onSaveAsCurrentFileRequested()
{
    int currentIndex = m_tabManager->currentIndex();
    if (currentIndex == -1) return;

    CodeEditor* editor = m_tabManager->editorFor(currentIndex);
    if (!editor) return;

    const QString newFilePath = QFileDialog::getSaveFileName(m_parentWidget, "Save As", QDir::homePath(), "All Files (*.*)");
    if (newFilePath.isEmpty()) return;

    if (m_fileManager.saveTextToFile(newFilePath, editor->toPlainText())) 
	{
        m_tabManager->updateTabInfo(currentIndex, newFilePath);
        editor->document()->setModified(false);
        emit updateUINeeded(currentIndex);
    } else {
        emit criticalErrorOccurred("Error", "Failed to save file.");
    }
}

bool DocumentManager::saveFileLogic(int index)
{
    if (index == -1) return false;
    QString filePath = m_tabManager->filePathFor(index);
    CodeEditor* editor = m_tabManager->editorFor(index);
    if (!editor) return false;

    if (filePath.isEmpty()) {
        onSaveAsCurrentFileRequested();
        return editor->document()->isModified() == false;
    }

    if (m_fileManager.saveTextToFile(filePath, editor->toPlainText())) 
	{
        editor->document()->setModified(false);
        emit updateUINeeded(index); 
        return true;
    }

    emit criticalErrorOccurred("Error", "Cannot save file.");
    return false;
}

void DocumentManager::onCloseTabAttemptRequested(int index)
{
    if (!m_tabManager->isTabModified(index)) 
	{
        m_tabManager->closeTab(index);
        return;
    }

    QString fileName = QFileInfo(m_tabManager->filePathFor(index)).fileName();
    if (fileName.isEmpty()) fileName = "Untitled";

    QMessageBox msgBox(m_parentWidget);
    msgBox.setText(QString("'%1' file has been modified.").arg(fileName));
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) 
	{
        if (saveFileLogic(index)) 
		{
            m_tabManager->closeTab(index);
        }
    } 
	else if (ret == QMessageBox::Discard) 
	{
        m_tabManager->closeTab(index);
    }
}
