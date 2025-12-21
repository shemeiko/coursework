#pragma once

#include "HighlightningRulesProvider.h"
#include "CppSpecFactory.h"

class CppHighlightingRulesProvider : public HighlightingRulesProvider {
public:
    explicit CppHighlightingRulesProvider(CppSpecFactory spec)
        : langSpec(std::move(spec)) {}
    QVector<HighlightingRule> createRules() const override;
protected:
    CppSpecFactory langSpec;
};