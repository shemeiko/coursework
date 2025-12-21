#pragma once

#include "HighlighterFactory.h"
#include "Highlighters/PythonHighlighterStrategy.h"

class PythonHighlighterFactory : public HighlighterFactory {
public:
    std::unique_ptr<LanguageHighlighterStrategy> createStrategy(QTextDocument *doc) override {
        return std::make_unique<PythonHighlighterStrategy>(doc);
    }
};