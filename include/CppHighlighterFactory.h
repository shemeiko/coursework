#pragma once

#include "HighlighterFactory.h"
#include "Highlighters/CppHighlighterStrategy.h"

class CppHighlighterFactory : public HighlighterFactory {
public:
    std::unique_ptr<LanguageHighlighterStrategy> createStrategy(QTextDocument *doc) override {
        return std::make_unique<CppHighlighterStrategy>(doc);
    }
};