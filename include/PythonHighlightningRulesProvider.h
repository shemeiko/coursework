#pragma once

#include "HighlightningRulesProvider.h"
#include "PythonSpecFactory.h"

class PythonHighlightingRulesProvider : public HighlightingRulesProvider {
public:
    explicit PythonHighlightingRulesProvider(PythonSpecFactory spec)
        : langSpec(std::move(spec)) {}
    QVector<HighlightingRule> createRules() const override;
protected:
    PythonSpecFactory langSpec;
};