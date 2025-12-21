#include "LanguageHighlighterStrategy.h"


void LanguageHighlighterStrategy::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : rules) {
        auto it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            auto match = it.next();
            setFormat(match.capturedStart(),
                      match.capturedLength(),
                      rule.format);
        }
    }
}