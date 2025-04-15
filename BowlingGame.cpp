#include <iostream>
#include <vector>

using namespace std;
namespace 
{
    constexpr int FULL_SCORE = 10;
}

class BowlingGame 
{
public:
    // Add a roll to the list
    bool roll(int pins) 
    {
        if (pins < 0 || pins > 10) 
        {
            cerr << "Invalid pin count: " << pins << ". It must be between 0 and 10.\n";
            return false;
        }
        rolls.push_back(pins);
        return true;
    }

    // Calculate total score
    int getTotalScore() 
    {
        int score = 0;
        int rollIndex = 0;

        for (int frame = 0; frame < 10; ++frame)
         {
            if (rollIndex >= rolls.size()) break;

            if (isStrike(rollIndex)) 
            {
                if (rollIndex + 2 >= rolls.size()) break;   
                score += FULL_SCORE + rolls[rollIndex + 1] + rolls[rollIndex + 2];
                rollIndex += 1;
            }
            else if (isSpare(rollIndex)) 
            {
                if (rollIndex + 2 >= rolls.size()) break;
                score += FULL_SCORE + rolls[rollIndex + 2];
                rollIndex += 2;
            }
            else 
            {
               if (rollIndex + 1 >= rolls.size()) break;
                int framePins = rolls[rollIndex] + rolls[rollIndex + 1];
                if (framePins > 10) 
                {
                    cerr << "Invalid frame score: more than 10 pins in a non-strike frame.\n";
                    return -1;
                }
                score += framePins;
                rollIndex += 2;
            }
        }

        return score;
    }

private:
    // helper functions to check for strikes and spares
    bool isStrike(int index) 
    {
        return FULL_SCORE == rolls[index];
    }

    bool isSpare(int index) 
    {
        return (index + 1 < rolls.size()) && (FULL_SCORE == rolls[index] + rolls[index + 1]);
    }

    vector<int> rolls;
};




int main() 
{
    BowlingGame game;
    cout << "Enter rolls (max 21 values for 10 frames, including bonus rolls):\n";

    int pins;
    int roll_count = 0;
    while (cin >> pins) 
    {
        if (++roll_count > 21) 
        {
            cerr << "Too many rolls entered. Maximum is 21.\n";
            return 1;
        }

        if (!game.roll(pins)) 
        {
            cerr << "Roll rejected. Please try again.\n";
            return 1;
        }

        // Stop input if user presses Enter 
        if (cin.peek() == '\n') break;
    }

    cout << "Total Score: " << game.getTotalScore() << endl;
    return 0;
}