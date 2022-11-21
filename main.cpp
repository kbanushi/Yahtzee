/**------------------------------------------
    Program 5: Yahtzee!

    Course: CS 141, Fall 2022.
    System: MacOS using Clion
    Starter Code Author: Dr. Sara Riazi
    Student Author: Krenar Banushi
-------------------------------------------*/
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <stdio.h>
#include <iomanip>
#include <vector>
using namespace std;

//#################################################################################################
class Dice{
private:
    int value = 0;
public:
    //Randomly assigns a value between from 1 to 6 to the dice.
    void roll(){value = rand() % 6 + 1;}
    //This function returns the value of the dice as an integer
    int reveal() const {return value;}
    //The autograder will set the values directly instead of calling roll() to avoid randomness!
    void setValue(int value) {this->value = value;}
};
//#################################################################################################
const int HAND_SIZE = 5;

class Hand {
public:
    //Display the value of the five dice
    void show();
    //Randomize the dice values while avoiding the selections the user made
    void play();
    //Returns the total of the dice added up as a integer
    int handTotal();
    //Returns a pointer to a die from the dice vector
    Dice* getDice(int diceNum);
    //selection is the string of dice numbers that the player wants to keep.
    //For example, "125" means that player wants to keep the first, second, and fifth dice, and roll the rest.
    void setSelection(string selection);
    //Will sort the dice vector in ascending order
    void sortDice();
    //Hand constructor
    Hand(){
        dice.resize(HAND_SIZE); //Set dice vector to size of HAND_SIZE
        userSelections.resize(HAND_SIZE); //Set user selections vector to size of HAND_SIZE
        //Populate dice vector with new dice objects
        for (int i = 0; i < HAND_SIZE; i++){
            dice.at(i) = new Dice;
        }
    }
private:
    vector<Dice*> dice;
    vector<int> userSelections; //This could also be a bool vector since we only use 0,1 to specify if the element was selected or not.
};

//This function will return the address of the Dice as a pointer.
//This function takes in an integer specifying which dice to return as a parameter
//This function returns a Dice* type
Dice* Hand::getDice(int diceNum) {
    return dice.at(diceNum);
}

//This function displays the hand dice values
//This function takes in no parameters
//This function returns void
void Hand::show() {
    cout << "Hand: ";
    for (int i = 0; i < HAND_SIZE; i++){
        cout << dice.at(i)->reveal() << " ";
    }
    cout << endl;
}

//This function will randomly assign values to all dice that have not been selected by the user
//This function takes no parameters
//This function has a return type of void
void Hand::play() {
    for (int i = 0; i < HAND_SIZE; i++){
        //The pointer i is used to check if that position in the userSelections vector was selected, as well as rolling the dice
        //If the userSelection returns 0, then we can roll the die at index i.
        if (userSelections.at(i) == 0){
            dice.at(i)->roll();
        }
    }
}

//This function will store the user's selections on which dice NOT to roll
//This function takes in a string specifying the user's dice selections
//This function has a void return type
void Hand::setSelection(std::string selection) {
    //We use the word clear to erase all user selections after each roll. This is entirely optional
    if (selection != "clear"){
        //We split the user input and use the input string integers as indexes
        for (int i = 0; i < selection.size(); i++){
            userSelections.at(selection.at(i) - '0' - 1) = 1;
        }
    }else{
        userSelections.clear();
        userSelections.resize(HAND_SIZE);
    }
}

//This function returns the sum of the dice in the hand
//This function takes in no parameters
//This function returns the sum as a type int
int Hand::handTotal() {
    int total = 0;
    //Find sum of all elements of dice vector
    for (auto x : dice){
        total += x->reveal();
    }
    return total; //Returns the total as an integer
}

//This function uses insertion sort to sort the dice in the hand
//This function takes in no parameters
//This function has a void return type
void Hand::sortDice() {
    Dice* temp;
    int minPos;

    //Iterate through all dice, assign the minPos as i
    for (int i = 0; i < dice.size() - 1; i++){
        minPos = i;
        //Iterate through every other element to see if we can find a lower min
        for (int j = i + 1; j < dice.size(); j++){
            //If we find lower min (comparing the dice values), we update minPos
            if (dice.at(j)->reveal() < dice.at(minPos)->reveal())
                minPos = j;
        }
        //Swap element at i with element at minPos if i != minPos
        if (i != minPos){
            temp = dice.at(i);
            dice.at(i) = dice.at(minPos);
            dice.at(minPos) = temp;
        }
    }
}
//######################################################################

//List of rows in the board
const int ONES = 0;
const int TWOS = 1;
const int THREES = 2;
const int FOURS = 3;
const int FIVES = 4;
const int SIXES = 5;
const int THREE_OF_KIND = 6;
const int FOUR_OF_KIND = 7;
const int FULL_HOUSE = 8;
const int SMALL_STRAIGHT = 9;
const int LARGE_STRAIGHT = 10;
const int CHANCE = 11;
const int YAHTZEE = 12;

const int BOARD_SIZE = 13;

class Game {
public:
    Game(){
        scores.resize(BOARD_SIZE);
        rowPlayed.resize(BOARD_SIZE);
    }
    //calcScore returns a score of a hand (5 dice) for given row in the board.
    //Note the rows are indexed from zero.
    //For example if the hand is 2 1 1 5 1 then calcScore(hand, ONES) returns 3 and  calcScore(hand, TWOS) returns 2.
    int calcScore(Hand* hand, int row);
    //Display the board
    void show();
    //Returns the score of the upper part of the board
    int getUpperScore();
    //Returns the score of the lower part of the board
    int getLowerScore();
    //Returns the bonus points
    int getBonusScore();
    //Returns the total score
    int getTotalScore();
    //Play a hand based on the selected row
    void play(Hand* hand, int row);
    //Check to see if a row has been played or not (returns true if a row has been played)
    bool isPlayed(int row);
    //Check to see if all rows haven been played or not (returns true if all rows have been played)
    bool isFinished();
private:
    vector<int> scores;
    vector<bool> rowPlayed;
    //Helper function to calculate a play from the upper rows
    int calcAnUpperScore(Hand* hand, int row) const;
    //Helper function to calculate a play from the lower rows
    int calcALowerScore(Hand* hand, int row);
    //Check to see if a hand contains the desired frequency (returns true if it contains the desiredFrequency)
    bool checkDiceFrequency(const vector<int>& diceFrequencies, int desiredFrequency, bool exactFrequency);
    //Check to see if the hand contains sequential dice up to numSequentialDice (returns true if the hand contained the sequential dice)
    bool checkSequentialDice(Hand* hand, int numSequentialDice);
};

//This is a helper function for the calcScore function.  It will calculate a score from the upper half of the board
//This function takes in a Hand pointer and integer specifying which row is being played
//This function returns the awarded points as an integer.
int Game::calcAnUpperScore(Hand *hand, int row) const {
    int score = 0;
    //Sum the score using the row + 1 (Because the input was decremented) everytime the (row + 1) appears in the hand
    for (int i = 0; i < HAND_SIZE; i++){
        if (hand->getDice(i)->reveal() == (row + 1)){
            score += (row + 1); //Running total
        }
    }
    return score; //Return the total score as an integer
}

//This function is a helper function for the calc score function.  It will calculate a score from the lower half of the board
//This function takes in a Hand pointer and an integer specifying which row is being played
//This function returns the awarded points as an integer
int Game::calcALowerScore(Hand *hand, int row){
    //We begin by finding the frequency of each die by incrementing a vector using the (die value - 1) as the index
    vector<int> diceFrequency(6, 0);
    for (int i = 0; i < HAND_SIZE; i++){
        diceFrequency.at(hand->getDice(i)->reveal() - 1)++;
    }
    switch (row){
        case THREE_OF_KIND:
            //call to checkDiceFrequency using the vector created above, and desired frequency of 3.
            if (checkDiceFrequency(diceFrequency, 3, false))
                return hand->handTotal(); //Return the sum of the hand if the desired frequency exists in the hand.
            return 0;
        case FOUR_OF_KIND:
            //call to checkDiceFrequency using the vector created above, and desired frequency of 4.
            if (checkDiceFrequency(diceFrequency, 4, false))
                return hand->handTotal(); //Return the sum of the hand if the desired frequency exists in the hand.
            return 0;
        case FULL_HOUSE:
            //Points are awarded if the hand contains dice with frequencies 3 and 2, or a hand with a frequency of 5.
            if (checkDiceFrequency(diceFrequency, 2, true) && checkDiceFrequency(diceFrequency, 3, true))
                return 25;
            else if (checkDiceFrequency(diceFrequency, 5, false))
                return 25; //Return 25 points if either condition is met.
            return 0;
        case SMALL_STRAIGHT:
            //Call to checkSequential dice using the hand and the desired number of sequential dice in a row.
            if (checkSequentialDice(hand, 4))
                return 30; //Return 30 points if the hand contains 4 sequential dice
            return 0;
        case LARGE_STRAIGHT:
            if (checkSequentialDice(hand, 5))
                return 40; //Return 40 points if the hand contains 5 sequential dice.
            return 0;
        case CHANCE:
            return hand->handTotal(); //Return the hand total
        case YAHTZEE:
            if (checkDiceFrequency(diceFrequency, 5, false))
                return 50; //Return 50 if the hand contains a dice frequency of 5.
            return 0;
        default:
            return 0; //FIXME: Return 0 if the user chooses a row outside of the board size
    }
}

//This function will show the board to the user using the game's scores vector to output the score for each row.
//This function takes in no parameters
//This function has a void return type
void Game::show() {
    cout << "1." << setw(8) << "Ones: " << setw(15) << scores.at(0) << endl
         << "2." << setw(8) << "Twos: " << setw(15) << scores.at(1) << endl
         << "3." << setw(10) << "Threes: " << setw(13) << scores.at(2) << endl
         << "4." << setw(9) << "Fours: " << setw(14) << scores.at(3) << endl
         << "5." << setw(9) << "Fives: " << setw(14) << scores.at(4) << endl
         << "6." << setw(9) << "Sixes: " << setw(14) << scores.at(5) << endl;
    cout << setw(9) << "Sum: " << setw(16) << getUpperScore() << endl
         << setw(11) << "Bonus: " << setw(14) << getBonusScore() << endl;
    cout << "7." << setw(19) << "Three of a kind: " << setw(4) << scores.at(6) << endl
         << "8." << setw(18) << "Four of a kind: " << setw(5) << scores.at(7) << endl
         << "9." << setw(14) << "Full house: " << setw(9) << scores.at(8) << endl
         << "10." << setw(17) << "Small straight: " << setw(5) << scores.at(9) << endl
         << "11." << setw(17) << "Large straight: " << setw(5) << scores.at(10) << endl
         << "12." << setw(9) << "Chance: " << setw(13) << scores.at(11) << endl
         << "13." << setw(10) << "Yahtzee: " << setw(12) << scores.at(12) << endl;
    cout << "Total:" << setw(19) << getTotalScore() << endl;
}

//This function will check if the hand contains a desired dice frequency.
//This function takes in a vector of integers, an integer of the desired frequency, and a boolean to specify if the frequency can be greater than or equal to the desired.
//This function returns a boolean output, if the desired frequency is found, the function returns true.
bool Game::checkDiceFrequency(const vector<int>& diceFrequencies, int desiredFrequency, bool exactFrequency = false) {
    if (exactFrequency){ //If we want to find an exact match in frequency (Used for FULL HOUSE)
        for (int x : diceFrequencies){
            if (x == desiredFrequency) //Check if there is a combination that meets the desiredFrequency
                return true;
        }
    }else{ //If we allow for a combination including the desired frequency plus higher frequencies
        for (int x : diceFrequencies){
            if (x >= desiredFrequency)
                return true;
        }
    }
    return false; //Return false if we do not find the desired frequency.
}

//This function will check if the hand contains a number of sequential dice in a row.
//This function takes in a Hand pointer, and an integer of the desired sequential dice in a row.
//This function returns a bool, if the desired sequential dice are found then the function returns true, otherwise it returns false.
bool Game::checkSequentialDice(Hand *hand, int numSequentialDice) {
    int count = 0;
    hand->sortDice(); //Sort the hand before checking.

    //This loop will check the current die and compare it to the following die, if it is exactly 1 less than the next die we increment the count of sequential dice found.
    //NOTE: This implementation does not remove repeated dice, instead we acknowledge they exist and continue the loop without changing anything if we come across a repeat.
    //This loop will also verify if we met the necessary amount of sequential dice before resetting the count.
    for (int i = 0; i < HAND_SIZE; i++){
        if (i + 1 < HAND_SIZE) {
            if ((hand->getDice(i)->reveal() + 1) == (hand->getDice(i+1)->reveal())) //Check if current die + 1 is == to next die
                count++;
            else if ((hand->getDice(i)->reveal()) == (hand->getDice(i+1)->reveal())) //Check if the current die and next are a repeat
                continue;
            else{
                //Check if we met the desired amount before resetting the count if the above conditions fail
                if (count + 1 >= numSequentialDice)  //We add one because the last element was not counted for in the earlier iterations.
                    return true;
                count = 0;
            }
        }else{
            count++; //We add one because the last element cannot compare against anything else, thus it is counted for
        }
    }
    if (count >= numSequentialDice)
        return true; //return true if we meet or succeed the desired sequential dice in a row
    return false; //False otherwise
}

//This function is used to check if a row has been played before.
//This function takes in an integer parameter specifying which row to check (NOTE: This value has already been decremented)
//This function returns a boolean value, if the row has been played the function will return true, otherwise will return false.
bool Game::isPlayed(int row) {
    return rowPlayed.at(row); //Using vector of rows played which is of boolean type.
}

//This function will check if the game has been finished by verifying if all rows have been played
//This function takes in no parameters.
//This function returns a boolean value based on if the game is finished. If all rows have been played the function returns true, otherwise returns false.
bool Game::isFinished() {
    for (bool x : rowPlayed){
        if (!x) //If we encounter a row that has not been played, return false and end function.
            return false;
    }
    return true; //Return true if we encounter no rows that haven't been played.
}

//This function will attempt to play the desired row using the current hand.
//This function takes in a Hand pointer and integer specifying the row to play as parameters.
//This function returns void.
void Game::play(Hand *hand, int row) {
    //If the selected row has been played the program will prompt the user to make another selection until they choose a row that hasn't been played.
    while (isPlayed(row)){
        cout << endl << "This combination has been played. Select another one: ";
        cin >> row;
    }
    rowPlayed.at(row) = true; //Update rows played once the row has been verified.
    scores.at(row) = calcScore(hand, row); //Set the score at the specified row to the calculated score for the input combination.
}

//This function will return all scores from the upper half of the board.
//This function takes in no parameters.
//This function returns the score of the upper half of the board as an integer.
int Game::getUpperScore() {
    int score = 0;
    //Sum the scores of rows 0-5 since the indexes are decremented by one
    for (int i = 0; i < 6; i++){
        score += scores.at(i);
    }
    return score; //Return the accumulated scores.
}

//This function will return the bonus score if the bonus score conditions are met
//This function takes in no parameters
//This function returns the bonus score as an integer.
int Game::getBonusScore() {
    return (getUpperScore() >= 63) ? 35 : 0; //If the upper score is >= 63, return 35 points, otherwise, return 0.
}

//This function will return the cumulative scores of the lower half of the board.
//This function takes in no parameters
//This function returns the total lower half scores as an integer
int Game::getLowerScore() {
    int score = 0;
    //Sum the scores of rows 6-12 since the indexes are decremented by one.
    for (int i = 6; i < 13; i++){
        score += scores.at(i);
    }
    return score; //Return the sum of the scores
}

//This function will return the total score of the entire board
//This function takes in no parameters
//This function returns the total score as an integer
int Game::getTotalScore() {
    //Will return the total scores using the functions we defined earlier for the upper, lower, and bonus scores.
    return getUpperScore() + getBonusScore() + getLowerScore();
}

//This function will calculate what score to return based on the hand and the row selected.
//This function takes in a Hand pointer and integer specifying which row to play
//This function returns the awarded points as an integer.
int Game::calcScore(Hand *hand, int row) {
    //This function splits the selection into two halves, the upper half of the board and lower half.
    if (row >= 0 && row <= 5){
        //Call helper function for upper half if selected row is 0-5 (NOTE: Row input was decremented)
        return calcAnUpperScore(hand, row);
    }
    else{
        //Call helper function for lower half if selected row is greater than 5 (NOTE: Row input was decremented)
        return calcALowerScore(hand, row);
    }
}//This function returns the calculated score based on the user's input combination as an integer
//#######################################################################################

//The run function acts as the main function of the program.  Here we allow the user the play as expected using randomized dice values.
//This function takes in no parameters
//This function has a void return type
void run() {
    Game game;
    Hand* hand = new Hand();
    int rolls, rowSelection;
    string userSelection;

    while (!game.isFinished()){ //Keep allowing user to play more rows until the rows are all filled.
        userSelection.clear();
        rolls = 3;
        hand->play();//Randomize the dice
        while (rolls > 0 && userSelection != "s"){ //Keep rolling until the user states not to or they run out of rolls.
            hand->setSelection("clear"); //Frees the user's dice selections
            game.show();
            hand->show();
            rolls--;
            cout << "Keep dice (s to stop rolling): ";
            cin >> userSelection;
            if (isdigit(userSelection.at(0)))
                hand->setSelection(userSelection);
            else
                break;
            hand->play();
        }
        cout << "Select a combination to play: ";
        cin >> rowSelection;
        game.play(hand, rowSelection - 1); //NOTE: The row selection is decremented
    }
}

// THE AUTOGRADER WILL REPLACE THE MAIN FUNCTION. 
// DO NOT ADD ANY LOGIC OF YOUR PROGRAM HERE.
int main() {
	srand(time(0));
	run();

	return 0;
}
