#pragma once

#include "HighlightingRulesCreator.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QVector>

class LanguageHighlighterStrategy : public QSyntaxHighlighter {
protected:
    QVector<HighlightingRule> rules;

public:
    explicit LanguageHighlighterStrategy(
        QTextDocument* parent,
        std::unique_ptr<HighlightingRulesCreator> factory
    )
        : QSyntaxHighlighter(parent),
          rules(factory->createRules())
    {}

protected:
    void highlightBlock(const QString& text) override final;
};