#pragma once

class QString;
#include <QStringList>

#define SYMBOLS_COUNT 53

struct Node {
    std::vector<Node*> children;
    bool terminal = false;

    Node() : children(SYMBOLS_COUNT, nullptr) {}

    ~Node() {
        for (int i = 0; i < children.size(); i++) delete children[i];
    }
};

class Trie {
    Node* root;

    int getAlphaIndex(QChar ch) const;
    QChar indexToChar(int idx) const;
    bool hasChildren(const Node* node) const;

public:
    Trie() : root(new Node) {}
    ~Trie() { delete root; }
    void Insert(const QString& key);
    void Delete(const QString& key);
    bool DeleteHelper(Node* node, const QString& key, int depth);
    Node* Search(const QString& key);
    void DFS(Node* node, QString& current, QStringList& result);
    QStringList AutoComplete(const QString& prefix);
};