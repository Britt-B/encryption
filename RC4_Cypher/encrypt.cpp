/*
Brittany Bergeron
CS 3318 ~ Project 3, RC4 ENCRYPTION

Goal:
	Implement the RC4 stream cipher in C++. 
	User should be able to enter any key that is 5 bytes to 32 bytes long. 
	Be sure to discard the first 3072 bytes of the pseudo random numbers. 
	THE KEY OR THE INPUT TEXT MUST NOT BE HARD CODED IN THE PROGRAM.
	The encryption program should input the plaintext file and output a cipher text in hex.
	
Input includes user entered key(5-32 bytes), and message.txt located by the program.
Output is encrypted hex cypher encryptedMessage.txt
*/

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main(){
	//hold given secret key
	string key;
	//hold message to encrypt
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
	
	//take in file w/ message
	ifstream inFile("message.txt");
	//error handling
	if(inFile.fail()){
		cerr<<"Error opening file\n";
		exit(1);
	}
	//create variable to hold message pieces
	string hold;
	while(inFile>>hold){
	//append next 'word' (no spaces)
		message.append(hold);
		//add space after
		message.append(" ");
	}
	//close file
	inFile.close();
	//remove final extra space
	message = message.substr(0, message.size()-1);
	
	//show user the message that is being encrypted from file
	cout << "\nEncrypting message from file message.txt:\n" << message << endl;
	
	
	/*Step 1:
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

	/*Step 2:
		//Stream generation
	*/
	//variable for random #
	int randNum;
	//variable for resulting cypher text
	string cypher;
	//next byte of text to append to cypher
	string v;
	//counting variable for formula below
	int i=0;
	//add 3072 to the message size for secure random (skips first 3072 keys generated)
	for(int m=0; m<message.size()+3072; m++){
		//use n to create a new random index each iteration
		int n = ((i++)%256);
		//use index n in mapped array to generate another index #
		int j = (j+S[n])%256;
		//swap the 2 indexes to alter next iterations
		swap(S[n],S[j]);
		//add and % for final index reference this iteration
		int t = (S[n] + S[j])% 256;
		//store random number
		randNum = S[t];
		//discard first 3072 bytes
		if(m>=3072){
			//random number XOR the current message byte gives encrypted value
			v=randNum^message[m-3072];
			//add to cypher string
			cypher.append(v);
		}
	}

	/*Step 3: 
		convert string of char to hex
	*/
	//holds hex(2bytes for 0x, 2 for numbers
	char holdHex[4];
	//store in result
	string result;
	//iterate through cypher text
	for(int i=0; i<cypher.size(); i++){
		//need to cast for hex; use unsigned int or will fill negative bits with FFFF...
		sprintf(holdHex,"%02X", (int)(*(unsigned char*)(&cypher[i])));
		//append resulting hexadecimal to cypher (does not include spaces or '0x'
		result.append(holdHex);
	}
	
	//inform user of resulting encryption and text location
	cout<<"\nMessage encrypted in hex:\n" << result << endl << "Encryption saved to encryptedMessage.txt\n"<<endl;
	
	//write hex encryption to file
	ofstream encryptedFile;
	encryptedFile.open("encryptedMessage.txt");
	encryptedFile << hex << result;
	encryptedFile.close();
	
	return 0;
}