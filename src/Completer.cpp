#include "Completer.h"
#include "Trie.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QStringList>
#include <QRect>
#include <QPoint>
#include <QDebug>

Completer::Completer(QPlainTextEdit *editor)
    : QObject(editor),
      editor(editor),
      trie(new Trie)
{
    popup = new QListWidget(editor);
    popup->setWindowFlags(Qt::ToolTip);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->setMouseTracking(true);
    popup->setAttribute(Qt::WA_ShowWithoutActivating);
    popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(popup, &QListWidget::itemClicked,
            this, &Completer::insertCompletion);
}

Completer::~Completer() {
    delete trie;
}

void Completer::add(const QString& word) {
    if (!trie->Search(word))
        trie->Insert(word);
}

void Completer::show(const QString& word) {
    QStringList items = trie->AutoComplete(word);
    if (items.isEmpty()) {
        hide();
        return;
    }

    for (QString& string : items) {
        qDebug() << string;
    }

    popup->clear();
    popup->addItems(items);
    popup->setCurrentRow(0);

    QRect cr = editor->cursorRect();
    QPoint pos = editor->mapToGlobal(cr.bottomLeft());

    popup->move(pos);
    popup->show();
}

void Completer::hide() {
    popup->hide();
}

bool Completer::isVisible() const {
    return popup->isVisible();
}

void Completer::insertCompletion(QListWidgetItem *item) {
    QTextCursor c = editor->textCursor();
    c.select(QTextCursor::WordUnderCursor);
    c.removeSelectedText();
    c.insertText(item->text());
    editor->setTextCursor(c);
    hide();
}

void Completer::selectNext()
{
    int row = popup->currentRow();
    if (row < popup->count() - 1)
        popup->setCurrentRow(row + 1);
}

void Completer::selectPrev()
{
    int row = popup->currentRow();
    if (row > 0)
        popup->setCurrentRow(row - 1);
}

void Completer::confirm()
{
    auto *item = popup->currentItem();
    if (!item)
        return;

    QTextCursor c = editor->textCursor();
    c.select(QTextCursor::WordUnderCursor);
    c.removeSelectedText();
    c.insertText(item->text());
    editor->setTextCursor(c);
    hide();
}