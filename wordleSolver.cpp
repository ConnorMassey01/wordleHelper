#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

/*
TODO

*/

#define LETTER_SUM = 2315*5; //number of words in the words.txt file times 5 letters per word

//global variable
vector<string> availableWords;
vector<int> letterCount;

void getWords(){
    //open the file
    string fileName = "words.txt";
    fstream file;
    file.open(fileName);
    if(file.fail()){
		cout << "could not open file\n";
        //exit the program as there is no reason to go forward from here
		exit(1);
	} 
    //extract the words from the file
    string inputWord;
    while(getline(file, inputWord)){
        availableWords.push_back(inputWord);
    }
    //close the file
    file.close();
}

void calculateLetterFrequency(){
    //initialize letterCount to 26 zeros
    for(int i = 0; i < 26; i++){
        letterCount.push_back(0);
    }
    for(int i = 0; i < availableWords.size(); i++){
        for(int j = 0; j < availableWords[i].size(); j++){
            //increase at each instance of a letter being found
            letterCount[availableWords[i].at(j) - 'a'] ++;
        }
    }
}

void updateAvailableWords(const string& inputWord, const string& gameFeedback){
    //variable to hold characters not in the answer
    vector<char> white;
    //variable to hold characters in the answer but not in correct place
    vector<pair<int, char>> yellow;
    //variable to hold characters in the correct place, pair holds the index of the character and its position
    vector<pair<int, char>> green;
    //set up the variables
    for(int i = 0; i < inputWord.length(); i++){
        if(gameFeedback[i] == 'w'){
            white.push_back(inputWord[i]);
        }
        else if(gameFeedback[i] == 'g'){
            green.push_back({i, inputWord[i]});
        }
        else{
            yellow.push_back({i, inputWord[i]});
        }
    }
    //search through all available words to update the list
    for(int i = 0; i < availableWords.size(); i++){
        //remove all the words in available words that contain characters found in remove
        for(int j = 0; j < white.size(); j++){
            //check if character from remove is within the available word
            if(availableWords[i].find(white[j]) != string::npos){
                //overwrite word at i
                availableWords[i] = availableWords.back();
                //move i back one to check the same spot again;
                i--;
                //remove the back of the list
                availableWords.pop_back();
                //set j to break the inner for loop as the word does not have to be removed twice
                j = white.size();
            }
        }
    }
    //search through all available words to update the list
    for(int i = 0; i < availableWords.size(); i++){
        //remove all words that dont contain a character stored in yellow
        for(int j = 0; j < yellow.size(); j++){
            //check if character that should be in the words somewhere is not
            //also remove if available word has character in same position as it should not be there
            if(availableWords[i].find(yellow[j].second) == string::npos || availableWords[i].at(yellow[j].first) == yellow[j].second){
                //overwrite word at i
                availableWords[i] = availableWords.back();
                //move i back one to check the same spot again;
                i--;
                //remove the back of the list
                availableWords.pop_back();
                //set j to break the inner for loop as the word does not have to be removed twice
                j = yellow.size();
            }
        }
    }
    //search through all available words to update the list
    for(int i = 0; i < availableWords.size(); i++){
        //remove all words that don't contain characters in the known correct place
        for(int j = 0; j < green.size(); j++){
            //check if word does not have character in correct place
            if(availableWords[i].at(green[j].first) != green[j].second){
                //overwrite word at i
                availableWords[i] = availableWords.back();
                //move i back one to check the same spot again;
                i--;
                //remove the back of the list
                availableWords.pop_back();
                //set j to break the inner for loop as the word does not have to be removed twice
                j = green.size();
            }
        }
    }
}

//if a's score is greater than b's score return true
bool compareScore(string a, string b){
    int scoreA = 0;
    int scoreB = 0;
    //calculate the scores
    for(int i = 0; i < a.length(); i++){
        scoreA += letterCount[a[i] - 'a'];
        scoreB += letterCount[b[i] - 'a'];
    }
    return scoreA > scoreB;
}

void sortAvailableWords(){
    //give each available letter a score based on the letters in the word
    sort(availableWords.begin(), availableWords.end(), compareScore);
}

void displayAvailableWords(){
    cout << '\n';
     int score = 0;
    //loop through all available words and print them to the console
    for(int i = 0; i < availableWords.size(); i++){
        //calculate the scores
        score = 0;
        for(int j = 0; j < availableWords[i].length(); j++){
            score += letterCount[availableWords[i].at(j) - 'a'];
        }
        cout << setw(5) << left << availableWords[i] << ": " << setw(8)  << score;
        if(i%6 == 5) cout << '\n';
        if(i == 100) break;
    }
    cout << "\n\n";
}

void reset(){
    //clear the available words
    availableWords.clear();
    //get the words from the file again
    getWords();
}

void gameLoop(){
    string inputWord;
    string gameFeedback;
    cout << "Instructions\n";
    cout << "-Enter the 5 letter word when prompted\n";
    cout << "-Enter the game result in the form of each colour result e.g ggywy\n";
    cout << "-Enter 0 when prompted to state the input word to reset to the initial state\n";
    cout << "-Enter 1 when prompted to state the input word to quit\n";

    while(true){
        inputWord = "";
        gameFeedback = "";
        //get word input
        while(inputWord.length() != 5){
            cout << "State the input word: ";
            cin >> inputWord;
            if(!inputWord.empty() && inputWord[0] == '0'){
                reset();
                cout << "Reset successful\n";
            }
            else if(!inputWord.empty() && inputWord[0] == '1'){
                cout << "Quit\n";
                exit(1);
            }
        }   
        //get letters from game feedback and check that it is given correctly
        while(gameFeedback.length() != 5){
            cout << "State the game result: ";
            cin >> gameFeedback;
            for(int i = 0; i < gameFeedback.length(); i++){
                if(gameFeedback[i] != 'w' && gameFeedback[i] != 'y' && gameFeedback[i] != 'g'){
                    cout << "Must only use 'w' 'y' or 'g' for input. Try again\n";
                    //clear input and skip to end of loop
                    gameFeedback = "";
                    i = gameFeedback.size();
                }
            }
        }
        //update available options
        updateAvailableWords(inputWord, gameFeedback);

        //sort the available words by best choice 
        sortAvailableWords();

        //show top 100 available options
        displayAvailableWords();
    }
   
}

int main(){
    //get all the words in the file
    getWords();
    //calculate the frequency of each letter
    calculateLetterFrequency();
    //game loop to get input from user and show available words
    gameLoop(); 
}