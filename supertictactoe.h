#ifndef SUPERTICTACTOE_H
#define SUPERTICTACTOE_H

#include<QWidget>
#include<QGridLayout>
#include<QPushButton>
#include<vector>
#include<QGroupBox>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QComboBox>
#include<stack>
#include<QTimer>
#include<QTime>

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
    void changeBoardSize(int index); //Slot to change board size

private:
    QGridLayout *globalGridLayout;
    QVBoxLayout *rightPanelLayout;
    QLabel *turnLabel;
    QPushButton *restartButton;
    QPushButton *resignButton;
    QPushButton *undoButton;    //Button to return t the previous move
    QComboBox *boardSizeSelect; //Select button for board size
    QHBoxLayout *mainLayout;
    std::vector<std::vector<QGridLayout*>> localGridLayouts;
    std::vector<std::vector<std::vector<std::vector<QPushButton*>>>> boardButtons;
    std::vector<std::vector<std::vector<std::vector<int>>>> boardState;
    std::vector<std::vector<int>> globalBoardState;
    int currentPlayer;
    int nextLocalBoard;
    QGroupBox *activeLocalBoardFrame;
    int boardSize; // Current board size

    struct Move{
        int localRow;
        int localCol;
        int globalRow;
        int globalCol;
        int player;
        int nextLocalBoard;
    };

    std::stack<Move> moveStack; //Used in undo sequence

    void createBoard();
    void updateTurnLabel();
    void clearBoard();
};

#endif // SUPERTICTACTOE_H
