#pragma once
#include <QMainWindow>
#include <QWidget>

class MyEditor;
class QTabWidget;
class QDockWidget;
class QCloseEvent;
class QFileSystemModel;
class QTreeView;
class EditorDocument;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void AddNewTab(EditorDocument* document);
    void openFile();
    void openFolder();
    void saveFile();
    void saveFileAs();
    MyEditor* currentEditor() const;
    EditorDocument* getDocument(int idx) const;
    void updateTabTitle(int tabIndex);
    void onFileRenamed(const QString &dirPath, const QString &oldName, const QString &newName);
    void closeTab(int index);
    void closeCurrentTab();
    void onDockContextMenu(const QPoint &point);
    void createNewFile(const QModelIndex &index);
    void createNewFolder(const QModelIndex &index);
    void activateFile(const QModelIndex &index, bool openIfMissing);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initMenu();
    void initTabs();
    void initDock();
    
    QTreeView *tree;
    QFileSystemModel *model;
    QTabWidget *tabs;
    QDockWidget *dock;
};