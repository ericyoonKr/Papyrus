// CodeEditorTab.cpp

#include "CodeEditorTab.h"
#include <QFileInfo>
#include <QDebug>

CodeEditorTab::CodeEditorTab(QWidget* parent) : QTabWidget(parent){

    setTabsClosable(true);
    setMovable(true);
    connect(this, &QTabWidget::currentChanged, 
            this, &CodeEditorTab::currentChanged);
    connect(this, &QTabWidget::tabCloseRequested, 
            this, &CodeEditorTab::onTabCloseRequested);
}

// Model Point
// Enable tabs to control CodeEditor
void CodeEditorTab::setModel(QMap<QString, CodeEditor*>* openFilesModel){

    m_openFiles_ptr = openFilesModel; 
}

void CodeEditorTab::onTabCloseRequested(int index){

    if(isTabModified(index)) {
        emit closeTabAttempt(index);
    } else {
        closeTab(index);
    }
}



void CodeEditorTab::addNewTab(const QString& filePath, const QString &content){

    Q_ASSERT(m_openFiles_ptr != nullptr);

    int currentIndex = this->currentIndex();
    CodeEditor* currentEditor = editorFor(currentIndex);

    bool isUntitled = (currentIndex != -1) ? filePathFor(currentIndex).isEmpty() : false;
    bool isModified = (currentIndex != -1) ? isTabModified(currentIndex) : false;

    if (currentEditor && isUntitled && !isModified) {
        m_openFiles_ptr->remove("");

        currentEditor->setPlainText(content);
        currentEditor->document()->setModified(false);

        const QString fileName = QFileInfo(filePath).fileName();
        setTabText(currentIndex, fileName);
        setTabToolTip(currentIndex, filePath);
        
        m_openFiles_ptr->insert(filePath, currentEditor);

    } else {
        CodeEditor *newEditor = new CodeEditor(this);

        newEditor->setPlainText(content);
        newEditor->document()->setModified(false);

        connect(newEditor->document(), &QTextDocument::modificationChanged, 
                this, [this, newEditor](bool modified) {
                    int index = indexOf(newEditor);
                    if(index != -1) {
                        emit tabModificationChanged(index, modified);
                    }
                }
            );

        const QString fileName = filePath.isEmpty() ? "Untitled" : QFileInfo(filePath).fileName();
        const int newIndex = addTab(newEditor, fileName);
        setTabToolTip(newIndex, filePath);
        setCurrentIndex(newIndex);

        m_openFiles_ptr->insert(filePath, newEditor);
    }
}

void CodeEditorTab::updateTabInfo(int index, const QString &newFilePath){

    Q_ASSERT(m_openFiles_ptr != nullptr);

    if(index < 0 || index >= count()) return;
    QString oldFilePath = filePathFor(index);
    CodeEditor *editor = editorFor(index);

    // this part needs to be checked... inevitable?
    if (m_openFiles_ptr -> contains(oldFilePath)){
        m_openFiles_ptr -> insert(newFilePath, editor);
        setTabToolTip(index, newFilePath);
    }

    m_openFiles_ptr -> insert(newFilePath, editor);
    setTabToolTip(index, newFilePath);
}

void CodeEditorTab::closeTab(int index){

    Q_ASSERT(m_openFiles_ptr != nullptr);
    QString filePath = filePathFor(index);
    CodeEditor* editor = editorFor(index);
    if(editor){
        m_openFiles_ptr->remove(filePath);
        removeTab(index);
        editor->deleteLater();
    }
}

void CodeEditorTab::setTabText(int index, const QString &text){
    
    if (index >= 0 && index < count()) {

        QTabWidget::setTabText(index, text);
    }
}

CodeEditor *CodeEditorTab::editorFor(int index) const {

    return qobject_cast<CodeEditor*>(widget(index));
}

QString CodeEditorTab::filePathFor(int index) const {

    if (index >= 0 || index >= count()) return tabToolTip(index);

    return QString();
}

bool CodeEditorTab::isTabModified(int index) const {

    CodeEditor *editor = editorFor(index);
    return editor ? editor->document()->isModified() : false;
}

int CodeEditorTab::currentIndex() const {
    
    return QTabWidget::currentIndex();
}

void CodeEditorTab::setCurrentWidget(QWidget *widget){

    QTabWidget::setCurrentWidget(widget);
}
