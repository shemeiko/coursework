#pragma once

#include <QObject>

class QPlainTextEdit;
class QListWidget;
class QListWidgetItem;
class Trie;

class Completer : public QObject {
    Q_OBJECT
public:
    explicit Completer(QPlainTextEdit *editor);
    ~Completer();

    void add(const QString& word);
    void show(const QString& word);
    void hide();

    void selectNext();
    void selectPrev();
    void confirm();

    bool isVisible() const;

private slots:
    void insertCompletion(QListWidgetItem *item);

private:
    Trie* trie;
    QPlainTextEdit *editor;
    QListWidget *popup;
};