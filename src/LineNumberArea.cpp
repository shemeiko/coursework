#include "LineNumberArea.h"
#include "MyEditor.h"

LineNumberArea::LineNumberArea(MyEditor *editor)
    : QWidget(editor), editor(editor)
{}

QSize LineNumberArea::sizeHint() const {
    return QSize(editor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    editor->lineNumberAreaPaintEvent(event);
}