#include "Trie.h"
#include <QStringList>

int Trie::getAlphaIndex(QChar ch) const {
    if (ch >= 'a' && ch <= 'z')
        return ch.unicode() - 'a';
    if (ch >= 'A' && ch <= 'Z')
        return 26 + (ch.unicode() - 'A');
    if (ch == '_') return 52;
    if (ch == '-') return 53;
    return -1;
}

QChar Trie::indexToChar(int idx) const {
    if (idx < 26) return QChar('a' + idx);
    if (idx < 52) return QChar('A' + (idx - 26));
    if (idx == 52) return '_';
    return '-';
}

bool Trie::hasChildren(const Node* node) const {
    for (Node* p : node->children)
        if (p) return true;
    return false;
}

void Trie::Insert(const QString& key) {
    Node* curr = root;

    for (int i = 0; i < key.length(); i++) {
        int ch_num = getAlphaIndex(key[i]);
        
        if (ch_num < 0 || ch_num >= SYMBOLS_COUNT) return;

        if (!curr->children[ch_num])
            curr->children[ch_num] = new Node;

        curr = curr->children[ch_num];
    }
    curr->terminal = true;
}

void Trie::Delete(const QString& key) {
    if (key.isEmpty()) return;

    DeleteHelper(root, key, 0);
}

bool Trie::DeleteHelper(Node* node, const QString& key, int depth) {
    if (!node) return false;

    if (depth == key.size()) {
        if (!node->terminal)
            return false;

        node->terminal = false;

        return !hasChildren(node);
    }

    int ch_num = getAlphaIndex(key[depth]);
    if (ch_num == -1 || !node->children[ch_num])
        return false;

    bool nextDelete = DeleteHelper(node->children[ch_num], key, depth + 1);
    if (nextDelete) {
        delete node->children[ch_num];
        node->children[ch_num] = nullptr;
    }

    return !node->terminal && !hasChildren(node);
}

Node* Trie::Search(const QString& key) {
    Node* curr = root;

    for (QChar c : key) {
        int ch_num = getAlphaIndex(c);
        
        if (ch_num < 0 || ch_num >= SYMBOLS_COUNT) return nullptr;

        if (!curr->children[ch_num]) return nullptr;
        curr = curr->children[ch_num];
    }

    return curr;
}

void Trie::DFS(Node* node, QString& current, QStringList& result) {
    if (node->terminal)
        result.push_back(current);

    for (int i = 0; i < SYMBOLS_COUNT; i++) {
        if (node->children[i]) {
            current.push_back(indexToChar(i));
            DFS(node->children[i], current, result);
            current.chop(1);
        }
    }
}

QStringList Trie::AutoComplete(const QString& prefix) {
    QStringList result;
    Node* node = Search(prefix);

    if (!node)
        return result;

    QString current = prefix;
    DFS(node, current, result);

    return result;
}