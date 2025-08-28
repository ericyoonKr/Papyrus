// CodeEditorTab.h

#pragma once

#include <QTabWidget>
#include <QMap>
#include "CodeEditor.h"
#include "TabManagerInterface.h"

class CodeEditorTab final : public QTabWidget, public TabManagerInterface {
    Q_OBJECT
    Q_INTERFACES(TabManagerInterface) 


public:
    explicit CodeEditorTab(QWidget* parent = nullptr);
    void setModel(QMap<QString, CodeEditor*>* openFilesModel);
    void addNewTab(const QString& filePath, const QString& content) override;
    void updateTabInfo(int index, const QString& newFilePath) override;
    void closeTab(int index) override;
    void setTabText(int index, const QString& text) override;

    CodeEditor *editorFor(int index) const override;
    QString filePathFor(int index) const override;
    bool isTabModified(int index) const override;
    int currentIndex() const override;
    void setCurrentWidget(QWidget* widget) override;
    QWidget* asWidget() override {return this;}

signals:
    void currentChanged(int index);
    void tabModificationChanged(int index, bool modified);
    void closeTabAttempt(int index);
private slots:
    void onTabCloseRequested(int index);
private:
    QMap<QString, CodeEditor*> *m_openFiles_ptr = nullptr;
};