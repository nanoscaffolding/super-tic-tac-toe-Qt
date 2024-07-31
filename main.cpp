#include "supertictactoe.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SuperTicTacToe w;
    w.show();
    return a.exec();
}
