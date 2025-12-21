#pragma once

class QTextDocument;
class LanguageHighlighterStrategy;

class HighlighterFactory {
public:
    virtual ~HighlighterFactory() = default;

    virtual std::unique_ptr<LanguageHighlighterStrategy> createStrategy(QTextDocument *doc) = 0;
};