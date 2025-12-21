#include <QSyntaxHighlighter>
#include "MyEditor.h"
#include "CppHighlighterFactory.h"
#include "LanguageHighlighterStrategy.h"
#include "PythonHighlighterFactory.h"
#include "Language.h"
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QPainter>

MyEditor::MyEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setFont(QFont("Consolas", 11));
    setTabStopDistance(24);

    lineNumberArea = new LineNumberArea(this);

    connect(this, &MyEditor::blockCountChanged, this, &MyEditor::updateLineNumberAreaWidth);
    connect(this, &MyEditor::updateRequest, this, &MyEditor::updateLineNumberArea);
    connect(this, &MyEditor::cursorPositionChanged, this, &MyEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void MyEditor::setLanguage(Language lang) {
    switch (lang) {
        case Language::Cpp:
            setHighlighter(std::make_unique<CppHighlighterFactory>());
            break;
        case Language::Python:
            setHighlighter(std::make_unique<PythonHighlighterFactory>());
            break;
        default:
            setHighlighter(nullptr);
            break;
    }
}

void MyEditor::setHighlighter(std::unique_ptr<HighlighterFactory> factory) {
    highlighter.reset();

    if (factory) {
        highlighter = factory->createStrategy(document());
    }
}

int MyEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void MyEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void MyEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void MyEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void MyEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::gray).lighter(150);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void MyEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::GlobalColor::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

// void MyEditor::keyPressEvent(QKeyEvent* event) {
//     QPlainTextEdit::keyPressEvent(event);
// }