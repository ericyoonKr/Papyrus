// CodeEditor.cpp
// Code editing page rendering engine 

#include "CodeEditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QFont>
#include <QWheelEvent>
#include <QAction>
#include <qnamespace.h>
#include <QObject>

LineNumberArea::LineNumberArea(CodeEditor *editor) 
	: QWidget(editor), m_codeEditor(editor){}

QSize LineNumberArea::sizeHint()const{

	return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent* event){
	m_codeEditor->lineNumberAreaPaintEvent(event);
}

CodeEditor::CodeEditor(QWidget* parent): QPlainTextEdit(parent)
{

	QFont font;
	font.setPointSize(14);

	// CodeEditor Widget gets stylesheet's background and border attributes
	setAttribute(Qt::WA_StyledBackground, true);
	m_lineNumberArea = new LineNumberArea(this);

	m_lineNumberArea->updateFont(this->font());

	connect(this, &CodeEditor::updateRequest, 
		    this, &CodeEditor::updateLineNumberArea);

	updateLineNumberAreaWidth();
}

void CodeEditor::setFont(const QFont& font)
{

	QPlainTextEdit::setFont(font);
	if(m_lineNumberArea) {
		m_lineNumberArea->updateFont(this->font());
		updateLineNumberAreaWidth();
	}
}


void CodeEditor::resizeEvent(QResizeEvent* event)
{
	
	QPlainTextEdit::resizeEvent(event);
	QRect cr = contentsRect();
	m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), 
									 	lineNumberAreaWidth(), cr.height()));
}


void CodeEditor::wheelEvent(QWheelEvent* e)
{

	if (e->modifiers() == Qt::ControlModifier)
	{
		if(e->angleDelta().y() > 0 ) 
		{
			zoomIn();
		} 
		else 
		{
			zoomOut();
		}
		m_lineNumberArea->updateFont(this->font());
		updateLineNumberAreaWidth();
		e->accept();
	} 
	else 
	{
		QPlainTextEdit::wheelEvent(e);
	}
}

void CodeEditor::updateLineNumberAreaWidth()
{

	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect& rect, int dy)
{

	if(dy!=0){
		m_lineNumberArea->scroll(0, dy);
	} 
	else 
	{
		m_lineNumberArea->update(0, rect.y(), 
						   		 m_lineNumberArea->width(), rect.height());
	} 
	if (rect.contains(viewport()->rect()))
	{
		updateLineNumberAreaWidth();
	}
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 4;
    int space = 5 + fontMetrics().horizontalAdvance(QLatin1Char('1'))*digits;
	return space;
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor(Qt::white).lighter(110));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) 
	{
        if (block.isVisible() && bottom >= event->rect().top()) 
		{
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, m_lineNumberArea->width() - 3, 
							 fontMetrics().height(), Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void LineNumberArea::updateFont(const QFont& font)
{

	setFont(font);
}
