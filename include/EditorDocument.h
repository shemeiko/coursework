#pragma once

#include <memory>
#include <QString>
#include "Language.h"

class DocumentState;

class EditorDocument {

public:
    explicit EditorDocument();

    bool saveToFile(const QString &path);
    void loadFromFile(const QString& path);

    bool isSaved() const;

    void setText(QString text) { e_text = text; };
    QString text() { return e_text; }
    void changeState(std::unique_ptr<DocumentState> state);
    void onModified(const QString &text);
    void getFileType();
    Language getType() { return type; }
    void setFilePath(const QString& path);
    QString getFilePath() { return filePath; }
    bool isPreInit() { return preInit; }
    void endPreInit() { preInit = false; }

private:
    std::unique_ptr<DocumentState> m_state;
    Language type;
    QString e_text;
    QString filePath;
    bool preInit = true;
};