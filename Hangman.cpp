#include "Hangman.h"
#include <iostream>
#include <cstdlib>    //Necessary preprocessor directives for following components
#include <fstream>
#include <vector>

using namespace std;

vector<string> words; //These vectors are used in most of the functions below, hence global scope.

Hangman::Hangman()
{
    totalTries = 0;
    queueSize = 6;
    bodyQueue = new string[queueSize];
    queueTail = 0;
    queueHead = 0;
}

Hangman::~Hangman()
{
    delete []bodyQueue;
}

/*This function prints the directions of the game to the console, keeps the main.cpp neat to define them here.*/
void Hangman::loadDirections() const
{
	cout << "\nDirections:" << endl;
	cout << "  -Choose a difficulty level - (1) Easy (2) Medium or (3) Hard" << endl;
	cout << "  -I will choose a word based on the difficulty you've chosen" << endl;
	cout << "  -Once I've chosen a word, you can guess a letter" << endl;
	cout << "  -For each incorrect guess I will add a body part to the hanged man" << endl;
	cout << "  -I'll keep track of how many words you guess correctly" << endl;
	cout << "  -The object is to see how many words you guess before I\n   complete my drawing (6 incorrect guesses)" << endl;
	cout << "\nGood Luck and try to avoid the noose!\n" << endl;
}

void Hangman::initializeGame()
{
    cout << "Hello " << getName() << ", please choose a level to begin." << endl;           //Calls getName function which prints user's name to console.
	cout << "Choose a difficulty level - ";
	chooseLevel();                                                                          //Calls chooseLevel function which allows user to select level, level is assigned appropriate text files.
	cout << endl;
    loadWords();    //Calls loadWords function which loads words text file into vector.

	cout << "Guess a letter for this Level " << getLevel() << " word." << endl;    //Calls getLevel function which prints level integer to console.

	scrambler();  //Calls the scrambler function which essentially runs the entire game.

}

/*This function allows the user to enter the name they wish to be referred to as. The getline function captures the entire string as well
as white space; the data member name is assigned the string and prints the user's name to the console when called by the getName function.*/
void Hangman::enterName()
{
	string n;
	getline(cin, n);
	name = n;
}

/*This function allows the user to choose a difficulty level from 1 to 3, each level is associated with a words text file (containing 10 words).
The data member level is assigned the int lev and prints the level number to the console when called by the getLevel function.*/
void Hangman::chooseLevel()
{
	int lev;
	string s;
	while (true) {
        cout << "(1) Easy (2) Medium or (3) Hard: ";
	 	cin >> lev;
		level = lev;
	    if (level == 1) {
			wordsFile = "level_1_words.txt";
			break;
		}
		else if (level == 2) {
			wordsFile = "level_2_words.txt";
			break;
		}
		else if (level == 3) {
			wordsFile = "level_3_words.txt";
			break;
		}
		else if (cin.fail()) {
			cin.clear();
			getline(cin, s);
			cout << "There is no such level, please choose from the options listed." << endl;
			continue;
		}
		else {
			cout << "There is no such level, please choose from the options listed." << endl;
			continue;
		}
	}
}

/*This function opens the words text files, checks for errors, loads the words into a vector called words, and then closes the words text file.*/
void Hangman::loadWords()
{
	ifstream input;
	input.open(wordsFile.c_str());

	if (input.fail()) {
		cout << "Could not open " << wordsFile;
		exit(1);
	}

	while (!input.eof()) {
		string s;
		input >> s;
		words.push_back(s);
	}

	input.close();
}


/*This function is the "meat" of the program. I couldn't figure out how to simplify it without messing up the entire game, so I'll describe each
step within the function.*/
/*
void Hangman::scrambler() {
//we need a while loop for this whole function so that when the word is completed it stops asking for guesses
	for (int i=0; i<words.size(); i++) {  //This loop takes a word from the words vector and puts it into a string called letters,
		string letters;                   //thus each letter of the word occupies its own index of the string. Note that the rest of the code
		letters = words[i];               //is contained in the body of this for loop, which I found to be necessary for the game to operate proper
                                          //memory snapshot: word cat is placed in string {c, a, t}

        vector<string> lettersVector;                  //Here I declare a vector of letters and fill the vector with the letters
		for (int i=0; i<letters.size(); i++){   //of the word from the words vector above.
			lettersVector.push_back(letters.substr(i, 1));    //memory snapshot: integer vector created {c, a, t}
		}

        vector<string> underscores;
		for (int i=0; i<letters.size(); i++) {
            underscores.push_back("_");
            cout<<underscores[i];
		}

		cout << endl;
		string guess;
        vector<string> hangman;
		int incorrectGuesses = 0; //Initializes local variables to record # of tries to guess letter

		while (guess != words[i]) {                 //while loop runs while user's guess is not the word

			cout<<"Guess a letter: ";
			cin >> guess;                           //allows user to input a string guess
			//Following for loop for converting uppercase to lower case found at http://www.cplusplus.com/forum/beginner/613/.
			for (int i=0; i<guess.length(); i++) {  //allows user to enter upper or lowercase letters
				guess[i]=tolower(guess[i]);         //converts uppercase letters to lowercase letters
			}

			//when the for loop is here it would check the letter for as many letters as in the word
			//and output "WRONG" x number of times
			int checking = 0;
			for (int i = 0; i < letters.size(); i++)
            {
                if (guess == lettersVector[i])
                {
                    checking++;
                    int index = i;
                    underscores[index] = lettersVector[i];
                }
            }
            if(checking > 0)
            {
                cout<<"You guessed a correct letter, keep going!"<<endl;
                int index = i;
                underscores[index] = lettersVector[i];
            }
            else if(checking <= 0)
            {
                cout <<"WRONG! That letter is not part of the word." << endl;
                while(!queueisEmpty())
                {
                    string bodypart = dequeueHangman();
                    for(int i=0; i <hangman.size(); i++)
                    {
                        hangman.push_back(bodypart);
                        cout << hangman[i] << endl;
                    }
                }
            }

            //this should be printed at the end of every guess no matter whether the guess was right or not
            for(int i=0; i<underscores.size(); i++)
            {
                cout<<underscores[i];
            }
            cout<<endl;  //adds an endline after the printout of the word
		}

		//totalTries += tries;      //sum of tries used per level

	} //end of for loop body, for loop ends when last word in words text file is unscrambled
}
*/
void Hangman::scrambler()
{
    bool dead = false;  //boolean to see if the player guessed too many incorrect guessed
    bool winner = false;  //boolean to see if the player has guessed the word correctly or not
    resetHangman();
    while(!dead || !winner)  //while the player hasn't lost but also hasn't won
    {
        for (int i=0; i<words.size(); i++)  //This loop takes a word from the words vector and puts it into a string called letters,
        {
            string letters;                   //each letter of the word occupies its own index of the string.
            letters = words[i];               //memory snapshot: word cat is placed in string {c, a, t}

            vector<string> lettersVector;  //declare a vector of letters and fill the vector with the letters of the word from the words vector
            for (int i=0; i<letters.size(); i++)
            {
                lettersVector.push_back(letters.substr(i, 1));    //memory snapshot: integer vector created {c, a, t}
            }

            vector<string> underscores;  //vector for the blank spaces representing the incomplete part(s) of the word
            for (int i=0; i<letters.size(); i++)
            {
                underscores.push_back("_");
                cout<<underscores[i];
            }

            cout << endl;
            string guess;  //this is the cin letter of the player's guess; gets overwritten with every new guess
            vector<string> hangman;
            int incorrectGuesses = 0;  //Initializes local variables to record # of tries to guess letter

            while (guess != words[i])  //while loop runs while user's guess is not the completed word
            {
                cout<<"Guess: ";
                cin >> guess;
                cin.clear();

                for (int i=0; i<guess.length(); i++)  //converts every input to lowercase
                {
                    guess[i]=tolower(guess[i]);
                }

                for (int i=0; i<letters.size(); i++)  //takes the player's guess and checks it against every letter in the word
                {
                    int index = 0;

                    if (guess == lettersVector[i])
                    {
                        cout<<"That's correct! ";
                        index = i;
                        underscores[index] = lettersVector[i];

                        for(int i=0; i<underscores.size(); i++)
                        {
                            cout<<underscores[i];
                        }
                        cout<<endl;
                        //if()  condition if all letters guessed correctly, sets winner bool to true
                    }

                    else if(guess != lettersVector[i] && i == index)//ERROR: condition will only run properly for first letter in word correct, else condition will always be called, even if guess is correct
                    {
                        //cout << guess << endl;
                        string bodypart = dequeueHangman();
                        cout << "WRONG! The " << bodypart << " was added to your hangman." << endl;
                        hangman.push_back(bodypart);
                        cout << "Your hangman has the following body parts: " << endl;

                        for(int i=0; i < hangman.size(); i++)
                        {
                            cout << hangman[i] << endl;
                        }

                        if(queueisEmpty())
                        {
                            cout << "GAME OVER! You have been hung!" << endl;
                            cout << " ____  " << endl;
                            cout << " |  |  " << endl;
                            cout << " |  O  " << endl;
                            cout << " | /|\\ " << endl;
                            cout << " | / \\ " << endl;
                            cout << " |     " << endl;
                            cout << "_|_____" << endl;
                            cout << endl;
                            dead = true;
                        }
                        break;
                    }
                }
            }
        } //end of for loop body, for loop ends when last word in words text file is unscrambled
    }
	initializeGame();
}

void Hangman::enqueueHangman()
{
    string body;
    ifstream bodyFile("body.txt");

    int i = 0;
    while(!queueisFull())
    {
        getline(bodyFile, body, ',');
        bodyQueue[queueTail] = body;
        queueTail++;
    }
}

string Hangman::dequeueHangman()
{
    string dequeueText;
    if(queueHead == queueSize-1)
    {
        queueHead = 0;
    }
    else
    {
        dequeueText = bodyQueue[queueHead];
        bodyQueue[queueHead] = " ";
        queueHead++;
    }
    return dequeueText;
}

bool Hangman::queueisFull()
{
    if(queueTail == queueSize-1)
        return true;
    else
        return false;
}

bool Hangman::queueisEmpty()
{
    if(queueHead == queueTail)
        return true;
    else
        return false;
}

/*Function to print results based on final score.*/
void Hangman::results() const
{
	if (totalTries == 0 || totalTries < 60)
		cout << "Keep trying, you can get a better score!" << endl;
	else if (totalTries == 100)
		cout << "You're the master at Hangman!" << endl;
	else if (totalTries < 100 && totalTries >= 80)
		cout << "Great job!" << endl;
	else
		cout << "Nice try!" << endl;
}

void Hangman::resetHangman()
{
    int i = 0;
    while(!queueisEmpty())
    {
        bodyQueue[i] = " ";
        queueHead++;
        i++;
    }

    while(!queueisFull())
    {
       enqueueHangman();
    }
}

/*my thoughts on the fix for the letter check:
I think the check for correct letters should be it's own loop (or function) with a variable saving whether there were right letters or not;
Then the incorrect guesses should be it's own separate loop (or function) as well. This we we have the function check for whether guessed
letter exists in the word; if it does then no need to run the second one, if it doesn't, then run the incorrect guess loop/function. This
way we can count on the checker going through every single letter to see if it's there before deciding whether it's a correct guess or not.
I also suggest that we only put words in the word text bank that doesn't have repeat letters...if it's possible. It'll make it uniquely hard
for the player to guess, but it will also make it easier for us so we don't have to worry about the player getting more than one letter
correct in a single guess...makes for a simpler function.
*/

/*pseudo code for hangman figure
7x7 2-D array
Default Image:
    cout << " ____  " << endl;
    cout << " |  |  " << endl;
    cout << " |     " << endl;
    cout << " |     " << endl;
    cout << " |     " << endl;
    cout << " |     " << endl;
    cout << "_|_____" << endl;

Completed Image:
    cout << " ____  " << endl;
    cout << " |  |  " << endl;
    cout << " |  O  " << endl;
    cout << " | /|\\ " << endl;
    cout << " | / \\ " << endl;
    cout << " |     " << endl;
    cout << "_|_____" << endl;

Idea of implementation:
    array[x][y]
    for i = 0, i++{
        for j = 0 j++{
            cout array[i][j]
        }
    }

Need to figure out how to add the appropriate symbols in the correct spots;
I think it may be easiest to have predefined arrays that replace the existing arrays, for instance:
    first wrong guess, replace the third array with one that has a head in it;
    second wrong guess replace the fourth array with one that has a torso in it;
    third wrong guess replace the fourth array with one that has a torso and left arm;
    so-on.
Benefit is that we don't have to tell the array where to add body parts;
Downside is that we have to explicitly predefine each possible array combo;
*/

/*pseudo for scoring
-counter for each time the game is started; +1 once the man is hanged but the program is still rerun
    -this counter determines the size of the score-keeping array
-second counter that is +1 for every word that is completely guessed
    -this counter gets reset when the man is hanged
    -before the counter reset, the value gets stored in the array in the position equivalent to the first counter's value
-at the end of every game, the score is printed with some text congratulating the player
-at the end of every game after the first, the previous highest score is printed as well
    -if previous highest is greater than latest score, "slacker/motivational"-style comment printed out
    -if previous highest is less than the latest score, congratulatory comment printed out
SCORING COMPLETE
*/




