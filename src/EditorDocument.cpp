#include "EditorDocument.h"
#include "DocumentState.h"
#include "states/SavedState.h"
#include "states/UnSavedState.h"
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

EditorDocument::EditorDocument()
{
};

void EditorDocument::changeState(std::unique_ptr<DocumentState> state) {
    m_state = std::move(state);
}

bool EditorDocument::isSaved() const {
    return m_state ? m_state->isSaved() : true;
}

void EditorDocument::loadFromFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text))
        return;

    QTextStream in(&file);
    e_text = in.readAll();
    file.close();

    setFilePath(path);
    changeState(std::make_unique<SavedState>());
}

bool EditorDocument::saveToFile(const QString &path) {
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << text();
    file.close();

    setFilePath(path);
    changeState(std::make_unique<SavedState>());
    return true;
}

void EditorDocument::setFilePath(const QString& path) {
    filePath = path;
    getFileType();
}

void EditorDocument::onModified(const QString &text) {
    setText(text);

    if (isSaved())
        changeState(std::make_unique<UnSavedState>());
}

void EditorDocument::getFileType() {
    QString ext = QFileInfo(filePath).suffix().toLower();

    if (ext == "txt")
        type = Language::PlainText;
    else if (ext == "cpp" || ext == "h" || ext == "hpp")
        type = Language::Cpp;
    else if (ext == "py")
        type = Language::Python;
    else
        type = Language::None;
}