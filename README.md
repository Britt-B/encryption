# Encryption
## Ceasar Cypher
Ceasar Cypher solves an encrypted message from cipher.txt by determining possible key values. The input is scanned and compared to common (english) language trends such as letter and small word frequency. The letters e, t, a, r, i, and o are tested as possible key shift values. The most frequent 1, 2, and 3  letter words are used to verify the correctness of the key shift. A success ratio is calculated (greater than 0.05 passes) and the output is printed to console and solvedFile.txt.
### Run Locally:
* g++ -o ceasar ceasar.cpp
* ./ceasar
***
## Diffie Hellman
The key exchange is meant to create a secure key exchange between 2 users. The first prompt asks each user to enter 2 prime numbers between 4000 and 10000 (verified by primes.txt). They then must enter 2 secret keys between 40 and 100. The shared key is calculated as a big power and returned to the users.
### Run Locally:
* g++ -o DHexchange DHexchange.cpp
* ./DHexchange
***
## RC4 Cypher
(Using encrypt.cpp) the user enters any key between 5 and 32 bytes. message.txt is read and a key scheduling algorithm is used to permutate possible key mappings. A stream generation encrypts the message and converts the text to hexadecimal. The message is stored in encryptedMessage.txt. decrypt.cpp reverses this process by converting the hex from encryptedMessage.txt back to ascii, using the key to generate mappings, and streaming the decrypted message to decryptedMessage.txt.
### Run Locally:
* g++ encrypt -o encrypt.cpp
* ./encrypt
* g++ decrypt -o decrypt.cpp
* ./decrypt
