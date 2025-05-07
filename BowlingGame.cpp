#include "BowlingGame.h"
#include <iostream>
#include <numeric>
#include <limits>
#include <sstream>

using namespace std;
using namespace BowlingGameConstants;

// Exception implementations
BowlingGameException::BowlingGameException(const string& message)
    : runtime_error(message)
{
}

InvalidPinCountException::InvalidPinCountException(const string& message)
    : BowlingGameException(message)
{
}

InputValidationException::InputValidationException(const string& message)
    : BowlingGameException(message)
{
}

// Validation methods
void BowlingGame::ValidatePinCount(int pins, int maxPins) const
{
    if (pins < MIN_PINS || pins > maxPins)
    {
        stringstream errorMsg;
        errorMsg << "Invalid pin count: " << pins 
                << ". Must be between " << MIN_PINS 
                << " and " << maxPins;
        throw InvalidPinCountException(errorMsg.str());
    }
}

void BowlingGame::Start()
{
    cout << "Starting Bowling Game...\n";
    
    try
    {
        for (int frameIndex = 0; frameIndex < 9; ++frameIndex)
        {
            HandleFrame(frameIndex);
        }
        
        HandleTenthFrame();
        
        int totalScore = CalculateScore();
        cout << "\nTotal Score: " << totalScore << endl;
    }
    catch (const BowlingGameException& e)
    {
        throw; // Rethrow to be handled by main
    }
}

void BowlingGame::HandleFrame(int frameIndex)
{
    vector<int> VCurrentFrame;
    int firstRoll = ReadRollInput("Frame " + to_string(frameIndex + 1) + " - Enter Roll 1 (0-10): ", FULL_SCORE);
    
    ValidatePinCount(firstRoll, FULL_SCORE);
    VCurrentFrame.push_back(firstRoll);

    if (firstRoll == FULL_SCORE)
    {
        cout << "Strike!\n";
    }
    else
    {
        int remainingPins = FULL_SCORE - firstRoll;
        cout << "Remaining pins: " << remainingPins << endl;
        
        int secondRoll = ReadRollInput("Enter Roll 2 (0-" + to_string(remainingPins) + "): ", remainingPins);
        ValidatePinCount(secondRoll, remainingPins);
        VCurrentFrame.push_back(secondRoll);
    }

    AFrames[frameIndex] = VCurrentFrame;
}

void BowlingGame::HandleTenthFrame()
{
    vector<int> VTenthFrame;
    cout << "10th Frame - Enter Roll 1 (0-10): ";
    
    int firstRoll = ReadRollInput("", FULL_SCORE);
    ValidatePinCount(firstRoll, FULL_SCORE);
    VTenthFrame.push_back(firstRoll);

    int secondMaxPins = (firstRoll == FULL_SCORE) ? FULL_SCORE : FULL_SCORE - firstRoll;
    cout << "Remaining pins: " << secondMaxPins << endl;
    
    int secondRoll = ReadRollInput("Enter Roll 2 (0-" + to_string(secondMaxPins) + "): ", secondMaxPins);
    ValidatePinCount(secondRoll, secondMaxPins);
    VTenthFrame.push_back(secondRoll);

    if (firstRoll == FULL_SCORE || firstRoll + secondRoll == FULL_SCORE)
    {
        int thirdRoll = ReadRollInput("Enter Roll 3 (0-10): ", FULL_SCORE);
        ValidatePinCount(thirdRoll, FULL_SCORE);
        VTenthFrame.push_back(thirdRoll);
    }

    AFrames[9] = VTenthFrame;
}

int BowlingGame::ReadRollInput(const string& prompt, int maxPins)
{
    int pinCount;
    while (true)
    {
        try
        {
            if (!prompt.empty())
            {
                cout << prompt;
            }
            
            if (!(cin >> pinCount))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw InputValidationException("Invalid input. Please enter a number.");
            }
            
            ValidatePinCount(pinCount, maxPins);
            break;
        }
        catch (const BowlingGameException& e)
        {
            cout << e.what() << endl;
            continue;
        }
    }
    return pinCount;
}

bool BowlingGame::IsStrike(const vector<int>& frame) const
{
    return !frame.empty() && frame[0] == FULL_SCORE;
}

bool BowlingGame::IsSpare(const vector<int>& frame) const
{
    return frame.size() >= 2 && frame[0] + frame[1] == FULL_SCORE;
}

int BowlingGame::GetStrikeBonus(int index) const
{
    if(index >= MAX_FRAMES - 1)
    {
        return 0; // No bonus for strike in the last frame
    }
    const auto& VNextFrame = AFrames[index + 1];

    if (IsStrike(VNextFrame))
    {
        const auto& VNextNextFrame = AFrames[index + 2];
        return FULL_SCORE + (!VNextNextFrame.empty() ? VNextNextFrame[0] : 0);
    }
    else
    {
        return VNextFrame.size() >= 2 ? VNextFrame[0] + VNextFrame[1] : (VNextFrame.empty() ? 0 : VNextFrame[0]);
    }
}

int BowlingGame::GetSpareBonus(int index) const
{
    if(index >= MAX_FRAMES - 1)
    {
        return 0; // No bonus for spare in the last frame
    }
    const auto& VNextFrame = AFrames[index + 1];
    return !VNextFrame.empty() ? VNextFrame[0] : 0;
}

int BowlingGame::CalculateScore() const
{
    int totalScore = 0;
    
    for (int frameIndex = 0; frameIndex < MAX_FRAMES; ++frameIndex)
    {
        const auto& VCurrentFrame = AFrames[frameIndex];
        
        if(frameIndex == MAX_FRAMES - 1)
        {
            totalScore += accumulate(VCurrentFrame.begin(), VCurrentFrame.end(), 0);
            continue;
        }
        if (IsStrike(VCurrentFrame))
        {
            totalScore += FULL_SCORE + GetStrikeBonus(frameIndex);
        }
        else if (IsSpare(VCurrentFrame))
        {
            totalScore += FULL_SCORE + GetSpareBonus(frameIndex);
        }
        else
        {
            totalScore += accumulate(VCurrentFrame.begin(), VCurrentFrame.end(), 0);
        }

        cout << "Frame " << frameIndex + 1 << " Score: " << totalScore << endl;
    }
    
    return totalScore;
}
