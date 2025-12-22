#pragma once
#include <QWidget>
#include <QPlainTextEdit>
#include "LineNumberArea.h"

class LanguageHighlighterStrategy;
class QKeyEvent;

enum class Language;

class MyEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit MyEditor(QWidget *parent = nullptr);

    void setLanguage(Language lang);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;
    // void MyEditor::keyPressEvent(QKeyEvent* event) override;

private:
    void setHighlighter(std::unique_ptr<LanguageHighlighterStrategy> strategy);
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

    std::unique_ptr<LanguageHighlighterStrategy> highlighter;
    LineNumberArea *lineNumberArea;
};