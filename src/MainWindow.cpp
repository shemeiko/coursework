#include "MainWindow.h"
#include "MyEditor.h"
#include "EditorDocument.h"
#include "states/SavedState.h"
#include "states/UnSavedState.h"
#include <QApplication>
#include <QMenuBar>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QTabBar>
#include <QDockWidget>
#include <QAction>
#include <QMessageBox>
#include <QTreeView>
#include <QHeaderView>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QInputDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
{
    setWindowTitle("MyEditor");
    resize(900, 400);
    
    initMenu();
    initTabs();
    initDock();

    model = new QFileSystemModel;
    model->setReadOnly(false);
    tree = new QTreeView;
    tree->setModel(model);
    tree->setRootIndex(model->index(QDir::currentPath()));

    tree->setEditTriggers(QAbstractItemView::EditKeyPressed);
    
    tree->hideColumn(1);
    tree->hideColumn(2);
    tree->hideColumn(3);
    
    tree->header()->hide();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    tree->setExpandsOnDoubleClick(true);

    tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree, &QTreeView::customContextMenuRequested,
            this, &MainWindow::onDockContextMenu);

    connect(model, &QFileSystemModel::fileRenamed,
        this, &MainWindow::onFileRenamed);
        
    dock->setWidget(tree);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dock);
    dock->hide();

    QAction *toggleDock = dock->toggleViewAction();
    toggleDock->setShortcut(QKeySequence("Ctrl+B"));
    
    addAction(toggleDock);
    setCentralWidget(tabs);

    connect(tree, &QTreeView::clicked, this,
        [this](const QModelIndex &index) {
            activateFile(index, false);
        });

    connect(tree, &QTreeView::doubleClicked, this,
        [this](const QModelIndex &index) {
            activateFile(index, true);
        });

    AddNewTab(nullptr);
}

void MainWindow::activateFile(const QModelIndex &index, bool openIfMissing)
{
    if (!index.isValid() || model->isDir(index))
        return;

    const QString filepath = model->filePath(index);

    for (int i = 0; i < tabs->count(); ++i) {
        if (auto *doc = getDocument(i);
            doc && doc->getFilePath() == filepath)
        {
            tabs->setCurrentIndex(i);
            return;
        }
    }

    if (!openIfMissing)
        return;

    auto *doc = new EditorDocument;
    doc->loadFromFile(filepath);
    AddNewTab(doc);
}

void MainWindow::initMenu() {
    QMenuBar *menu = menuBar();
    QMenu *fileMenu = menu->addMenu("File");
    QMenu *editMenu = menu->addMenu("Edit");
    QMenu *viewMenu = menu->addMenu("Format");

    QAction *newTab = fileMenu->addAction("New");
    newTab->setShortcut(QKeySequence::New);
    QAction *open = fileMenu->addAction("Open");
    open->setShortcut(QKeySequence::Open);
    QAction *openFolder = fileMenu->addAction("Open Folder");
    openFolder->setShortcut(QKeySequence("Ctrl+Shift+O"));
    QAction *save = fileMenu->addAction("Save");
    save->setShortcut(QKeySequence::Save);
    QAction *saveAs = fileMenu->addAction("Save As");
    saveAs->setShortcut(QKeySequence::SaveAs);
    fileMenu->addSeparator();
    QAction *close = fileMenu->addAction("Close");

    QAction *copy = editMenu->addAction("Copy");
    copy->setShortcut(QKeySequence::Copy);
    QAction *paste = editMenu->addAction("Paste");
    paste->setShortcut(QKeySequence::Paste);

    viewMenu->setObjectName("View");
    QAction *wrap = viewMenu->addAction("Word Wrap");
    wrap->setCheckable(true);

    connect(newTab, &QAction::triggered, [=]() { AddNewTab(nullptr); });
    connect(open, &QAction::triggered, this, &MainWindow::openFile);
    connect(openFolder, &QAction::triggered, this, &MainWindow::openFolder);
    connect(save, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAs, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(close, &QAction::triggered, QApplication::instance(), &QApplication::quit);

    QAction *closeTab = new QAction(this);
    closeTab->setShortcut(QKeySequence("Ctrl+W"));
    addAction(closeTab);

    connect(copy, &QAction::triggered, this, [this]() {
    if (MyEditor *editor = currentEditor())
        editor->copy();
    });
    connect(paste, &QAction::triggered, this, [this]() {
    if (MyEditor *editor = currentEditor())
        editor->paste();
    });

    connect(wrap, &QAction::toggled, this, [this](bool checkable) {
        for (int i = 0; i < tabs->count(); ++i) {
            if (auto *editor = qobject_cast<MyEditor*>(tabs->widget(i)))
                editor->setWordWrapMode(
                    checkable 
                        ? QTextOption::WrapAtWordBoundaryOrAnywhere 
                        : QTextOption::NoWrap);
        }
    });

    connect(closeTab, &QAction::triggered, this, &MainWindow::closeCurrentTab);
}

void MainWindow::initTabs() {
    tabs = new QTabWidget(this);

    tabs->setDocumentMode(true);
    tabs->setTabsClosable(true);
    tabs->setUsesScrollButtons(true);
    tabs->setMovable(true);
    tabs->tabBar()->setExpanding(false);

    connect(tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
}

void MainWindow::initDock() {
    dock = new QDockWidget(this);

    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    dock->setWindowTitle("File Explorer");
}

MyEditor* MainWindow::currentEditor() const {
    return qobject_cast<MyEditor*>(tabs->currentWidget());
}

EditorDocument* MainWindow::getDocument(int idx) const {
    QWidget *widget = tabs->widget(idx);
    if (!widget) return nullptr;

    return static_cast<EditorDocument*>(
        widget->property("document").value<void*>()
    );
}

void MainWindow::AddNewTab(EditorDocument* document) {
    if (!document) document = new EditorDocument;
    auto *editor = new MyEditor;

    QString filename = QFileInfo(document->getFilePath()).fileName();

    int idx = tabs->addTab(editor, filename.isEmpty() ? "Untitled" : filename);
    tabs->setCurrentIndex(idx);

    editor->setPlainText(document->text());
    editor->setLanguage(document->getType());

    editor->setProperty("document", QVariant::fromValue<void*>(document));

    QMenu *fileMenu = menuBar()->findChild<QMenu*>("View");
    QAction *firstAction = fileMenu->actions().first();

    if (firstAction->isCheckable())
        editor->setWordWrapMode(QTextOption::NoWrap);

    connect(editor, &QPlainTextEdit::textChanged,
        this, [this, editor, document]()
    {
        if (document->isPreInit()) {
            document->endPreInit();
            return;
        }

        document->onModified(editor->toPlainText());
        int idx = tabs->indexOf(editor);
        if (idx != -1) updateTabTitle(idx);
    });
}

void MainWindow::openFile() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Open File",
        QDir::currentPath(),
        "Text Files (*.txt);;All Files (*.*)"
    );

    if (filename.isEmpty()) return;

    auto *document = new EditorDocument;
    document->loadFromFile(filename);

    AddNewTab(document);
}

void MainWindow::openFolder() {
    QString dir = QFileDialog::getExistingDirectory(
        this, 
        "Open Folder", 
        QDir::currentPath(), 
        QFileDialog::ShowDirsOnly
    );

    if (dir.isEmpty()) return;

    QModelIndex index = model->setRootPath(dir);
    if (auto *tree = qobject_cast<QTreeView*>(dock->widget())) {
        tree->setRootIndex(index);
    }

    if (!dock->isVisible()) {
        dock->show();
        dock->setWindowTitle("Folder as Workspace");
    }
}

void MainWindow::saveFile() {
    EditorDocument *doc = getDocument(tabs->currentIndex());
    QString filepath = doc->getFilePath();
    if (filepath.isEmpty()) { 
        saveFileAs();
        return;
    }

    bool res = doc->saveToFile(filepath);
    if (!res) 
        QMessageBox::warning(this, "Error", "Cannot save file!");

    tabs->setTabText(tabs->currentIndex(), QFileInfo(filepath).fileName());
}

void MainWindow::saveFileAs() {
    QString filepath = QFileDialog::getSaveFileName(
        this,
        "Save File",
        QDir::homePath(),
        "Text Files (*.txt);;All Files (*.*)"
    );

    if (filepath.isEmpty()) return;

    EditorDocument *doc = getDocument(tabs->currentIndex());
    
    bool res = doc->saveToFile(filepath);
    if (!res) 
        QMessageBox::warning(this, "Error", "Cannot save file!");

    tabs->setTabText(tabs->currentIndex(), QFileInfo(filepath).fileName());
}

void MainWindow::updateTabTitle(int tabIndex) {
    QString tabText = tabs->tabText(tabIndex);
    if (!tabText.startsWith('*'))
        tabs->setTabText(tabIndex, tabText.prepend('*'));
}

void MainWindow::closeTab(int index) {
    MyEditor *editor = qobject_cast<MyEditor*>(tabs->widget(index));
    EditorDocument *doc = getDocument(index);

    if (!doc->isSaved()) {
        tabs->setCurrentIndex(index);

        auto answer = QMessageBox::question(
                this,
                "Save Changes?",
                "Do you want to save changes to " + tabs->tabText(index).remove('*') + "?",
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                QMessageBox::Save
        );

        if (answer == QMessageBox::Cancel) {
            return;
        }

        if (answer == QMessageBox::Save) {
            saveFile();

            if (!doc->isSaved())
                return;
        }
    }

    tabs->removeTab(index);

    if (editor) {
        connect(editor, &QObject::destroyed, this, [doc]() {
            if (doc) delete doc;
        });
        editor->deleteLater();
    } else {
        if (doc) delete doc;
    }
}

void MainWindow::closeCurrentTab() {
    int idx = tabs->currentIndex();
    closeTab(idx);
}

void MainWindow::onFileRenamed(
    const QString &dirPath,
    const QString &oldName,
    const QString &newName
) {
    const QString oldPath = QDir(dirPath).filePath(oldName);
    const QString newPath = QDir(dirPath).filePath(newName);

    for (int i = 0; i < tabs->count(); ++i) {
        EditorDocument *doc = getDocument(i);
        MyEditor *editor = qobject_cast<MyEditor*>(tabs->widget(i));
        if (!doc || !editor) continue;

        if (doc->getFilePath() == oldPath) {
            doc->setFilePath(newPath);

            QString tabText = tabs->tabText(i);
            bool modified = tabText.startsWith('*');
            tabs->setTabText(
                i,
                (modified ? "*" : "") + QFileInfo(newPath).fileName()
            );

            editor->setLanguage(doc->getType());
            break;
        }
    }
}

void MainWindow::onDockContextMenu(const QPoint &pos)
{
    QModelIndex index = tree->indexAt(pos);
    QPoint globalPos = tree->viewport()->mapToGlobal(pos);

    QMenu menu(this);

    QAction *renameAct = nullptr;
    QAction *deleteAct = nullptr;
    QAction *newFileAct = nullptr;
    QAction *newFolderAct = nullptr;

    if (index.isValid()) {
        tree->setCurrentIndex(index);
        renameAct = menu.addAction("Rename");
        deleteAct = menu.addAction("Delete");
    } else {
        newFileAct = menu.addAction("New File");
        newFolderAct = menu.addAction("New Folder");
    }

    QAction *chosen = menu.exec(globalPos);
    if (!chosen) return;

    if (chosen == renameAct) {
        tree->edit(tree->currentIndex());
        return;
    }

    if (chosen == deleteAct) {
        QString path = model->filePath(tree->currentIndex());
        if (model->isDir(tree->currentIndex()))
            QDir(path).removeRecursively();
        else
            QFile::remove(path);
        return;
    }

    if (chosen == newFileAct)
        createNewFile(tree->rootIndex());

    if (chosen == newFolderAct)
        createNewFolder(tree->rootIndex());
}

void MainWindow::createNewFolder(const QModelIndex &index)
{
    bool ok;
    QString name = QInputDialog::getText(
        this,
        "New Folder",
        "Folder name:",
        QLineEdit::Normal,
        "NewFolder",
        &ok
    );

    if (!ok || name.isEmpty())
        return;

    QString parentPath = model->filePath(index);
    QDir dir(parentPath);

    if (!dir.mkdir(name)) {
        QMessageBox::warning(this, "Error", "Cannot create folder");
        return;
    }

    model->setRootPath(model->rootPath());
}

void MainWindow::createNewFile(const QModelIndex &index)
{
    bool ok;
    QString name = QInputDialog::getText(
        this,
        "New File",
        "File name:",
        QLineEdit::Normal,
        "new_file.txt",
        &ok
    );

    if (!ok || name.isEmpty())
        return;

    QString parentPath = model->filePath(index);
    QFile file(parentPath + "/" + name);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Cannot create file");
        return;
    }
    file.close();

    model->setRootPath(model->rootPath());
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // for (int i = 0; i < tabs->count(); i++) {
    //     EditorDocument *doc = getDocument(i);
    //     if (!doc->isSaved()) {
    //         auto answer = QMessageBox::question(
    //             this,
    //             "Save Changes?",
    //             "Do you want to save changes to " + tabs->tabText(i).remove('*') + "?",
    //             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
    //             QMessageBox::Save
    //         );

    //         if (answer == QMessageBox::Cancel) {
    //             return;
    //         }

    //         if (answer == QMessageBox::Save) {
    //             saveFile();

    //             if (!doc->isSaved())
    //                 return;
    //         }
    //     }
    // }

    event->accept();
}