#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <cmath>
#include <chrono>

using namespace std;

/************************************************** Constants **************************************************/
/*size of the stack that stores the card options and the race value*/
#define stackSize1 50
/*number of card options that the user choose from each turn*/
#define cards 4
/*player stack size that contains all user choices*/
#define stackSize2 10

/************************************************** Global variables **************************************************/
/*Boolean that controls whether or not the game ends*/
bool gameOver = false;
/*Boolean that determines whether a player is playing against computer or against another player */
bool aiPlaying = false;
/*a variable that stores The race value that both players compete to reach or get close to */
int raceValue;
/* variables that store the summation of all pops both users make till the end of the game*/
int player1Score = 0;
int player2Score = 0;
/*the variables that store the elapsed time by the programe from start to finish*/
double  elapsed_time_inputs = 0, elapsed_time_total = 0;

/************************************************** Function declarations **************************************************/
/*A function that displays game instructions*/
void menu();
/*A function that determines the state of the the boolean aiPlaying to specify the flow of the code whether it's a player against another player or a player against the computer*/
void AIChecker();
/*a functionn that detects whether or not any key was hit on the keyboard*/
void checkKeyboard();
/*a function that shuffles the choices that appear to the users*/
void gamePlayPreparation();
/*a function that shuffle the choices for player 1 only*/
void gamePlayPreparation1();
/*a function that shuffles the choices for player 2 only*/
void gamePlayPreparation2();
/*a function that fills player 1 stack with his choice in each turn*/
void player1TurnToPick();
/*a function that fills player 2 stack with his choice in each turn*/
void player2TurnToPick();
/*a function that randomly fills AI stack (player 2 stack) with his random choice in each turn*/
void aiTurnToPick();
/*a function that displays the list of chosen values by each player*/
void decks();
/*a function that lets the users take turns to fill their stacks*/
void preGame();
/*the function that lets each user pop a value from his stack to be added up to his score*/
void Game();
/*a function that specifies whether or not a player score has exceeded the race value*/
void exceededRaceValue(int, int);
/*a function that compares the scores of tboth players with the race value to determine the winner*/
void whoWins();
/*a function that takes input from the user to detemine whether to start a new game or to terminate the programe*/
void continueGame();

/*the stack that generates and stores the choices to be displayed to the users + some utility functions needed to make the game more fun */
class Stack      
{
public:
    /*stack data and the index (top) that points to the top value in the stack*/
    int data[stackSize1];
    int top;
    /*a temporary array that stores the generated values that leads to race value; these values will be displayed for the user to choose from*/
    float chosen[stackSize1];
    /*a float that stores 1% of the race value*/
    float unity_percentage;
    /* a constructor that initializes top = -1 (.i.e stack is empty)*/
    Stack()
    {
        int top = -1;
    }

    /*stack basic operations*/
    void push(int x)
    {
        if (top < stackSize1)
        {
            top++;
            data[top] = x;
        }
        else
        {
            cout << "Stack is Full \n";
        }
    }

    int pop()
    {
        int temp;
        if (top > -1)
        {
            temp = data[top];
            top--;
            return temp;
        }

        else
            cout << "Stack is Empty\n";
    }

    /*function that generates the choices to be given to the players to choose from*/
    void choices()
    {

        /*algorithm that generates numbers that sum up exactly to the race value and pushes those values to the temporary array chosen[] */

        /*n is a variaable that stores the upper limit of the series (1+2+3...+n = race value)*/
        double n = -0.5 + 0.5 * sqrt(1 + 8 * raceValue) / 2;
        /*the variable count keeps track of the closing index of the temporary array chosen[], .i.e it specifies how many slots of the chosen[] array has been filled */
        int count = 0;
        /*if statement to set the variable count to a maximum = stackSize1 and not higher to prevent overflowing */
        if (ceil(n) < stackSize1)
        {
            count = ceil(n);
        }
        else
        {
            count = stackSize1;
        }
        /*a loop that fills the numbers from 1 to n in the temporary array chosen[]*/
        for (int j = 0; j < count; j++)
        {
            chosen[j] = j + 1;
        }

        /*if the temporary array is not full, the following algorithm will generate calculated percentages of the race Value and fill the rest of 
        the temporary array (chosen[]) with these percentages*/
        unity_percentage = 0;
        /*Put all possible percentages in the array chosen[]*/
        for (int i = count; i < stackSize1; i++)
        {
            unity_percentage = (float(raceValue) - unity_percentage * 50) / 100;
            chosen[i] = (i + 1) * unity_percentage;

            if (chosen[i] == raceValue)
                chosen[i] = chosen[i] - unity_percentage;
            else if (chosen[i] == 0)
                chosen[i]++;
        }
        
        /*push all generated values to the stack of choices*/
        top = -1;
        for (int h = 0; h < stackSize1; h++)
        {
           
            push(floor(chosen[h]));
        }
    }
    /*function that shuffles the contents of the stack*/
    void shuffleStack()
    {
        /*push all values generated again then shuffle the content of the stack*/
        top = -1;
        for (int h = 0; h < stackSize1; h++)
        {
           
            push(floor(chosen[h]));
        }
        int temp = 0;
        int randomIndex = 0;
        for (int i = 0; i < stackSize1; i++)
        {
            randomIndex = rand() % stackSize1;
            temp = data[i];
            data[i] = data[randomIndex];
            data[randomIndex] = temp;
        }
    }
    /*a function that randomly determines the race value*/
    void setRaceValue(int maximum) 
    {
        top = 0;

        data[top] = (rand() % maximum) + 100;//from 100 to (maximum + 100)

    }
   
};
/*the object that will be used to utilize this stack*/
Stack initialValues;

/*the stack that will store the player choices*/
class Player 
{
public:
    /*stack data and the index (top) that points to the top value in the stack*/
    int data[stackSize2];
    int top;
    /*an array that stores the cards that each user will choose from during his turn*/
    int assignedValue[cards];
    /* a constructor that initializes top = -1 (.i.e stack is empty)*/
    Player()
    {
        top = -1;
    }
    /*stack basic operations*/
    void push(int x)
    {
        if (top < stackSize2)
        {
            top++;
            data[top] = x;
        }
        else
        {
            cout << "Stack is Full \n";
        }
    }

    int pop()
    {
        int temp;
        if (top > -1)
        {
            temp = data[top];
            top--;
            return temp;
        }
        else
        {
            cout << "Stack is Empty\n";
        }
    }
    /*displaying the available choices for each player*/
    void displayDeckChoices(int p) 
    {
        if (p == 1)
        {
            cout << "Player " << p << " Deck choices: \n\n";

        }
        else if (p == 2 && aiPlaying)
        {
            cout << "AI Deck choices: \n\n";

        }
        else
        {
            cout << "Player " << p << " Deck choices: \n\n";
        }
        char temp = 'a';
        for (int i = 0; i < cards; i++) // a loop for displaying the choices 
        {
            cout << temp << ") " << assignedValue[i] << endl;
            temp++;
        }
    }
    /*a function that prints the list of the chosen cards by the player*/
    void printFullDeck(int p)
    {
        int i;
        if (p == 1)
        {
            cout << "Player " << p << " Deck choices: \n\n";

        }
        else if (p == 2 && aiPlaying)
        {
            cout << "AI Deck choices: \n\n";

        }
        else
        {
            cout << "Player " << p << " Deck choices: \n\n";
        }

        for (i = top; i > -1; i--)
        {
            cout << data[i] << endl;
        }
        cout << endl;
    }

};
Player player1;
Player player2;



int main()
{
    auto start = std::chrono::steady_clock::now();  //get the time when running the code
    srand(time(NULL));
    /*setting the race value*/
    initialValues.setRaceValue(400);
    raceValue = initialValues.data[initialValues.top];
    /*generating the choices*/
    initialValues.choices();
    /* displaying the instructions*/
    menu();
    /*the while loop that breaks only if the user says so*/
    while (!gameOver)
    {
        /*setting the boolean aiPlaying*/
        AIChecker();
        /*providing turns for the users to fill their stacks one by one*/
        preGame();
        /*clearing the screen*/
        system("cls");
        cout << "\nTake a final look at your deck choices\n";
        /*displaying users' final choices*/
        decks();
        cout << "-----------------------------------------------------------------------------------\n";
        cout << "\nPress any key to start the game...";
        /*waiting for the user to press any key to start the game*/
        checkKeyboard();
        /*now the users take turns to pop from their stack and add to their scores to try and reach the race value*/
        Game();
        /*displaying the winner*/
        whoWins();
        cout << "\n\nPress any key to continue...";
        Sleep(500);
        checkKeyboard();
        /*asking the user whether to start a new game or to terminate the program by setting the boolean gameOver to equal true or false*/
        continueGame();
    }
    /*code to calculate the elapsed time by the programe*/
    auto end = std::chrono::steady_clock::now();  //get the end time
    double elapsed_time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()); //calculate the time of the whole program including input time
    elapsed_time_total = elapsed_time - elapsed_time_inputs;//Total elapsed timr after subtracting the time of entering the inputs to get the accurate time of the program itself
    cout << "elapsed time for inputs in secends : " << elapsed_time_inputs / 1e9 << endl;
    cout << "elapsed time for mainprogram with inputs in secends : " << elapsed_time / 1e9 << endl;
    cout << "elapsed time of program without inputs in secends : " << abs(elapsed_time_total) / 1e9 << endl;//The required time execution for the program

    return 0;
}


/************************************************** Function definitions **************************************************/
/*function for displaying the games's rules*/
void menu() 
{
    cout << "********** Welcome to STACK GAME **********\n\n";
    cout << "Press any key to view the instructions...\n\n";
    checkKeyboard();
    cout << "1) This is a game for the lucky ones.\n";
    cout << "2) 2 players take turns to climb up to a specified number -(Race Value)- through a bunch of successive summations.\n";
    cout << "3) Each Player gets to choose his deck of numbers that will be added up together to get him to the race Value,\n   but they will have to choose these numbers carefully because the number pushed first\n   in their stack will be the last one added to their score\n";
    cout << "4) Win condition is either reaching the race value exactly after poping all numbers from stack or being the closest \n   to the race value at the end of the game.\n";
    cout << "5) A player can choose to play against AI as well.\n";
    cout << "\n\n Enjoy!!!\n\n";
    cout << endl << "Press any key to continue. :)\n";
    cout << endl;
    checkKeyboard();
}

void checkKeyboard()
{
    auto start = std::chrono::steady_clock::now();  //get the time when running the code
    bool x = true;
    while (x)
    {
        /*_kbhit() is a function that detects if the the keyboard was hit*/
        if (_kbhit())
        {
            x = false;
        }
    }
    /*to reset kbhit()*/
    _getch();
    auto end = std::chrono::steady_clock::now();  //get the end time
    double  elapsed_time2 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()); //find the difference
    elapsed_time_inputs += elapsed_time2;
}
/*function that checks if the player wants to play against the computer or there is another player*/
void AIChecker() 
{
    char k;
    cout << "Are you playing against AI? (Choose a or b, then press Enter)\n";
    cout << "a) yes \nb) No\n";
    auto start = std::chrono::steady_clock::now();  //get the time when running the code

    cin >> k;
    if (k == 'a' || k == 'A')
    {
        aiPlaying = true;
    }
    else if (k == 'b' || k == 'B')
    {
        aiPlaying = false;
    }
    else
    {
        system("cls");
        cout << "INVALID INPUT!!\n";

        AIChecker();
    }
    auto end = std::chrono::steady_clock::now();  //get the end time
    double elapsed_time6 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()); //find the difference
    elapsed_time_inputs += elapsed_time6;
}

void preGame()
{
    for (int i = 0; i < stackSize2; i++)
    {
        gamePlayPreparation();
        system("cls");
        cout << "Race Value is : " << raceValue << endl << endl;
        player1TurnToPick();
        if (aiPlaying)
        {
            aiTurnToPick();
        }
        else
        {
            player2TurnToPick();
        }
    }
}


void aiTurnToPick()
{
    player2.displayDeckChoices(2);
    cout << "\nAI turn to choose from the above set, please wait\n";
    player2.push(player2.assignedValue[rand() % 4]);
    Sleep(500);
}

/*this function fills the array that the user will choose from with random integers*/
void gamePlayPreparation()
{
    gamePlayPreparation1();
    gamePlayPreparation2();
}

/*this function fills the array that the user will choose from with random integers */
void gamePlayPreparation1()
{
    initialValues.shuffleStack();
    for (int i = 0; i < cards; i++)
    {
        player1.assignedValue[i] = initialValues.pop();
    }
}

void gamePlayPreparation2()
{
    initialValues.shuffleStack();
    for (int i = 0; i < cards; i++)
    {
        player2.assignedValue[i] = initialValues.pop();
    }
}
/*prints out the numbers that each player has chosen*/
void decks()
{
    cout << endl;
    cout << "******************************** Final Choices for each Player are ********************************\n\n";
    player1.printFullDeck(1);
    player2.printFullDeck(2);
}
/*This function is responsible for filling the stack of player 1 by pushing the choices that he will choose from the 4 choices in front of him in his own stack
 this is repaeated until his stack is full to begin the game*/
void player1TurnToPick()
{
    player1.displayDeckChoices(1);
    cout << endl;
    cout << "Player 1 turn to choose from the shown set above.\n\n";
    cout << "Please press a, b, c, or d to choose.\n\n";
    cout << "Press s to reshuffle the shown set.\n\n";
    char a = 0;
    bool x = true;
    auto start = std::chrono::steady_clock::now();  //get the time when running the code
    while (x)
    {
        if (_kbhit())
        {
            a = _getch();
            switch (a)
            {
            case 'a':
                player1.push(player1.assignedValue[0]);
                x = false;
                break;
            case 'b':
                player1.push(player1.assignedValue[1]);
                x = false;
                break;
            case 'c':
                player1.push(player1.assignedValue[2]);
                x = false;
                break;
            case 'd':
                player1.push(player1.assignedValue[3]);
                x = false;
                break;
            case 's':
                gamePlayPreparation1();
                system("cls");
                cout << "Race Value is : " << raceValue << endl << endl;
                player1TurnToPick();
                x = false;
                break;
            default:
                cout << "Invalid Input !!, please enter a, b, c, or d.\n\n";
                break;
            }
            auto end = std::chrono::steady_clock::now();  //get the end time
            double elapsed_time4 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()); //find the difference
            elapsed_time_inputs += elapsed_time4;
        }
    }
}
/*This function is responsible for filling the stack of player 2 by pushing the choices that he will choose from the 4 choices infront of him in his own stack
 this is repaeated until his stack is full to begin the game*/
void player2TurnToPick()
{
    player2.displayDeckChoices(2);
    cout << endl;
    cout << "Player 2 turn to choose from the shown set above.\n\n";
    cout << "Please press a, b, c, or d to choose.\n\n";
    cout << "Press s to reshuffle the shown set.\n\n";
    char a = 0;
    bool x = true;
    auto start = std::chrono::steady_clock::now();  //get the time when running the code
    while (x)
    {
        if (_kbhit())
        {
            a = _getch();
            switch (a)
            {
            case 'a':
                player2.push(player2.assignedValue[0]);
                x = false;
                break;
            case 'b':
                player2.push(player2.assignedValue[1]);
                x = false;
                break;
            case 'c':
                player2.push(player2.assignedValue[2]);
                x = false;
                break;
            case 'd':
                player2.push(player2.assignedValue[3]);
                x = false;
                break;
            case 's':
                gamePlayPreparation2();
                system("cls");
                cout << "Race Value is : " << raceValue << endl << endl;
                player1.displayDeckChoices(1);
                cout << endl;
                cout << "Player 1 turn to choose from the shown set above.\n\n";
                cout << "Please press a, b, c, or d to choose.\n\n";
                cout << "Press s to reshuffle the shown set.\n\n";
                player2TurnToPick();
                x = false;
                break;

            default:
                cout << "Invalid Input !!, please enter a, b, c, or d.\n\n";
                break;
            }
            auto end = std::chrono::steady_clock::now();  //get the end time
            double  elapsed_time5 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()); //find the difference
            elapsed_time_inputs += elapsed_time5;
        }
    }
}

/*a function that is responsible for returning the player to the game if his sum exceeded the race value,it makes the next pop from his stack a negative value so that his sum
 returns back to be below the race value and continue the game till his turns are finished*/
void exceededRaceValue(int playerNumber, int previousScore)
{
    
    if (playerNumber == 1)
    {
        if (player1Score > raceValue)
        {
            cout << "\nPlayer 1 surpassed the race value\n ";
            cout << "\nNext pop will be a negative value\n";
            for (int j = 0; j <= player1.top; j++)
            {
                player1.data[j] = -1 * player1.data[j];
            }
        }
        else if (player1Score < raceValue)
        {
            if (previousScore > raceValue)
            {
                cout << "\nPlayer 1 has gone below the race value\n ";
                cout << "\nNext pop will be a positive value\n";

            }
            for (int j = 0; j <= player1.top; j++)
            {
                player1.data[j] = abs(player1.data[j]);
            }
        }

    }
    else if (playerNumber == 2 && aiPlaying)
    {
        if (player2Score > raceValue)
        {
            cout << "\nAI surpassed the race value\n ";
            cout << "\nNext pop will be a negative value\n";
            for (int j = 0; j <= player2.top; j++)
            {
                player2.data[j] = -1 * player2.data[j];
            }
        }
        else if (player2Score < raceValue)
        {
            if (previousScore > raceValue)
            {
                cout << "\nAI has gone below the race value\n ";
                cout << "\nNext pop will be a positive value\n";

            }
            for (int j = 0; j <= player2.top; j++)
            {
                player2.data[j] = abs(player2.data[j]);
            }
        }
    }
    else
    {
        if (player2Score > raceValue)
        {
            cout << "\nPlayer 2 surpassed the race value\n ";
            cout << "\nNext pop will be a negative value\n";
            for (int j = 0; j <= player2.top; j++)
            {
                player2.data[j] = -1 * player2.data[j];
            }
        }
        else if (player2Score < raceValue)
        {
            if (previousScore > raceValue)
            {
                cout << "\nAI has gone below the race value\n ";
                cout << "\nNext pop will be a positive value\n";

            }
            for (int j = 0; j <= player2.top; j++)
            {
                player2.data[j] = abs(player2.data[j]);
            }
        }
    }
}
/*function that pops out the numbers from each player's stack from top to bottom and displays the score of each player for each turn
 and also makes the next pop a negative number if the sum exceeded the race value*/
void Game()
{
    int previousScore1 = 0;
    int previousScore2 = 0;

    for (int i = 0; i < stackSize2; i++)
    {
        system("cls");
        cout << "********** Game Started **********\n";
        cout << "\n\nRace Value: " << raceValue << endl;
        previousScore1 = player1Score;
        cout << "\nPlayer 1 Turn to pop from his STACK\n\n Press any key to pop\n\n";
        checkKeyboard();
        player1Score += player1.pop();
        cout << "Player 1 score = " << player1Score << endl << endl;

        exceededRaceValue(1, previousScore1);

        cout << "-----------------------------------------------------------------------------------\n";
        if (aiPlaying)
        {
            previousScore2 = player2Score;
            cout << "\nAI Turn to pop from his STACK\n\n Please wait...\n\n";
            Sleep(500);
            player2Score += player2.pop();
            cout << "AI score = " << player2Score << endl << endl;
            exceededRaceValue(2, previousScore2);

            cout << "-----------------------------------------------------------------------------------\n";
            cout << "\nPress any key to proceed...\n";
            checkKeyboard();
        }
        else
        {
            previousScore2 = player2Score;
            cout << "\nPlayer 2 Turn to pop from his STACK\n\n Press any key to pop\n\n";
            checkKeyboard();
            player2Score += player2.pop();
            cout << "Player 2 score = " << player2Score << endl << endl;
            exceededRaceValue(2, previousScore2);

            cout << "-----------------------------------------------------------------------------------\n";
            cout << "\nPress any key to proceed...\n";
            checkKeyboard();
        }

    }
}
/*function that determines who wins either player1,player 2 or the AI*/
void whoWins()
{
    system("cls");
    cout << "********** Game Ended **********\n\n";
    cout << "Race Value : " << raceValue << endl << endl;
    cout << "Player 1 score = " << player1Score << endl;
    if (aiPlaying)
    {
        cout << "AI score = " << player2Score << endl;
        cout << "-----------------------------------------------------------------------------------\n";
    }
    else
    {
        cout << "Player 2 score = " << player2Score << endl;
        cout << "-----------------------------------------------------------------------------------\n";
    }
    if (player1Score == raceValue && player2Score == raceValue)
    {
        cout << "-----------------------------------------------------------------------------------\n";
        cout << "***** IT'S A DRAW !!!! *****\n\n";
        return;
    }
    else if (abs(raceValue - player1Score) == abs(raceValue - player2Score))
    {
        cout << "\nThe difference betwwen each player and the race value = " << abs(raceValue - player1Score) << endl << endl;
        cout << "-----------------------------------------------------------------------------------\n";
        cout << "***** IT'S A DRAW !!!! *****\n\n";
        return;
    }
    else if (abs(raceValue - player1Score) > abs(raceValue - player2Score))
    {
        int range1 = abs(raceValue - player1Score);
        int range2 = abs(raceValue - player2Score);
        cout << "\nDifference between Player 1 and race value = " << range1 << endl;
        if (aiPlaying)
        {
            cout << "\nDifference between AI and race value = " << range2 << endl;
            cout << "-----------------------------------------------------------------------------------\n";
            cout << "***** AI WINS !!!! *****\n\n";
        }
        else
        {
            cout << "\nDifference between Player 2 and race value = " << range2 << endl;
            cout << "-----------------------------------------------------------------------------------\n";
            cout << "***** PLAYER 2 WINS !!!! *****\n\n";
        }

    }
    else if (abs(raceValue - player1Score) < abs(raceValue - player2Score))
    {
        int range1 = abs(raceValue - player1Score);
        int range2 = abs(raceValue - player2Score);
        cout << "\nDifference between Player 1 and race value = " << range1 << endl;
        if (aiPlaying)
        {
            cout << "\nDifference between AI and race value = " << range2 << endl;
            cout << "-----------------------------------------------------------------------------------\n";

        }
        else
        {
            cout << "\nDifference between Player 2 and race value = " << range2 << endl;
            cout << "-----------------------------------------------------------------------------------\n";
        }
        cout << "***** PLAYER 1 WINS !!!! *****\n\n";
    }
    else if (player1Score == raceValue && player2Score != raceValue)
    {
        cout << "***** PLAYER 1 WINS !!!! *****\n\n";
    }
    else if (player1Score != raceValue && player2Score == raceValue)
    {
        if (aiPlaying)
        {
            "***** AI WINS !!!! *****\n\n";
        }
        else
        {
            "***** PLAYER 2 WINS !!!! *****\n\n";
        }
    }
}
/*After gameover, you can either end the game and exit or you can play another game*/
void continueGame()
{
    system("cls");
    cout << "Do you want to start a new game? \n(If yes press ENTER)\n(If no press ESC to terminate the program)\n";

    bool x = true;
    char a = 0;
    auto start = std::chrono::steady_clock::now();  //get the time when running the code
    while (x)
    {
        if (_kbhit())
        {
            a = _getch();
            switch (a)
            {
            case 27: //Ascii for ESC button is 27
                gameOver = true;
                x = false;
                break;
            case 13: // Ascii for Enter is 13
                x = false;
                break;
            default:
                cout << "Invalid Input !!\n";
                break;

            }
            auto end = std::chrono::steady_clock::now();  //get the end time
            double  elapsed_time3 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()); //find the difference
            elapsed_time_inputs += elapsed_time3;
        }
    }
}
