// CodeEditor.h 
#pragma once

#include <QPlainTextEdit>
#include <QWidget>
#include <QAction>
#include <QWheelEvent>
#include <QFont>
#include <QObject>
#include <qevent.h>
#include <qtmetamacros.h>

class LineNumberArea;

class CodeEditor : public QPlainTextEdit{

	Q_OBJECT

public:

	explicit CodeEditor(QWidget* parent = nullptr);
	// LineNumberArea will call these public functions
	void setFont(const QFont &font);
	void lineNumberAreaPaintEvent(QPaintEvent* event);
	int lineNumberAreaWidth();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private slots:
	void updateLineNumberAreaWidth();
	void updateLineNumberArea(const QRect& rect, int dy);

private:
	LineNumberArea* m_lineNumberArea;
};

class LineNumberArea : public QWidget{

	Q_OBJECT

public:

	explicit LineNumberArea(CodeEditor* editor);
	QSize sizeHint() const override;

	void updateFont(const QFont &font);

protected:

	void paintEvent(QPaintEvent* event) override;

private:
	CodeEditor* m_codeEditor;
};
