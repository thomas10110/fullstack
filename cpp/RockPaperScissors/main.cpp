#include <iostream>
#include <string>


using namespace std;
char getUserChoice{};
char getComputerChoice{};
int scoreUser{};
int scoreComputer{};
string result{};


int main() {
    
    cout << "Welcome to Rock, Paper, Scissors!" << endl;
    cout << "*********************************" << endl;
    cout << " To end game, write E" << endl;

    while (true) {

        cout << "\nEnter your choice (R, P, S or E to exit): ";
        cin >> getUserChoice;

        // Convert to uppercase (allows user to type r/p/s)
        getUserChoice = toupper(getUserChoice);

        // Exit
        if (getUserChoice == 'E') {
            cout << "Exiting game..." << endl;
            break;
        }

        // Validate input
        if (getUserChoice != 'R' && getUserChoice != 'P' && getUserChoice != 'S') {
            cout << "Invalid input. Please enter R, P, S, or E." << endl;
            continue; // restart loop
        }
    // Generate computer choice
    srand(time(0));
    
    int randomNum = rand() % 3;


    switch (randomNum) {
        case 0:
            getComputerChoice = 'R';
            break;
        case 1:
            getComputerChoice = 'P';
            break;
        case 2:
            getComputerChoice = 'S';
            break;
    }

    if (getUserChoice == getComputerChoice) {
        result = "It's a tie!";
    } else if ((getUserChoice == 'R' && getComputerChoice == 'S') ||
               (getUserChoice == 'P' && getComputerChoice == 'R') ||
               (getUserChoice == 'S' && getComputerChoice == 'P')) {
                
        result = "You win!";
        scoreUser++;
    } else {
        result = "Computer wins!";
        scoreComputer++;
    }

    cout << "Computer chose: " << getComputerChoice << endl;
    cout << result << endl;
    cout << "Score - You: " << scoreUser << " Computer: " << scoreComputer << endl; 
    }
    return 0;
}