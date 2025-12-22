#include "Highlighters/PythonHighlighterStrategy.h"
#include "PythonHighlightingRulesCreator.h"

PythonHighlighterStrategy::PythonHighlighterStrategy(QTextDocument *parent)
    : LanguageHighlighterStrategy(
        parent,
        std::make_unique<PythonHighlightingRulesCreator>(
            PythonSpecFactory{}
        )
    )
{}