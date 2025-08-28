#pragma once

#include <QString>

class QWidget;
class CodeEditor;

class TabManagerInterface {

public:

    virtual ~TabManagerInterface() = default;
    virtual void addNewTab(const QString& filePath, const QString& content) = 0;
    virtual void updateTabInfo(int index, const QString& newFilePath) = 0;
    virtual void closeTab(int index) = 0;
    virtual void setTabText(int index, const QString& text) = 0;

    virtual CodeEditor* editorFor(int index) const = 0;
    virtual QString filePathFor(int index) const = 0;
    virtual bool isTabModified(int index) const = 0;
    virtual int currentIndex() const = 0;

    virtual void setCurrentWidget(QWidget* widget) = 0;
    virtual QWidget* asWidget() = 0;

};

Q_DECLARE_INTERFACE(TabManagerInterface, "papyrus.TabManagerInterface")
