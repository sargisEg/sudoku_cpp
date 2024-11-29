#include "scoreswindow.h"
#include "welcomewindow.h"
#include "gamewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    WelcomeWindow welcomeWindow;
    welcomeWindow.show();
    ScoresWindow scoresWindow;
    GameWindow *gameWindow;

    QObject::connect(&welcomeWindow, &WelcomeWindow::scores, [&welcomeWindow, &scoresWindow]() {
        welcomeWindow.hide();
        scoresWindow.show();
        scoresWindow.initData();
    });


    QObject::connect(&scoresWindow, &ScoresWindow::exit, [&welcomeWindow, &scoresWindow]() {
        welcomeWindow.show();
        scoresWindow.close();
    });

    QObject::connect(&welcomeWindow, &WelcomeWindow::start, [&welcomeWindow, &gameWindow](int difficulty, QString name) {
        gameWindow = new GameWindow(difficulty, name);
        welcomeWindow.hide();
        gameWindow->show();

        QObject::connect(gameWindow, &GameWindow::exit, [&welcomeWindow, &gameWindow]() {
            welcomeWindow.show();
            gameWindow->close();
        });

        QObject::connect(gameWindow, &GameWindow::solvedSignal, [&welcomeWindow, &gameWindow]() {
            welcomeWindow.show();
            gameWindow->close();
        });
    });

    return a.exec();
}
