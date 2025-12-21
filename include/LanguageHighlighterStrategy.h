#pragma once

#include "HighlightningRulesProvider.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QVector>
#include <memory>

class LanguageHighlighterStrategy : public QSyntaxHighlighter {
protected:
    QVector<HighlightingRule> rules;

public:
    explicit LanguageHighlighterStrategy(
        QTextDocument* parent,
        std::unique_ptr<HighlightingRulesProvider> factory
    )
        : QSyntaxHighlighter(parent),
          rules(factory->createRules())
    {}

protected:
    void highlightBlock(const QString& text);
};