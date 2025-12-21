#pragma once

#include <QVector>
#include <QRegularExpression>
#include <QTextCharFormat>

struct HighlightingRule {
    QRegularExpression pattern;
    QTextCharFormat format;
};

class HighlightingRulesProvider {
public:
    virtual ~HighlightingRulesProvider() = default;
    virtual QVector<HighlightingRule> createRules() const = 0;
};