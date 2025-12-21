#pragma once

#include <QStringList>

class LanguageSpecFactory {
public:
    virtual ~LanguageSpecFactory() = default;

    virtual QStringList keywords() const = 0;
    virtual QStringList statements() const = 0;
    virtual QStringList types() const = 0;
    virtual QStringList preprocessor_directives() const = 0;
};