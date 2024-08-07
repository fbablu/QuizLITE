#ifndef QUIZLITE_MAINWINDOW_H
#define QUIZLITE_MAINWINDOW_H

#include "../Menu/Shortcuts.h"
#include "../User/UserSession.h"
#include "AddQuestionsPage.h"
#include "CreateSetPage.h"
#include "EnterSetPage.h"
#include "FlashcardPage.h"
#include "InverseMCPage.h"
#include "LibraryPage.h"
#include "MCPage.h"
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <iostream>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QApplication;
class QWidget;
class QVBoxLayout;
class QStackedWidget;
class QPushButton;
class QLineEdit;
class QListWidget;
QT_END_NAMESPACE

class CreateSetPage;
class LibraryPage;
class AddQuestionsPage;
class EnterSetPage;
class MCPage;
class InverseMCPage;
class FlashcardPage;
class Shortcuts;

/**
 * @class MainWindow
 * @brief The main window for the QuizLITE application.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief Constructor for MainWindow.
     *
     * @param parent The parent widget.
     */
    explicit MainWindow(QWidget* parent = nullptr);

public slots:
    /**
     * @brief Shows the CreateSetPage.
     */
    void showCreateSetPage();

    /**
     * @brief Shows the AddQuestionsPage for the specified set.
     *
     * @param setName The name of the set to add questions to.
     */
    void showAddQuestionsPage(const QString& setName);

    /**
     * @brief Adds a question and answer to the current set.
     *
     * @param question The question text.
     * @param answer The answer text.
     */
    void addToSet(const QString& question, const QString& answer);

    /**
     * @brief Finalizes the current set and saves it.
     */
    void finishSet();

    /**
     * @brief Opens the specified set in the EnterSetPage.
     *
     * @param setName The name of the set to open.
     */
    void openSet(const QString& setName);

    /**
     * @brief Shows the LibraryPage.
     */
    void showLibraryPage();

    /**
     * @brief Handles the deletion of a set.
     *
     * @param setName The name of the set to delete.
     */
    void handleDeleteSet(const QString& setName);

    void updateMenus();

    void showMCPage(const QString& setName);
    void showInverseMCPage(const QString& setName);
    void showFlashcardPage(const QString& setName);

    void connectSignalsAndSlots();

    void showEnterSetPage();

private:
    QStackedWidget* pageStack;
    LibraryPage* libraryPage;
    CreateSetPage* createSetPage;
    AddQuestionsPage* addQuestionsPage;
    EnterSetPage* enterSetPage;
    MCPage* mcPage;
    InverseMCPage* inverseMCPage;
    FlashcardPage* flashcardPage;
    QVector<QPair<QString, QString>> currentSetQA;
    QString currentSetName;
    QMap<QString, QListWidget*> setWidgets;
    QPushButton backToLibrary;
    Shortcuts* shortcuts;
};

#endif // QUIZLITE_MAINWINDOW_H
