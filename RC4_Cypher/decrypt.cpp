/*
Brittany Bergeron
CS 3318 ~ Project 3, RC4 DECRYPTION

Goal:
	Implement the RC4 stream cipher in C++. 
	User should be able to enter any key that is 5 bytes to 32 bytes long. 
	Be sure to discard the first 3072 bytes of the pseudo random numbers. 
	THE KEY OR THE INPUT TEXT MUST NOT BE HARD CODED IN THE PROGRAM.
	The decryption program should input the cipher text file in hex and output the plaintext.
	
Input includes user entered key(5-32 bytes), and encryptedMessage.txt located by the program.
Output is encrypted hex cypher decryptedMessage.txt
*/

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

int main(){
	
	//hold given secret key
	string key;
	//hold message to decrypt
	string message;
	
	//prompt for key input from user and save as key
	cout<<"Enter key between 5 and 32 bytes:\n";
	getline(cin, key);
	//get size of key
	int keyLength= key.size();
	//check if key size valid, then report error if not
	if( (keyLength>32 ) || (keyLength<5)){
		cerr<<"Invalid key size\n";
		exit(1);
	}
	
	//take in file w/ cypher
	ifstream inFile("encryptedMessage.txt");
	//error handling
	if(inFile.fail()){
		cerr<<"Error opening file\n";
		exit(1);
	}
	//hold cypher pieces in string for decrypting
	string hold;
	while(inFile>>hold){
		//entire hex appends to string (spaces were encrypted/given hex value)
		message.append(hold);		
	}
	//close file
	inFile.close();
	
	//show user the cypher that is being decrypted from file
	cout << "\nDecrypting hex message from file encryptedMessage.txt:\n" << message << endl;
	
	/*Step 1: 
		convert hex back to ascii
	*/
	//string cypher holds converted ascii from hex value
	string cypher;
	//iterate through input message, every 2 chars in hex string is one byte
	for(int i=0; i<message.size(); i+=2){
		//take next one byte from hex message
		string hold = message.substr(i,2);	
		//convert from hex using string to unsigned long; automatically get desired char value
		char c = stoul(hold, nullptr, 16);
		//append the char to the cypher message
		cypher.append(1,c);
	}
	
	
	/*Step 2:
		permutate all 256 possible bytes w/ variabe klength key 5-32 bytes (40-256 bits)
		use KSA (key-scheduling algorithm)	
	*/
	//arrays that will be used to map keys in step 2
	int S[256];
	char T[256];
	//fill arrays
	for(int i=0; i<256; i++){
		S[i]=i;
		//repeat key as necessary to fill array size 256 using %
		T[i] = key[i%keyLength];
	}
	//hold temp index to swap
	int k=0;
	//permutate array
	for(int i=0; i<256; i++){
		k=(k+S[i] + T[i])%256;
		swap(S[i], S[k]);
	}

	/*Step 3:
		//Stream generation
	*/
	//variable for random #
	int randNum;
	//variable for resulting plain text
	string result;
	//next byte of text to append to plain text 
	string v;
	//counting variable for formula below
	int i=0;
	//add 3072 to the message size for secure random (skips first 3072 keys generated)
	for(int m=0; m<cypher.size()+3072; m++){
		//use n to create a new random index each iteration
		int n = (i++)%256;
		//use index n in mapped array to generate another index #
		int j = (j+S[n])%256;
		//swap the 2 indexes to alter next iterations
		swap(S[n],S[j]);
		//store random number
		int t = (S[n] + S[j])% 256;
		//store random number
		randNum = S[t];
		//discard first 3072 bytes
		if(m>=3072){
			//random number XOR the current cypher byte gives decrypted value
			v=randNum^cypher[m-3072];
			//add decryption to result string
			result.append(v);
		}
	}
	
	//inform user of resulting decryption and text location
	cout<<"\nMessage decrypted:\n" << result << endl << "Decryption saved to decryptedMessage.txt\n"<<endl;
	
	//write plain text encryption to file
	ofstream decryptedFile;
	decryptedFile.open("decryptedMessage.txt");
	decryptedFile << result;
	decryptedFile.close();
	
	return 0;
}