#pragma once

#include "LanguageSpecFactory.h"
#include <QStringList>

class PythonSpecFactory : public LanguageSpecFactory {
public:
    QStringList keywords() const override {
        return {
            "False", "None", "True",
            "and", "as", "assert", "async", "await",
            "break", "class", "continue",
            "def", "del",
            "elif", "else", "except",
            "finally", "for", "from",
            "global",
            "if", "import", "in", "is",
            "lambda",
            "nonlocal", "not",
            "or",
            "pass",
            "raise", "return",
            "try",
            "while", "with", "yield"
        };
    }

    QStringList statements() const override {
        return {
            "if", "elif", "else",
            "for", "while",
            "try", "except", "finally",
            "with"
        };
    }

    QStringList types() const override {
        return {
            "int", "float", "complex",
            "bool", "str", "bytes",
            "list", "tuple", "set", "dict",
            "None"
        };
    }

    QStringList preprocessor_directives() const override {
        return {
            "!", "coding"
        };
    }
};
