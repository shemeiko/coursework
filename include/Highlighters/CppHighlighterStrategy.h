#pragma once

#include "LanguageHighlighterStrategy.h"

class CppHighlighterStrategy : public LanguageHighlighterStrategy {
    Q_OBJECT

public:
    explicit CppHighlighterStrategy(QTextDocument *parent = nullptr);
};