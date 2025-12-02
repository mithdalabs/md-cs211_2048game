# md-cs211_2048game
2048 Game for CS211
- Instructions -
1.	Refer here for how the game looks and how to play the game.
2.	You must only use the up, down, right left arrows or ‘W’ ’A’ ‘ S’ ’D’ for the up, left, down, right movements of the numbers.
3.	The game must have AT LEAST 4*4 grids. 
4.	The game must have the option to reset a game at any point.
5.	Write a project report which can include but is not limited to; project brief, project description, write-up, PowerPoint presentation, etc. in addition to your code file.
6.	Source code file (saved with the extension .cpp or GitHub/GitLab link) and project report ( in PDF or .docx format )
a.	Code file and report must be submitted separately as 2 separate files in the same submission.


Optional (1 extra credit each)

1.	You may add the current and the best score feature.
2.	You may add the number of grids.
3.	You may create the GUI (Graphic User Interface).
4.	Any other ad-on/ feature

Project Brief ---
The goal of this project was to design and implement a playable version of the popular puzzle game 2048 using C++. The program runs in a console environment and includes tile movement, merging logic, in-game score tracking, win/lose detection, and a reset/quit option.

Goals included:
Clean, structured, modular code
Clear commenting throughout program
Beginner-friendly logic and readability
Well-designed, intuitive, and user-friendly interface 

The final program meets all requirements and can be extended with more advanced features if desired.

Project Description ---
2048 is a single-player sliding tile puzzle.
The objective is to combine matching tiles to reach the value 2048.
Each move slides all tiles in one direction (up, down, left, right).
When two equal-valued tiles collide, they merge and the player's score increases.

This C++ implementation provides:

A customizable board size (minimum 4×4)
Random tile generation (2 or 4)
Correct sliding & merging behavior
A fully operational game loop
Clean function-based control flow

The project uses the following functions:

Function and Purpose:
printBoard() : Displays the game grid with values and borders
emptyCells() : Finds all empty tiles
addRandomTile() : Spawns a 2 or 4 in a random empty tile
canMove() : Determines if the game should continue or game over
rotateClockwise() / rotateCounter() : Used for movement logic
moveLeft() : Core merging algorithm
performMove() : Applies movement in any direction
checkWin() : Determines if the player has won by checking if any tile has the value of 2048 or greater
runGame() : Main game loop
main() : Repeated play sessions and quit handling

Technical Write Up ---
1. The board is represented as:

using Board = vector<vector<int>>;

This dynamic structure allows:
Flexible board sizes
Easy rotation operations
Simple iteration for sliding/merging

2. Movement & Merging Logic

Rather than writing four separate movement algorithms, the game uses:
moveLeft() as the only merging function
Board rotation to map all moves into left-moves

For example:
Move Right = rotate 180° → moveLeft → rotate 180°
Move Up = rotate counterclockwise → moveLeft → rotate clockwise
Move Down = rotate clockwise → moveLeft → rotate counterclockwise

This avoids duplicated, error-prone logic and ensures consistency.

3. Random Number Generation

C++17’s <random> library provides:
static mt19937 rng((unsigned)random_device{}());

This ensures:
High-quality randomness
No repeated sequences
Cross-platform deterministic behavior
Tile values (2 or 4) are generated with a probability distribution.

4. Reset System Without goto

Initially used to fulfill the reset game requirement, instead of using goto, which is discouraged in C++ programming, the game session is wrapped inside runGame().
Pressing r ends runGame() early, control returns to main(), which then asks player if they wish to play again.
This keeps the program structured and readable.

5. Development Challenges

Implementing a Clean Movement Mechanism
Ensuring accurate and bug-free merging required:
Preventing double merges
Proper left-compression of values
Avoiding index errors during merging
Using a vector-based approach simplified this process.

6. Maintaining Cross-Platform Screen Clearing

Windows uses:
system("cls");

Linux/macOS use ANSI escape sequences.
The code detects the platform using:
#ifdef _WIN32

Ensuring universal compatibility.
