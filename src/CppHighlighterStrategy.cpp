#include "Highlighters/CppHighlighterStrategy.h"
#include "CppHighlightingRulesCreator.h"

CppHighlighterStrategy::CppHighlighterStrategy(QTextDocument *parent)
    : LanguageHighlighterStrategy(
        parent,
        std::make_unique<CppHighlightingRulesCreator>(
            CppSpecFactory{}
        )
    )
{}