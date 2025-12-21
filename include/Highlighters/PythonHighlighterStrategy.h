#pragma once

#include "LanguageHighlighterStrategy.h"

class PythonHighlighterStrategy : public LanguageHighlighterStrategy {
    Q_OBJECT

public:
    explicit PythonHighlighterStrategy(QTextDocument *parent = nullptr);
};