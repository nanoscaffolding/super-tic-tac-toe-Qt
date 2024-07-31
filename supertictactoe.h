#ifndef SUPERTICTACTOE_H
#define SUPERTICTACTOE_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <vector>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <stack>

class SuperTicTacToe : public QWidget {
    Q_OBJECT

public:
    SuperTicTacToe(QWidget *parent = nullptr);


private slots:
    void handleButtonClick(int localRow, int localCol, int globalRow, int globalCol);
    void switchPlayer();
    void checkForWin();
    void highlightActiveLocalBoard(int globalRow, int globalCol);
    void restartGame();
    void resignGame();
    void undoMove();


private:
    int boardSize;

    QLabel *turnLabel;

    QPushButton *restartButton;
    QPushButton *resignButton;
    QPushButton *undoButton;


    QGridLayout *globalGridLayout;
    QVBoxLayout *rightPanelLayout;
    QHBoxLayout *mainLayout;
    std::vector<std::vector<QGridLayout*>> localGridLayouts;
    std::vector<std::vector<std::vector<std::vector<QPushButton*>>>> boardButtons;
    std::vector<std::vector<std::vector<std::vector<int>>>> boardState;
    std::vector<std::vector<int>> globalBoardState;
    int currentPlayer;
    int nextLocalBoard;
    QGroupBox *activeLocalBoardFrame;

    struct Move{
        int localRow;
        int localCol;
        int globalRow;
        int globalCol;
        int player;
        int nextLocalBoard;
    };
    std::stack<Move> moveStack;

    void createBoard();
    void updateTurnLabel();
};

#endif // SUPERTICTACTOE_H
