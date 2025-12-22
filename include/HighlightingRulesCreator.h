#pragma once

#include <QVector>
#include <QRegularExpression>
#include <QTextCharFormat>

struct HighlightingRule {
    QRegularExpression pattern;
    QTextCharFormat format;
};

class HighlightingRulesCreator {
public:
    virtual ~HighlightingRulesCreator() = default;
    virtual QVector<HighlightingRule> createRules() const = 0;
};