#include "CppHighlightingRulesCreator.h"

QVector<HighlightingRule> CppHighlightingRulesCreator::createRules() const {
    QVector<HighlightingRule> rules;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);

    for (const QString& kw : langSpec.keywords()) {
        rules.append({
            QRegularExpression("\\b" + QRegularExpression::escape(kw) + "\\b"),
            keywordFormat
        });
    }

    QTextCharFormat typeFormat;
    typeFormat.setForeground(Qt::darkCyan);
    typeFormat.setFontWeight(QFont::Bold);

    for (const QString& kw : langSpec.types()) {
        rules.append({
            QRegularExpression("\\b" + QRegularExpression::escape(kw) + "\\b"),
            typeFormat
        });
    }

    QTextCharFormat statementFormat;
    statementFormat.setForeground(Qt::darkMagenta);
    statementFormat.setFontWeight(QFont::Bold);

    for (const QString& kw : langSpec.statements()) {
        rules.append({
            QRegularExpression("\\b" + QRegularExpression::escape(kw) + "\\b"),
            statementFormat
        });
    }

    QTextCharFormat preprocessorFormat;
    preprocessorFormat.setForeground(Qt::darkGreen);
    preprocessorFormat.setFontWeight(QFont::Bold);

    for (const QString& kw : langSpec.preprocessor_directives()) {
        rules.append({
            QRegularExpression(
                "^\\s*#\\s*" + QRegularExpression::escape(kw) + "\\b"
            ),
            preprocessorFormat
        });
    }

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::gray);
    commentFormat.setFontItalic(true);

    // single-line comment
    rules.append({
        QRegularExpression("//[^\n]*"),
        commentFormat
    });

    // multi-line comment
    rules.append({
        QRegularExpression("/\\*[\\s\\S]*?\\*/"),
        commentFormat
    });

    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor(206, 145, 120));

    rules.append({
        QRegularExpression(R"("([^"\\]|\\.)*")"),
        stringFormat
    });

    rules.append({
        QRegularExpression(R"('([^'\\]|\\.)*')"),
        stringFormat
    });

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor(181, 206, 168));

    rules.append({
        QRegularExpression(R"(\b\d+(\.\d+)?([eE][+-]?\d+)?\b)"),
        numberFormat
    });

    return rules;
}
