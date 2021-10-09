/*
Brittany Bergeron
CS 3318
project 2: Ceaser Cipher

Purpose:	To solve an encrypted ceaser cipher text file.
Input:		Takes input from cipher.txt
Output:		Gives output to solvedCipher.txt
*/

//headers
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

//declare functions
bool verifyKey(int key, string cipherMod);
int findKey(string cipherMod);


int main(){

	//string to hold cipher input
	std::string cipherIn;
	//string to hold final result from shift
	std::string cipherResult;
	//input file stream reads cipher.txt
	std::ifstream inFile("cipher.txt");

	//error check opening of file
	if(inFile.fail()){
		cerr << "error opening file\n" << endl;
		exit(1);
	}

	//copy file text into string
	string hold;
	while(inFile >> hold){
		cipherIn.append(hold);
		cipherIn.append(" ");
	}
	//close file stream
	inFile.close();

	//show given text document before shift
	cout << "Given cipher from text file cipher.txt: \n" << cipherIn << endl << endl;
	//copy cipher to manipulate; save seperately to make sure input value not corrupted
	string cipherMod = cipherIn;

	//store a copy to lower case for easily finding key
	for(int i=0; i<cipherMod.length(); i++){
		cipherMod[i] = tolower(cipherMod[i]);
	}

	//begin working on key with finKey() function
	int key = findKey(cipherMod);

/* METHOD FOR PROPER SHIFTING W SPEC CHAR*/
	//hold current character for modification
	char current;
	//loop through cypherIn values and shift appropriate alphabet characters by key
	for(int i=0; i<cipherIn.length(); i++){
		//make sure alphabet letters; either lowercase (97-122) or uppercase (65-90)

		//lowercase
		if(cipherIn[i]>96 && cipherIn[i]<123){
			//account for wrap around values; adding the key made the value greater than 'z' or 122
			if( (cipherIn[i]+key) > 122){
				//add remainder from key before 'a' or 97
				current = 96+(key- (122-cipherIn[i]) );
			}
			//no wrap around, add as usual
			else{
				current = cipherIn[i]+key;
			}
			//append current character to cipherResult
			cipherResult += current;
		}

		//uppercase
		else if(cipherIn[i]>64 && cipherIn[i]<91){
			//account for wrap around values; adding the key made the value greater than 'Z' or 90
			if( (cipherIn[i]+key) > 90){
				//add remainder from key before 'A' or 65
				current = 64+(key- (90-cipherIn[i]) );
			}
			//no wrap around, add as usual
			else{
				current = cipherIn[i]+key;
			}
			//append current character to cipherResult
			cipherResult += current;
		}

		//non letter value (spaces, numbers, special characters)
		else{
			//append current character to result; no shift needed
			cipherResult += cipherIn[i];
		}
	}

	//return result to screen
	cout << "Key: " << key << "\nSolved cipher:\n" << cipherResult << "\n\n" << "Result saved to solvedCipher.txt";

	//write to file
	ofstream solvedFile;
	solvedFile.open("solvedCipher.txt");
	solvedFile << cipherResult;
	solvedFile.close();

	return 0;
}

//used to return key; uses seperate function verifyKey() for testing and shifting
int findKey(string cipherMod){

	//hold possible key
	int key;
	//how many times tested (this corrolates to index of commonLetters)
	int testNum = 0;
	//has key been verified correct?
	bool keyFound = false;
	//hold possible result
	string cipherResult;
	//e,t,a,r,i,o are ascii values 101,116,97,114,105,111 ~these are the most common letters in order (from wikipedia)
	char commonLetters[] = {'e','t','a','r','i','o'};
	//create and fill an array to store occurances of letters in alphabet
		//in this array, a is stored at index 0 and z at index 25; be sure to account for a +97 shift to ascii code
	int alphabet[26];
	for(int i=0; i<26; i++){
		alphabet[i] = 0;
	}

	//iterate through cipherMod and tally the letter occurances in their places
	for(int i=0; i<cipherMod.length(); i++){
		alphabet[cipherMod[i]-97]++;
	}

	//which letter has highest occurances?
	int mostOccurances = 0;
	//store the index to refer to this letter later
	int index;
	//iterate through array, find most used letter in cipher string
	for(int i=0; i<26; i++){
		if(alphabet[i] > mostOccurances){
			mostOccurances = alphabet[i];
			index = i;
		}
	}

	//index found: this is going to be tested as the most common letters, or each letter in commonLetters[]
	while((keyFound != true) && (testNum < 6)){
		//find the shift by subtracting values
		//check to see if the index of reoccuring letters is right of the leter being tested (wraps around aplhabet)
		if( (commonLetters[testNum]-97) < index){
			key = (26-index)+(commonLetters[testNum]-97);
		}
		//shifting left from index, value can be subtracted normally
		else{
			key = (index) - (commonLetters[testNum]-97);
		}

		//check this key
		keyFound = verifyKey(key, cipherMod);
		//if keyFound == false, test will happen again with next common letter
		testNum++;
	}
	return key;
}

//function shifts and checks cipher to see if common short words match the seperated values in the cipher
bool verifyKey(int key, string cipherMod){
	//array of common words to be tested
	string commonWords[] = {"a", "i", "be", "to", "in", "it", "is" "of", "the", "and", "for", "are", "but", "not"};
	//keyFound set to false; returns true when conditions met
	bool keyFound = false;
	//string to hold and check result
	string cipherResult;

	//store current character value (one at a time) to analyze and manipulate
	char current;
	//attempt shift; string will still be lowercase here; not including special characters for easy analysis
	for(int i=0; i<cipherMod.length(); i++){
		//make sure lowercase alphabet letters
		if(cipherMod[i]>96 && cipherMod[i]<123){
			//account for wrap around values; adding the key makes the value greater than 'z' or 122
			if( (cipherMod[i]+key) > 122){
				//add remainder from key after 'a' or 97
				current = 96+(key- (122-cipherMod[i]) );
			}
			//no wrap around, add as usual
			else{
				current = cipherMod[i]+key;
			}
			//append current character to result
			cipherResult += current;
		}
		//preserve space (ascii 32) for word seperation
		else if(cipherMod[i] == 32){
			cipherResult += " ";
		}
	}

/*iterate through stream and compare to common words : 1, 2, and 3 letter words should match at high rate if success*/
	//string stream to read result delimited by spaces
	istringstream iss(cipherResult);
	//hold current word in cipher to compare
	string currentWord;
	//values that will give a relative match success rate
	double positiveHits=0.0;
	double negativeHits=1.0;	//cannot divide by 0; set to 1
	//make sure basic check for one letter words pass to reduce false positive result
	bool passedTest = true;

	do{
		iss >> currentWord;
		//word length one only has 'a' and 'i' as option; if not these discard key possibility; break and return
		if(currentWord.length() == 1){
			//must be "a" or "i" ~ index 0-1
			if( (currentWord.compare(commonWords[0])!=0) && (currentWord.compare(commonWords[1])!=0) ){
				//cannot proceed without pass
				passedTest = false;
				break;
			}
		}

		//length 2 words tested for common word matches
		else if(currentWord.length() == 2){
			//likely "be", "to", "in", "it", "is" or "of" ~ index 2-7
			for(int i=2; i<8; i++){
				//take into account if word matches or not
				if(currentWord.compare(commonWords[i]) == 0){
					positiveHits++;
				}
				else{
					negativeHits++;
				}
			}
		}
		//length 3 words tested for common word matches
		else if(currentWord.length() == 3){
			//likely "the", "and", "for", "are", "but", "not" ~ index 8-13
			for(int i=8; i<14; i++){
				//take into account if word matches or not
				if(currentWord.compare(commonWords[i]) == 0){
					positiveHits++;
				}
				else{
					negativeHits++;
				}
			}
		}
	}while (iss);


	//find final ratio of matches;
	double ratio = positiveHits/negativeHits;
	//fails tend to be under 0.15, success tends to be above 0.07 for long text, 0.5 for short text
	if( passedTest && (ratio > 0.05) ){
		keyFound = true;
	}

	//return with best fit key
	return keyFound;
}
