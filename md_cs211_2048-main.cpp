#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <algorithm>
#include <cctype>

using namespace std;

using Board = array<array<int,4>,4>;

static std::mt19937 rng((unsigned)std::random_device{}());

void clearScreen() {
    // ANSI escape clear screen
    cout << "\x1B[2J\x1B[H";
}

void printBoard(const Board &b, int score) {
    clearScreen();
    cout << "\nLet's Play 2048!\n(w/a/s/d + Enter to move, q to quit)\n";
    cout << "Score: " << score << "\n\n";
    cout << "+------+------+------+------+ \n";
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (b[r][c] == 0)
                cout << "|      ";
            else {
                string s = to_string(b[r][c]);
                int pad = 6 - s.size();
                int left = pad / 2, right = pad - left;
                cout << "|" << string(left, ' ') << s << string(right, ' ');
            }
        }
        cout << "|\n";
        cout << "+------+------+------+------+ \n";
    }
}

vector<pair<int,int>> emptyCells(const Board &b) {
    vector<pair<int,int>> out;
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            if (b[r][c] == 0)
                out.emplace_back(r, c);
    return out;
}

void addRandomTile(Board &b) {
    auto empties = emptyCells(b);
    if (empties.empty()) return;
    uniform_int_distribution<int> pick(0, (int)empties.size() - 1);
    auto [r, c] = empties[pick(rng)];
    uniform_int_distribution<int> v(1,10);
    b[r][c] = (v(rng) == 10) ? 4 : 2;
}

bool canMove(const Board &b) {
    for (int r=0; r<4; ++r)
        for (int c=0; c<4; ++c)
            if (b[r][c] == 0) return true;

    for (int r=0; r<4; ++r)
        for (int c=0; c<3; ++c)
            if (b[r][c] == b[r][c+1]) return true;

    for (int c=0; c<4; ++c)
        for (int r=0; r<3; ++r)
            if (b[r][c] == b[r+1][c]) return true;

    return false;
}

Board rotateClockwise(const Board &b) {
    Board out{};
    for (int r=0; r<4; ++r)
        for (int c=0; c<4; ++c)
            out[c][3-r] = b[r][c];
    return out;
}

Board rotateCounter(const Board &b) {
    Board out{};
    for (int r=0; r<4; ++r)
        for (int c=0; c<4; ++c)
            out[3-c][r] = b[r][c];
    return out;
}

pair<Board,int> moveLeft(const Board &b) {
    Board out = b;
    int gained = 0;

    for (int r=0; r<4; ++r) {
        vector<int> vals;
        for (int c=0; c<4; ++c)
            if (out[r][c] != 0)
                vals.push_back(out[r][c]);

        vector<int> merged;
        for (size_t i=0; i<vals.size(); ++i) {
            if (i+1 < vals.size() && vals[i] == vals[i+1]) {
                merged.push_back(vals[i]*2);
                gained += vals[i]*2;
                ++i;
            } else {
                merged.push_back(vals[i]);
            }
        }

        for (int c=0; c<4; ++c)
            out[r][c] = (c < (int)merged.size()) ? merged[c] : 0;
    }

    return {out, gained};
}

bool boardsEqual(const Board &a, const Board &b) {
    for (int r=0; r<4; ++r)
        for (int c=0; c<4; ++c)
            if (a[r][c] != b[r][c])
                return false;
    return true;
}

bool performMove(Board &b, char move, int &score) {
    Board before = b;

    pair<Board,int> res;

    if (move == 'a' || move == 'A') {
        res = moveLeft(b);

    } else if (move == 'd' || move == 'D') {
        Board r1 = rotateClockwise(rotateClockwise(b));
        res = moveLeft(r1);
        res.first = rotateClockwise(rotateClockwise(res.first));

    } else if (move == 'w' || move == 'W') {
        Board r1 = rotateCounter(b);
        res = moveLeft(r1);
        res.first = rotateClockwise(res.first);

    } else if (move == 's' || move == 'S') {
        Board r1 = rotateClockwise(b);
        res = moveLeft(r1);
        res.first = rotateCounter(res.first);

    } else {
        // cout << "Invalid move, try again!\n";
        return false;
    }

    if (!boardsEqual(before, res.first)) {
        b = res.first;
        score += res.second;
        return true;
    }

    return false;
}

bool checkWin(const Board &b) {
    for (int r=0; r<4; ++r)
        for (int c=0; c<4; ++c)
            if (b[r][c] >= 2048)
                return true;
    return false;
}

int main() {
    Board board{};
    int score = 0;

    addRandomTile(board);
    addRandomTile(board);

    while (true) {
        printBoard(board, score);

        if (checkWin(board)) {
            cout << "You reached 2048! Continue? (y/n): ";
            char ch;
            cin >> ch;
            if (tolower(ch) != 'y') break;
        }

        if (!canMove(board)) {
            cout << "Game over! Score: " << score << "\n";
            break;
        }

        cout << "Move (w/a/s/d), q to quit: ";
        char cmd; 
        cin >> cmd;
        cmd = tolower(cmd);

        if (cmd == 'q') {
            cout << "Final score: " << score << endl;
            break;
        }

        bool moved = performMove(board, cmd, score);
        if (moved) {
            addRandomTile(board);
        }
        else if (moved == false) {
            cout << "Invalid move, try again!\n";
        }
    }

    cout << "Thanks for playing!\n";
    return 0;
}
