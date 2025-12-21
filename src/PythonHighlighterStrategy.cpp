#include "Highlighters/PythonHighlighterStrategy.h"
#include "PythonHighlightningRulesProvider.h"

PythonHighlighterStrategy::PythonHighlighterStrategy(QTextDocument *parent)
    : LanguageHighlighterStrategy(
        parent,
        std::make_unique<PythonHighlightingRulesProvider>(
            PythonSpecFactory{}
        )
    )
{}