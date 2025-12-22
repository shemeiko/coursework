#pragma once

#include "HighlightingRulesCreator.h"
#include "CppSpecFactory.h"

class CppHighlightingRulesCreator : public HighlightingRulesCreator {
public:
    explicit CppHighlightingRulesCreator(CppSpecFactory spec)
        : langSpec(std::move(spec)) {}
    QVector<HighlightingRule> createRules() const override;
protected:
    CppSpecFactory langSpec;
};