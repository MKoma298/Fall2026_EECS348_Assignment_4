//Prolouge

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

class Sudoku
{
private:
    int grid[9][9];
    int solutionCount;

    // Check whether a number can be placed in a cell.
    bool isValid(int row, int col, int num)
    {
        // Check the row.
        for (int c = 0; c < 9; c++)
        {
            if (grid[row][c] == num)
                return false;
        }

        // Check the column.
        for (int r = 0; r < 9; r++)
        {
            if (grid[r][col] == num)
                return false;
        }

        // Find the top-left corner of the 3x3 block.
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;

        // Check the 3x3 block.
        for (int r = startRow; r < startRow + 3; r++)
        {
            for (int c = startCol; c < startCol + 3; c++)
            {
                if (grid[r][c] == num)
                    return false;
            }
        }

        return true;
    }

    // Check whether the original puzzle has conflicts.
    bool validatePuzzle()
    {
        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                if (grid[r][c] != 0)
                {
                    int num = grid[r][c];

                    // Temporarily remove the number.
                    grid[r][c] = 0;

                    bool valid = isValid(r, c, num);

                    // Restore the number.
                    grid[r][c] = num;

                    if (!valid)
                        return false;
                }
            }
        }

        return true;
    }

    // Recursive backtracking search.
    void searchSolutions(int position)
    {
        // All 81 cells have been checked.
        if (position == 81)
        {
            solutionCount++;

            cout << "\nSolution " << solutionCount << ":\n";
            printPuzzle();

            return;
        }

        int row = position / 9;
        int col = position % 9;

        // Skip cells that already contain numbers.
        if (grid[row][col] != 0)
        {
            searchSolutions(position + 1);
            return;
        }

        // Try every possible digit.
        for (int num = 1; num <= 9; num++)
        {
            if (isValid(row, col, num))
            {
                grid[row][col] = num;

                // Search for all solutions.
                searchSolutions(position + 1);

                // Undo this choice and try another digit.
                grid[row][col] = 0;
            }
        }
    }

public:
    // Initialize an empty grid.
    Sudoku()
    {
        solutionCount = 0;

        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                grid[r][c] = 0;
            }
        }
    }

    // Read a puzzle file.
    bool loadPuzzle(string filename)
    {
        ifstream inputFile(filename);

        if (!inputFile.is_open())
            return false;

        char ch;
        int count = 0;

        // Read digits and underscores, ignoring whitespace.
        while (inputFile.get(ch))
        {
            if (ch >= '1' && ch <= '9')
            {
                if (count >= 81)
                    return false;

                grid[count / 9][count % 9] = ch - '0';
                count++;
            }
            else if (ch == '_')
            {
                if (count >= 81)
                    return false;

                grid[count / 9][count % 9] = 0;
                count++;
            }
            else if (!isspace(static_cast<unsigned char>(ch)))
            {
                // Reject invalid characters.
                return false;
            }
        }

        inputFile.close();

        // The file must contain exactly 81 cells.
        if (count != 81)
            return false;

        // Check for conflicting numbers.
        return validatePuzzle();
    }

    // Print the current grid.
    void printPuzzle()
    {
        for (int r = 0; r < 9; r++)
        {
            if (r % 3 == 0)
                cout << "+-------+-------+-------+\n";

            for (int c = 0; c < 9; c++)
            {
                if (c % 3 == 0)
                    cout << "| ";

                if (grid[r][c] == 0)
                    cout << "_ ";
                else
                    cout << grid[r][c] << " ";
            }

            cout << "|\n";
        }

        cout << "+-------+-------+-------+\n";
    }

    // Solve the puzzle and print every solution.
    void solve()
    {
        solutionCount = 0;

        searchSolutions(0);

        if (solutionCount == 0)
        {
            cout << "\nNo solution found\n";
        }
        else
        {
            cout << "\nTotal solutions found: "
                 << solutionCount << "\n";
        }
    }
};

int main()
{
    for (int i = 1; i <= 5; i++)
    {
        string filename = "puzzle" + to_string(i) + ".txt";

        cout << "\n=================================\n";
        cout << "Puzzle file: " << filename << "\n";
        cout << "=================================\n";

        Sudoku puzzle;

        if (puzzle.loadPuzzle(filename))
        {
            cout << "\nOriginal puzzle:\n";
            puzzle.printPuzzle();

            cout << "\nSearching for solutions...\n";
            puzzle.solve();
        }
        else
        {
            cout << "Could not load " << filename
                 << ". Check that the file exists, "
                 << "contains exactly 81 valid cells, "
                 << "and has no conflicting numbers.\n";
        }
    }

    return 0;
}