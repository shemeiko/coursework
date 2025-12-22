#pragma once

#include "HighlightingRulesCreator.h"
#include "PythonSpecFactory.h"

class PythonHighlightingRulesCreator : public HighlightingRulesCreator {
public:
    explicit PythonHighlightingRulesCreator(PythonSpecFactory spec)
        : langSpec(std::move(spec)) {}
    QVector<HighlightingRule> createRules() const override;
protected:
    PythonSpecFactory langSpec;
};