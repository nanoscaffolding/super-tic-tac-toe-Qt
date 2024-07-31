#include "SuperTicTacToe.h"
#include <QMessageBox>


SuperTicTacToe::SuperTicTacToe(QWidget *parent) : QWidget(parent), currentPlayer(1), nextLocalBoard(-1), activeLocalBoardFrame(nullptr), boardSize(3) {
    mainLayout = new QHBoxLayout(this);
    globalGridLayout = new QGridLayout();
    rightPanelLayout = new QVBoxLayout();

    // Add the board layout
    createBoard();

    // Create the right panel
    turnLabel = new QLabel("Current Turn: Player X", this);
    restartButton = new QPushButton("Restart Game", this);
    resignButton = new QPushButton("Resign", this);
    undoButton = new QPushButton("Undo", this);
    boardSizeSelect = new QComboBox(this); // Select button for board size
    boardSizeSelect->addItem("3x3");
    boardSizeSelect->addItem("4x4");

    connect(restartButton, &QPushButton::clicked, this, &SuperTicTacToe::restartGame);
    connect(resignButton, &QPushButton::clicked, this, &SuperTicTacToe::resignGame);
    connect(undoButton, &QPushButton::clicked, this, &SuperTicTacToe::undoMove);
    connect(boardSizeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SuperTicTacToe::changeBoardSize);

    rightPanelLayout->addWidget(turnLabel);
    rightPanelLayout->addWidget(restartButton);
    rightPanelLayout->addWidget(resignButton);
    rightPanelLayout->addWidget(undoButton);
    rightPanelLayout->addWidget(boardSizeSelect); // Add board size select button
    rightPanelLayout->addStretch();

    mainLayout->addLayout(globalGridLayout);
    mainLayout->addLayout(rightPanelLayout);
}

void SuperTicTacToe::createBoard() {
    localGridLayouts.resize(boardSize);
    boardButtons.resize(boardSize);
    boardState.resize(boardSize);
    globalBoardState.resize(boardSize);

    for (int i = 0; i < boardSize; ++i) {
        localGridLayouts[i].resize(boardSize);
        boardButtons[i].resize(boardSize);
        boardState[i].resize(boardSize);
        globalBoardState[i].resize(boardSize, 0);

        for (int j = 0; j < boardSize; ++j) {
            QGridLayout *localLayout = new QGridLayout();
            localGridLayouts[i][j] = localLayout;

            QGroupBox *groupBox = new QGroupBox(this);
            groupBox->setLayout(localLayout);
            groupBox->setStyleSheet("QGroupBox{ "
                                    "border: 2px solid black; "
                                    "margin-top: 0px; }");
            globalGridLayout->addWidget(groupBox, i * 2, j * 2);

            boardButtons[i][j].resize(boardSize);
            boardState[i][j].resize(boardSize);

            for (int k = 0; k < boardSize; ++k) {
                boardButtons[i][j][k].resize(boardSize);
                boardState[i][j][k].resize(boardSize);

                for (int l = 0; l < boardSize; ++l) {
                    boardState[i][j][k][l] = 0;
                    QPushButton *button = new QPushButton(this);
                    button->setFixedSize(50, 50);
                    localLayout->addWidget(button, k, l);
                    boardButtons[i][j][k][l] = button;

                    connect(button, &QPushButton::clicked, [this, i, j, k, l]() {
                        handleButtonClick(k, l, i, j);
                    });
                }
            }
        }
    }
}

void SuperTicTacToe::changeBoardSize(int index) {
    clearBoard();
    boardSize = (index == 0) ? 3 : 4; // Change board size based on selection
    createBoard();
    restartGame();
}

void SuperTicTacToe::clearBoard() { //rework needed
    QLayoutItem *child;
    while ((child = globalGridLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    localGridLayouts.clear();
    boardButtons.clear();
    boardState.clear();
    globalBoardState.clear();
}

void SuperTicTacToe::handleButtonClick(int localRow, int localCol, int globalRow, int globalCol) {
    if (boardState[globalRow][globalCol][localRow][localCol] != 0) return; // Ignore if already occupied
    if (nextLocalBoard != -1 && nextLocalBoard != globalRow * boardSize + globalCol) return; // Must play in the designated local board
    if (globalBoardState[globalRow][globalCol] != 0) return; // Ignore if local board is already won

    boardState[globalRow][globalCol][localRow][localCol] = currentPlayer;
    boardButtons[globalRow][globalCol][localRow][localCol]->setText(currentPlayer == 1 ? "X" : "O");

    // Record the move
    moveStack.push({localRow, localCol, globalRow, globalCol, currentPlayer, nextLocalBoard});

    // Check for a win in the local board
    bool localWin = false;
    if(boardSize == 3){
        for (int i = 0; i < 3; ++i) {
            if ((boardState[globalRow][globalCol][i][0] == currentPlayer && boardState[globalRow][globalCol][i][1] == currentPlayer && boardState[globalRow][globalCol][i][2] == currentPlayer) ||
                (boardState[globalRow][globalCol][0][i] == currentPlayer && boardState[globalRow][globalCol][1][i] == currentPlayer && boardState[globalRow][globalCol][2][i] == currentPlayer)) {
                localWin = true;
                break;
            }
        }
        if ((boardState[globalRow][globalCol][0][0] == currentPlayer && boardState[globalRow][globalCol][1][1] == currentPlayer && boardState[globalRow][globalCol][2][2] == currentPlayer) ||
            (boardState[globalRow][globalCol][0][2] == currentPlayer && boardState[globalRow][globalCol][1][1] == currentPlayer && boardState[globalRow][globalCol][2][0] == currentPlayer)) {
            localWin = true;
        }

        if (localWin) {
            globalBoardState[globalRow][globalCol] = currentPlayer;
            // Disable the buttons in this local board since it's won
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    boardButtons[globalRow][globalCol][i][j]->setEnabled(false);
                }
            }
        }
    }
    else if(boardSize == 4){
        for (int i = 0; i < 4; ++i) {
            if ((boardState[globalRow][globalCol][i][0] == currentPlayer && // if-statement for row and column win condition
                 boardState[globalRow][globalCol][i][1] == currentPlayer &&
                 boardState[globalRow][globalCol][i][2] == currentPlayer &&
                 boardState[globalRow][globalCol][i][3] == currentPlayer)
                ||
                (boardState[globalRow][globalCol][0][i] == currentPlayer &&
                 boardState[globalRow][globalCol][1][i] == currentPlayer &&
                 boardState[globalRow][globalCol][2][i] == currentPlayer &&
                 boardState[globalRow][globalCol][3][i] == currentPlayer)){
                localWin = true;
                break;
            }
        }
        if ((boardState[globalRow][globalCol][0][0] == currentPlayer && // if-statement for diagonal win condition
             boardState[globalRow][globalCol][1][1] == currentPlayer &&
             boardState[globalRow][globalCol][2][2] == currentPlayer &&
             boardState[globalRow][globalCol][3][3] == currentPlayer)
            ||
            (boardState[globalRow][globalCol][0][3] == currentPlayer &&
             boardState[globalRow][globalCol][1][2] == currentPlayer &&
             boardState[globalRow][globalCol][2][1] == currentPlayer &&
             boardState[globalRow][globalCol][3][0] == currentPlayer)) {
            localWin = true;
        }

        if (localWin) {
            globalBoardState[globalRow][globalCol] = currentPlayer;
            // Disable the buttons in this local board since it's won
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    boardButtons[globalRow][globalCol][i][j]->setEnabled(false);
                }
            }
        }
    }
    checkForWin();
    switchPlayer();

    // Determine the next local board
    if(boardSize == 3){
        nextLocalBoard = localRow * 3 + localCol;
        if (globalBoardState[nextLocalBoard / 3][nextLocalBoard % 3] != 0) {
            nextLocalBoard = -1; // If the target local board is won or drawn, the player can choose any board
        }
        highlightActiveLocalBoard(nextLocalBoard / 3, nextLocalBoard % 3); // Highlight the new active local board
    }
    if(boardSize == 4){
        nextLocalBoard = localRow * 4 + localCol;
        if (globalBoardState[nextLocalBoard / 4][nextLocalBoard % 4] != 0) {
            nextLocalBoard = -1; // If the target local board is won or drawn, the player can choose any board
        }
        highlightActiveLocalBoard(nextLocalBoard / 4, nextLocalBoard % 4); // Highlight the new active local board
    }
}

void SuperTicTacToe::undoMove() {
    if (moveStack.empty()) {
        QMessageBox::information(this, "Undo", "No moves to undo.");
        return;
    }

    // Get the last move
    Move lastMove = moveStack.top();
    moveStack.pop();

    // Revert the move
    boardState[lastMove.globalRow][lastMove.globalCol][lastMove.localRow][lastMove.localCol] = 0;
    boardButtons[lastMove.globalRow][lastMove.globalCol][lastMove.localRow][lastMove.localCol]->setText("");
    if (globalBoardState[lastMove.globalRow][lastMove.globalCol] != 0) {
        globalBoardState[lastMove.globalRow][lastMove.globalCol] = 0;
        // Re-enable buttons if the local board was marked as won
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                boardButtons[lastMove.globalRow][lastMove.globalCol][i][j]->setEnabled(true);
            }
        }
    }

    // Restore the player and the next local board
    currentPlayer = lastMove.player;
    nextLocalBoard = lastMove.nextLocalBoard;

    // Update UI elements
    updateTurnLabel();
    highlightActiveLocalBoard(nextLocalBoard / boardSize, nextLocalBoard % boardSize);
}

void SuperTicTacToe::switchPlayer(){ //
    if(currentPlayer == 1){
       currentPlayer = 2;
    }
    else{
        currentPlayer = 1;
    }
    updateTurnLabel();
}

void SuperTicTacToe::updateTurnLabel() {
    turnLabel->setText(QString("Current Turn: Player %1").arg(currentPlayer == 1 ? "X" : "O"));
}

void SuperTicTacToe::checkForWin() {
    if(boardSize == 3) {
        // Check rows and columns in the global board for a win
        for (int i = 0; i < 3; ++i) {
            if ((globalBoardState[i][0] == currentPlayer &&
                 globalBoardState[i][1] == currentPlayer &&
                 globalBoardState[i][2] == currentPlayer)
                ||
                (globalBoardState[0][i] == currentPlayer &&
                 globalBoardState[1][i] == currentPlayer &&
                 globalBoardState[2][i] == currentPlayer)){
                QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(currentPlayer == 1 ? "X" : "O"));
                return;
            }
        }
        // Check diagonals in the global board for a win
        if ((globalBoardState[0][0] == currentPlayer && //main diagonal
             globalBoardState[1][1] == currentPlayer &&
             globalBoardState[2][2] == currentPlayer)
            ||
            (globalBoardState[0][2] == currentPlayer && //counter diagonal
             globalBoardState[1][1] == currentPlayer &&
             globalBoardState[2][0] == currentPlayer)){
            QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(currentPlayer == 1 ? "X" : "O"));
            return;
        }
    }
    else if(boardSize == 4) {
        for (int i = 0; i < 4; ++i) {
            if ((globalBoardState[i][0] == currentPlayer && globalBoardState[i][1] == currentPlayer && globalBoardState[i][2] == currentPlayer && globalBoardState[i][3] == currentPlayer) ||
                (globalBoardState[0][i] == currentPlayer && globalBoardState[1][i] == currentPlayer && globalBoardState[2][i] == currentPlayer && globalBoardState[3][i] == currentPlayer)) {
                QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(currentPlayer == 1 ? "X" : "O"));
                return;
            }
        }
        // Check diagonals in the global board for a win
        if ((globalBoardState[0][0] == currentPlayer && //main diagonal
             globalBoardState[1][1] == currentPlayer &&
             globalBoardState[2][2] == currentPlayer &&
             globalBoardState[3][3] == currentPlayer)
            ||
            (globalBoardState[0][3] == currentPlayer && //counter diagonal
             globalBoardState[1][2] == currentPlayer &&
             globalBoardState[2][1] == currentPlayer &&
             globalBoardState[3][0] == currentPlayer)){
            QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(currentPlayer == 1 ? "X" : "O"));
            return;
        }
    }
}

void SuperTicTacToe::highlightActiveLocalBoard(int globalRow, int globalCol) {
    if(boardSize == 3){
    // Reset all group boxes to default border color
        for(int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                QGroupBox *groupBox = qobject_cast<QGroupBox*>(globalGridLayout->itemAtPosition(i * 2, j * 2)->widget());
                groupBox->setStyleSheet("QGroupBox { border: 2px solid black; margin-top: 0px; }");
                }
            }
            // Highlight the new active local board
        if(globalRow >= 0 && globalCol >= 0 && globalRow < 3 && globalCol < 3) {
            activeLocalBoardFrame = qobject_cast<QGroupBox*>(globalGridLayout->itemAtPosition(globalRow * 2, globalCol * 2)->widget());
            activeLocalBoardFrame->setStyleSheet("QGroupBox { border: 2px solid yellow; margin-top: 0px; }");
        }
    }
    else if(boardSize == 4){
        for(int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                QGroupBox *groupBox = qobject_cast<QGroupBox*>(globalGridLayout->itemAtPosition(i * 2, j * 2)->widget());
                groupBox->setStyleSheet("QGroupBox { border: 2px solid black; margin-top: 0px; }");
            }
        }
        // Highlight the new active local board
        if(globalRow >= 0 && globalCol >= 0 && globalRow < 4 && globalCol < 4) {
            activeLocalBoardFrame = qobject_cast<QGroupBox*>(globalGridLayout->itemAtPosition(globalRow * 2, globalCol * 2)->widget());
            activeLocalBoardFrame->setStyleSheet("QGroupBox { border: 2px solid yellow; margin-top: 0px; }");
        }
    }
}

void SuperTicTacToe::restartGame() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Restart", "Are you sure you want to restart the game?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        currentPlayer = 1;
        nextLocalBoard = -1;
        activeLocalBoardFrame = nullptr;
        updateTurnLabel();

        // Clear the move stack
        while (!moveStack.empty()) {
            moveStack.pop();
        }

        // Reset the game board for 3x3
        if (boardSize == 3) {
            for (int i = 0; i < 3; ++i) {
                globalBoardState[i].assign(3, 0);
                for (int j = 0; j < 3; ++j) {
                    boardState[i][j].assign(3, std::vector<int>(3, 0));
                    for (int k = 0; k < 3; ++k) {
                        for (int l = 0; l < 3; ++l) {
                            boardButtons[i][j][k][l]->setText("");
                            boardButtons[i][j][k][l]->setEnabled(true);
                        }
                    }
                }
            }
        }

        // Reset the game board for 4x4
        else if (boardSize == 4) {
            for (int i = 0; i < 4; ++i) {
                globalBoardState[i].assign(4, 0);
                for (int j = 0; j < 4; ++j) {
                    boardState[i][j].assign(4, std::vector<int>(4, 0));
                    for (int k = 0; k < 4; ++k) {
                        for (int l = 0; l < 4; ++l) {
                            boardButtons[i][j][k][l]->setText("");
                            boardButtons[i][j][k][l]->setEnabled(true);
                        }
                    }
                }
            }
        }

        // Reset the highlight on the boards
        highlightActiveLocalBoard(-1, -1);

        QMessageBox::information(this, "Restart", QString("The game has been restarted"));
    }
}


void SuperTicTacToe::resignGame() {
    // Ask for confirmation before resignation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Resign", "Are you sure you want to resign?", QMessageBox::Yes | QMessageBox::No);

    // If the player confirms resignation
    if (reply == QMessageBox::Yes) {
        // Identify the resigning player
        int resigningPlayer = currentPlayer;

        // Switch to the other player who will be the winner
        switchPlayer();

        // Display a message indicating the resigning and winning players
        if(resigningPlayer == 1){
            QString resignMessage = QString("Player X resigns! Player O wins!");
            QMessageBox::information(this, "Resign", resignMessage);
        }
        else if(resigningPlayer == 2){
            QString resignMessage = QString("Player O resigns! Player X wins!");
            QMessageBox::information(this, "Resign", resignMessage);
        }
        // Restart the game
        restartGame();
    }
}

