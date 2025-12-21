#include "PythonHighlightningRulesProvider.h"

QVector<HighlightingRule> PythonHighlightingRulesProvider::createRules() const {
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

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::gray);
    commentFormat.setFontItalic(true);

    rules.append({
        QRegularExpression("#[^\n]*"),
        commentFormat
    });

    QTextCharFormat decoratorFormat;
    decoratorFormat.setForeground(QColor(197, 134, 192));
    decoratorFormat.setFontWeight(QFont::Bold);

    rules.append({
        QRegularExpression(R"(@[A-Za-z_]\w*)"),
        decoratorFormat
    });

    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor(206, 145, 120));

    // single & double quoted
    rules.append({
        QRegularExpression(R"('([^'\\]|\\.)*')"),
        stringFormat
    });
    rules.append({
        QRegularExpression(R"("([^"\\]|\\.)*")"),
        stringFormat
    });

    // triple-quoted strings (docstrings)
    rules.append({
        QRegularExpression(R"('''[\s\S]*?''')"),
        stringFormat
    });
    rules.append({
        QRegularExpression(R"("""[\s\S]*?""")"),
        stringFormat
    });

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor(181, 206, 168));

    rules.append({
        QRegularExpression(
            R"(\b\d+(\.\d+)?([eE][+-]?\d+)?j?\b)"
        ),
        numberFormat
    });

    QTextCharFormat builtinFormat;
    builtinFormat.setForeground(QColor(220, 220, 170));

    const QStringList builtins = {
        "print", "len", "range", "enumerate",
        "map", "filter", "zip",
        "open", "type", "isinstance",
        "dir", "help", "id", "abs", "sum"
    };

    for (const QString& fn : builtins) {
        rules.append({
            QRegularExpression("\\b" + fn + "\\b"),
            builtinFormat
        });
    }

    return rules;
}