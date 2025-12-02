//C++ 2048 Game

#include <iostream> // for std input/output
#include <vector> // for vector 
#include <random> // for random number generator
#include <string>
#include <iomanip> // for manipulators
//maybe unused libraries...
//#include <algorithm> // for algorithm functions
//#include <cctype> // for character classification functions
//#include <tuple> // for tuple function
//#include <ctime> // for time functions

using namespace std;

static mt19937 rng((unsigned)random_device{}()); // random number generator

//global variables
int N;   // global variable to declare initial grid size (NxN)
bool quitFlag = false; // initialize quit flag to false so that only user choice in runGame() will trigger quit flag to end game
bool cheatWin = false; // initialize cheat flag to false so that only user choice in runGame() will trigger flag to trigger win game, for testing checkWin(), because I am not good at this game

// alias for dynamic board type
using Board = vector<vector<int>>;


void clearScreen() { // function to clear screen
#ifdef _WIN32 // if windows detected, uses "cls" to clear screen
    system("cls"); 
#else
    cout << "\033[2J\033[1;1H"; // if not windows, uses this ANSI escape code to clear screen
#endif
}

void printBoard(const Board &b, int score) { //print board with borders and score
    cout << "\nLet's Play 2048 (" << N << "x" << N << ")!\n";
    cout << "(w/a/s/d + Enter to move, q to quit, r to reset game)\n";
    cout << "Score: " << score << "\n\n";

    //print top border
    for (int i = 0; i < N; i++) { 
        cout << "+------";
    }
    cout << "+\n";

    // print each row
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (b[r][c] == 0) {
                cout << "|      ";
            }
            else {
                string s = to_string(b[r][c]);
                // align numbers to center of cell
                int pad = 6 - s.size();
                int left = pad / 2, right = pad - left;
                cout << "|" << string(left, ' ') << s << string(right, ' ');
            }
        }
        cout << "|\n";
        // print bottom border
        for (int i = 0; i < N; i++) {
            cout << "+------";
        }
        cout << "+\n";
    }
}

vector<pair<int,int>> emptyCells(const Board &b) { // return vector of empty cells, for tile spawning
    vector<pair<int,int>> out;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (b[r][c] == 0) {
                out.emplace_back(r, c);
            }
        }
    }
    return out;
}

void addRandomTile(Board &b) { // add value of 2 or 4 to random tile
    auto empties = emptyCells(b);
    if (empties.empty()) {
        return;
    }
    // pick random cell
    uniform_int_distribution<int> pick(0, (int)empties.size() - 1);
    pair<int,int> pos = empties[pick(rng)];
    int r = pos.first;
    int c = pos.second;
    // 90% chance for value to spawn is 2, 10% chance for value to spawn is 4
    uniform_int_distribution<int> v(1,10);
    b[r][c] = (v(rng) == 10) ? 4 : 2;
}

bool canMove(const Board &b) { // check if space is empty, also for detecting if game is over
    // if any cell is empty, movement is possible
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (b[r][c] == 0) {
                return true;
            }
        }
    }
    // check horizontal
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N-1; ++c) {
            if (b[r][c] == b[r][c+1]) {
                return true;
            }
        }
    }
    // check vertical
    for (int c = 0; c < N; ++c) {
        for (int r = 0; r < N-1; ++r) {
            if (b[r][c] == b[r+1][c]) {
                return true;
            }
        }
    }

    return false;
}

Board rotateClockwise(const Board &b) { // rotate board 90 degrees clockwise
    Board out(N, vector<int>(N));
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            out[c][N - 1 - r] = b[r][c];
        }
    }
    return out;
}

Board rotateCounter(const Board &b) { // rotate board 90 degree counter-clockwise
    Board out(N, vector<int>(N));
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            out[N - 1 - c][r] = b[r][c];
        }
    }
    return out;
}

pair<Board,int> moveLeft(const Board &b) { // moves/merges row left to return new values and score
    Board out = b;
    int gained = 0;

    for (int r = 0; r < N; ++r) {
        vector<int> vals;
        for (int c = 0; c < N; ++c) {
            if (out[r][c] != 0) {
                vals.push_back(out[r][c]);
            }
        }

        vector<int> merged;
        // merge adjacent equal values
        for (size_t i = 0; i < vals.size(); ++i) {
            if (i+1 < vals.size() && vals[i] == vals[i+1]) {
                merged.push_back(vals[i] * 2);
                gained += vals[i] * 2;
                ++i;
            } else {
                merged.push_back(vals[i]);
            }
        }
        //copy merged values back to board to be displayed
        for (int c = 0; c < N; ++c) {
            out[r][c] = (c < (int)merged.size()) ? merged[c] : 0;
        }
    }

    return {out, gained};
}

bool boardsEqual(const Board &a, const Board &b) { // compares two boards
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (a[r][c] != b[r][c]) {
                return false;
            }
        }
    }
    return true;
}

bool performMove(Board &b, char move, int &score) { // to move board values based on user input
    Board before = b;
    pair<Board,int> res;

    move = tolower(move);

    if (move == 'a') { // moves values left
        res = moveLeft(b);

    } else if (move == 'd') { // moves values right
        Board r1 = rotateClockwise(rotateClockwise(b));
        res = moveLeft(r1);
        res.first = rotateClockwise(rotateClockwise(res.first));

    } else if (move == 'w') { // moves values up
        Board r1 = rotateCounter(b);
        res = moveLeft(r1);
        res.first = rotateClockwise(res.first);

    } else if (move == 's') { // moves values down
        Board r1 = rotateClockwise(b);
        res = moveLeft(r1);
        res.first = rotateCounter(res.first);

    } else { // if user choice doesn't match w/a/s/d, does nothing and returns back to runGame()
        return false;
    }

    if (!boardsEqual(before, res.first)) {
        b = res.first;
        score += res.second;
        return true;
    }
    return false;
}

bool checkWin(const Board &b) { // check if a value on board is equal to 2048 or is greater than 2048 to end game
    if (cheatWin) {
        return true;
    }
    for (int r=0; r < N; r++) {
        for (int c=0; c < N; c++) {
            if (b[r][c] >= 2048) {
                return true;
            }
        }
    }
    return false;
}

void runGame() { // initializes game
    cout << "Enter board size (eg. \"5\" for a 5x5 grid, minimum size is 4x4): ";
    cin >> N;

    if (N < 3) {
        cout << "Grid too small! Setting to 4.\n"; // if choice is below 4, defaults to 4 to make 4x4 grid
        N = 4;
    }

    Board board(N, vector<int>(N, 0)); // initializes vector for board values with 0 according to choice of grid size
    int score = 0;

    addRandomTile(board); // add value to random tile on board
    addRandomTile(board); 

    while (true) {
        printBoard(board, score); // print board with current values and score

        if (checkWin(board)) { // if game win detected
            if (cheatWin) {
                cout << "You reached 2048 by cheating! Quiting game...";
                return;
            }
            else {
                cout << "You reached 2048! Continue? (y/n): ";
                char ch;
                cin >> ch;
                if (tolower(ch) != 'y') {
                    return;
                }
            }
        }

        if (!canMove(board)) { // if no more moves able to be done
            cout << "Game over! Final score: " << score << "\n";
            return;
        }

        cout << "Move (w/a/s/d), q = quit, r = reset: "; // prompt for user move choice
        char cmd;
        cin >> cmd;
        cmd = tolower(cmd);

        if (cmd == 'q') { // to end game
            cout << "Final score: " << score << endl;
            quitFlag = true;
            return;
        }
        else if (cmd == 'r') {
            cout << "Resetting game...\n";
            return;  // exit to main() to skip to "Play again" prompt for user choice to end game or replay game
        }
        else if (cmd == 'c') { //cheat option to win game because I am not good at this game
            cheatWin = true;
        }

        bool moved = performMove(board, cmd, score);
        if (moved) {
            clearScreen(); // clear screen
            addRandomTile(board); // add value to random tile on board
        }
        else {
            clearScreen(); // clear screen and then displays invalid message
            cout << "Invalid move, try again!\n";
        }
    }
}

int main() {
    while (true) {
        runGame(); // run one full game session
        if (quitFlag || cheatWin) { // if q is pressed in runGame or cheatWin enabled, breaks loop immediately
           break;
        }
        cout << "Play again? (y/n): ";
        char again;
        cin >> again;
        if (tolower(again) != 'y') {
            break;
        }
    }

    cout << "Thanks for playing!\n";
    return 0;
}