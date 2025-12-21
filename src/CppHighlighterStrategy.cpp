#include "Highlighters/CppHighlighterStrategy.h"
#include "CppHighlightningRulesProvider.h"

CppHighlighterStrategy::CppHighlighterStrategy(QTextDocument *parent)
    : LanguageHighlighterStrategy(
        parent,
        std::make_unique<CppHighlightingRulesProvider>(
            CppSpecFactory{}
        )
    )
{}