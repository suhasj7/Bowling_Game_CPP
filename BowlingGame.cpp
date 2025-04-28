#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace {
    constexpr int FULL_SCORE = 10;
    constexpr int MAX_ROLLS = 21;
    constexpr int MAX_FRAMES = 10;
}

// Custom exception class for bowling game errors
class BowlingGameException : public runtime_error {
public:
    explicit BowlingGameException(const string& message) : runtime_error(message) {}
};

// BowlingGame class
class BowlingGame {
public:
    // Add a roll to the list
    void roll(int pins) 
    {
        if (pins < 0 || pins > FULL_SCORE) 
        {
            throw BowlingGameException("Invalid pin count: " + to_string(pins) + ". It must be between 0 and 10.");
        }
        rolls.push_back(pins);
    }

    // Calculate total score
    int getTotalScore() 
    {
        int score = 0;
        int rollIndex = 0;

        for (int frame = 0; frame < MAX_FRAMES; ++frame) 
        {
            if (rollIndex >= rolls.size()) 
            {
                throw BowlingGameException("Incomplete game: Not enough rolls to calculate the score.");
            }

            if (isStrike(rollIndex)) 
            {
                if (rollIndex + 2 >= rolls.size()) 
                {
                    throw BowlingGameException("Incomplete game: Strike bonus rolls missing.");
                }
                score += FULL_SCORE + rolls[rollIndex + 1] + rolls[rollIndex + 2];
                rollIndex += 1;
            } 
            else if (isSpare(rollIndex)) 
            {
                if (rollIndex + 2 >= rolls.size()) 
                {
                    throw BowlingGameException("Incomplete game: Spare bonus roll missing.");
                }
                score += FULL_SCORE + rolls[rollIndex + 2];
                rollIndex += 2;
            } 
            else 
            {
                if (rollIndex + 1 >= rolls.size()) 
                {
                    throw BowlingGameException("Incomplete game: Frame rolls missing.");
                }
                int framePins = rolls[rollIndex] + rolls[rollIndex + 1];
                if (framePins > FULL_SCORE) 
                {
                    throw BowlingGameException("Invalid frame score: More than 10 pins in a non-strike frame.");
                }
                score += framePins;
                rollIndex += 2;
            }
        }

        return score;
    }

private:
    // Helper functions to check for strikes and spares
    bool isStrike(int index) const 
    {
        return FULL_SCORE == rolls[index];
    }

    bool isSpare(int index) const 
    {
        return (index + 1 < rolls.size()) && (FULL_SCORE == rolls[index] + rolls[index + 1]);
    }

    vector<int> rolls;
};

// Main function
int main() 
{
    try {
        BowlingGame game;
        cout << "Enter rolls (max 21 values for 10 frames, including bonus rolls):\n";

        int pins;
        int rollCount = 0;

        while (cin >> pins) 
        {
            if (++rollCount > MAX_ROLLS) 
            {
                throw BowlingGameException("Too many rolls entered. Maximum is 21.");
            }

            game.roll(pins);

            // Stop input if user presses Enter
            if (cin.peek() == '\n') break;
        }

        cout << "Total Score: " << game.getTotalScore() << endl;
    } catch (const BowlingGameException& ex)
    {
        cerr << "Error: " << ex.what() << endl;
        cout << "Press Enter to exit...";
        std::getchar();
        std::getchar();
        return 1;
    }
    catch (const exception &ex)
    {
        cerr << "Unexpected error: " << ex.what() << endl;
        cout << "Press Enter to exit...";
        std::getchar();
        std::getchar();
        return 1;
    }

    cout << "Press Enter to exit...";
    std::getchar();
    std::getchar();

    return 0;
}