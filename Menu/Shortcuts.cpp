#include "Shortcuts.h"
#include <QKeySequence>

Shortcuts::Shortcuts(MainWindow *mainWindow, EnterSetPage *enterSetPage) :
QObject(mainWindow),
mainWindow(mainWindow),
enterSetPage(enterSetPage) {}




void Shortcuts::setupMenus(QMenuBar *menuBar) {
    setMenu = menuBar->addMenu(tr("&Set"));
    setMenu->addAction(tr("&New Set"), QKeySequence::New, mainWindow, &MainWindow::showCreateSetPage);

    studyMenu = menuBar->addMenu(tr("&Study"));
    studyMenu->addAction(tr("&Multiple Choice"), Qt::Key_M,  enterSetPage, &EnterSetPage::openMCPage);
    studyMenu->addAction(tr("&Inverse Multiple Choice"), Qt::Key_I, this, [this](){enterSetPage->openInversePage();

    });
    studyMenu->addAction(tr("&Flashcards"), Qt::Key_F, this, [this](){ enterSetPage->openFlashcardsPage(); });

}

void Shortcuts::updateMenus(QWidget *currentWidget) {
    bool isLibraryPage = (currentWidget == mainWindow->findChild<QWidget*>("libraryPage"));
    bool isEnterSetPage = (currentWidget == enterSetPage);

    mainWindow->menuBar()->actions().at(0)->menu()->actions().at(0)->setEnabled(isLibraryPage);
    studyMenu->setEnabled(isEnterSetPage);
}

void Shortcuts::setCurrentSetName(const QString &setName)
{
    m_currentSetName = setName;
}

QString Shortcuts::currentSetName() const
{
    return m_currentSetName;
}