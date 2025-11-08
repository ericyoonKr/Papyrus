#pragma once

#include "TabManagerInterface.h"
#include "FileManager.h"
#include <QObject>
#include <QMap>

class CodeEditor;

class DocumentManager : public QObject
{
    Q_OBJECT

public:
    explicit DocumentManager(TabManagerInterface* tabManager, QMap<QString, CodeEditor*>* openFiles, QObject* parent = nullptr);

public slots:
   
    void onOpenFileRequested(const QString& filePath);
    void onLoadFromFileDialogRequested();
    void onSaveCurrentFileRequested();
    void onSaveAsCurrentFileRequested();
    void onCloseTabAttemptRequested(int index);

signals:
   
    void updateUINeeded(int index); 
    void criticalErrorOccurred(const QString& title, const QString& message);

private:
    bool saveFileLogic(int index);

    TabManagerInterface* m_tabManager;
    QMap<QString, CodeEditor*>* m_openFiles;
    FileManager m_fileManager;
    QWidget* m_parentWidget; 
};
