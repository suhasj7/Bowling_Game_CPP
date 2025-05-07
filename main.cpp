#include "BowlingGame.h"
#include <iostream>

using namespace std;

int main()
{
    try
    {
        cout << "!!! Welcome to the Bowling Game !!!\n";
        cout << "Start the game? (y/n): ";
        
        char userChoice;
        cin >> userChoice;

        if (userChoice == 'y' || userChoice == 'Y')
        {
            BowlingGame bowlingGame;
            bowlingGame.Start();
        }
        else
        {
            cout << "Game exited by user.\n";
        }
    }
    catch (const InvalidPinCountException& e)
    {
        cerr << "Pin Count Error: " << e.what() << endl;
    }
    catch (const InputValidationException& e)
    {
        cerr << "Input Error: " << e.what() << endl;
    }
    catch (const BowlingGameException& e)
    {
        cerr << "Game Error: " << e.what() << endl;
    }
    catch (const exception& e)
    {
        cerr << "Unexpected error: " << e.what() << endl;
    }

    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();
    
    return 0;
}