#pragma once
#include <vector>
#include <array>
#include <string>
#include <stdexcept>

namespace BowlingGameConstants
{
    constexpr int FULL_SCORE = 10;
    constexpr int MAX_FRAMES = 10;
    constexpr int MIN_PINS = 0;
}

// Exception hierarchy
class BowlingGameException : public std::runtime_error
{
public:
    explicit BowlingGameException(const std::string& message);
};

class InvalidPinCountException : public BowlingGameException
{
public:
    explicit InvalidPinCountException(const std::string& message);
};

class InputValidationException : public BowlingGameException
{
public:
    explicit InputValidationException(const std::string& message);
};

class BowlingGame 
{
public:
    void Start();

private:
    std::array<std::vector<int>, BowlingGameConstants::MAX_FRAMES> AFrames;
    
    void HandleFrame(int frameIndex);
    void HandleTenthFrame();
    int ReadRollInput(const std::string& prompt, int maxPins);
    bool IsStrike(const std::vector<int>& frame) const;
    bool IsSpare(const std::vector<int>& frame) const;
    int GetStrikeBonus(int index) const;
    int GetSpareBonus(int index) const;
    int CalculateScore() const;
    
    void ValidatePinCount(int pins, int maxPins) const;
};