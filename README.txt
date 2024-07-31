Github link: https://github.com/nanoscaffolding/super-tic-tac-toe-Qt

SuperTicTacToe

SuperTicTacToe is an extended version of the classic Tic-Tac-Toe game, featuring a larger board size with customizable dimensions (3x3, 4x4, 5x5). The game is implemented using Qt framework for the graphical user interface and C++ for the logic.
Features

    Dynamic Board Size: Choose between 3x3, 4x4, or 5x5 grid sizes for a varied gameplay experience.
    Interactive GUI: Graphical user interface with clickable buttons representing each cell in the game board.
    Player Turns: Alternates between Player X and Player O with each move.
    Win Conditions: Detects and highlights winning moves both locally within each sub-board and globally across the main board.
    Undo Move: Allows players to undo their last move if needed.
    Restart Game: Option to restart the game at any point, clearing all board states.
    Resignation: Players can choose to resign, resulting in a win for the opponent.

How to Use
    Starting the Game:
        Run the executable or build the project from source using Qt Creator or CMake.
    Gameplay:
        Click on any enabled button within the active local board to place your move.
        Follow the turn indicator to know whose turn it is (Player X or Player O).
    Winning the Game:
        Achieve a win by completing a row, column, or diagonal within a LOCAL board.
        Complete a row, column, or diagonal across the GLOBAL board to win the entire game.
    Restart or Resign:
        Use the Restart Game button to reset the board and start a new game.
        Resign option allows a player to concede the game, resulting in a win for the opponent.
    Changing Board Size:
        Select a different board size from the dropdown menu to change the dimensions of the game board.

Requirements
    Qt 5 or higher