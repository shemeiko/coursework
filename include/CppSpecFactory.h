#pragma once

#include "LanguageSpecFactory.h"
#include <QStringList>

class CppSpecFactory : public LanguageSpecFactory {
public:
    QStringList keywords() const override {
        return {
            "alignas", "alignof", "asm", "auto", "constexpr", "consteval",
            "constinit", "decltype", "explicit", "export", "friend",
            "inline", "mutable", "namespace", "noexcept", "operator",
            "register", "requires", "static", "thread_local", "typedef",
            "using", "virtual", "volatile", "class", "struct", "union",
            "public", "protected", "private", "template", "typename", "concept",
            "new", "delete", "extern"
        };
    }

    QStringList statements() const override {
        return {
            "if", "else", "switch", "case", "default",
            "for", "while", "do",
            "break", "continue", "return", "goto",
            "try", "catch", "throw",
            "co_await", "co_yield", "co_return"
        };
    }

    QStringList types() const override {
        return {
            "void", "bool", "char", "wchar_t", "char8_t",
            "char16_t", "char32_t",
            "short", "int", "long", "signed", "unsigned",
            "float", "double", "long double",
            "int8_t", "int16_t", "int32_t", "int64_t",
            "uint8_t", "uint16_t", "uint32_t", "uint64_t",
            "size_t", "ptrdiff_t", "nullptr_t"
        };
    }

    QStringList preprocessor_directives() const override {
        return {
            "include", "define", "undef",
            "if", "ifdef", "ifndef", "elif", "else", "endif",
            "pragma", "error", "warning", "line"
        };
    }
};