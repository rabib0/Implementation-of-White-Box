#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <cmath>
#include <vector>
#include "table.h"
#include <cstdlib>  
#include <ctime>  
#include "structures.h"
#include "encoding.h"

using namespace std;

ofstream inspectionFile;


void printState(unsigned char *state, ofstream &inspectionFile) {
    for (int i = 0; i < 16; i++) {
        inspectionFile << hex << setw(2) << setfill('0') << static_cast<int>(state[i]) << " ";
    }
    inspectionFile << endl;
    inspectionFile << endl;
    inspectionFile << "\n";
}


void encryptionRound(unsigned char *state, unsigned char *roundKey, unsigned char ***Ttable, int roundNumber,
                    unsigned char ***Ty, unsigned char **xorTable, ofstream &inspectionFile) {

    inspectionFile << "Round Key: ";
    printState(roundKey, inspectionFile);

    // Apply ShiftRows operation
    ShiftRows(state);
    inspectionFile << "After ShiftRows: ";
    printState(state, inspectionFile);

    // Apply T-boxes and Ty-table operations
    inspectionFile << "After T-boxes and Ty-table: ";
    TBoxestyiTables(state, Ttable, roundNumber, Ty, xorTable);
    printState(state, inspectionFile);
}


void finalRound(unsigned char *state, unsigned char ***TboxTable, ofstream &inspectionFile) {
    // Apply ShiftRows operation
    ShiftRows(state);
    inspectionFile << "After ShiftRows: ";
    printState(state, inspectionFile);

    // Apply T-box operation for the final round
    inspectionFile << "After T-box for the Final Round: ";
    TboxFinal(state, TboxTable);
    printState(state, inspectionFile);
}






/* The AES encryption function
 * Organizes the confusion and diffusion steps into one function
 */
void AESEncrypt(unsigned char *message, unsigned char *expandedKey, unsigned char *encryptedMessage,int numberOfRounds, ofstream &inspectionFile) {
	unsigned char state[16];
	unsigned char temp[16];
	unsigned char xtemp[16];

	// Copy the input message to the state array
	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}

    // Log the initial state
	inspectionFile << "Each 16 divisions : ";
	printState(state, inspectionFile);

	// Number of rounds excluding the final round

	// Loop through each round
	for (int round = 0; round < numberOfRounds; round++) {
		inspectionFile << "----------------------Round NO: " << round << " ----------------------" << "\n";
		if (round==9){
			for (int i = 0; i < 16; i++) {
				temp[i]=state[i];
		
		 	}
		 	
		}
		// Apply the inverse function on each byte of the state (except in the first round)
		if (round > 0) {
		    for (int i = 0; i < 16; i++) {
		        state[i] = inverse_function(state[i]);
		    }
		    inspectionFile << "Inversing : ";
		    printState(state, inspectionFile);
		}
		// Log the starting state of the current round
		inspectionFile << "Starting state of this round: ";
		printState(state, inspectionFile);

		if (round<9){
			encryptionRound(state, expandedKey+ (16 * round) , Tbox_round, round, Ty_Table, xorTable, inspectionFile);
			}
			
			
		else{
			encryptionRound(state, expandedKey+ (16 * round) , Tbox_round, 1, Ty_Table, xorTable, inspectionFile);
		}
		// Log the state before bijecting
		inspectionFile << "Before bijecting: ";
		printState(state, inspectionFile);

		// Apply the bijective function on each byte of the state
		for (int i = 0; i < 16; i++) {
		    state[i] = evaluate_function(state[i]);
		}

		// Log the state after bijecting
		inspectionFile << "After bijecting: ";
		printState(state, inspectionFile);
	}
	
	for (int i = 0; i < 16; i++) {
		state[i]=temp[i];
		
	}
	
	
	// Log the final round
	inspectionFile << "----------------------Final Round----------------------"<< "\n";

	    // Apply the inverse function on each byte of the state
	    for (int i = 0; i < 16; i++) {
		state[i] = inverse_function(state[i]);
	    }
		inspectionFile << "Inversing function: ";
		printState(state, inspectionFile);

	    // Perform the final round
	    finalRound(state, Tbox_final, inspectionFile);
		inspectionFile << "State after final round : ";
	    printState(state, inspectionFile);

		// Copy the encrypted state to the output buffer
		for (int i = 0; i < 16; i++) {
			encryptedMessage[i] = state[i];
		}
	}






int main() {
    ofstream inspectionFile("inspection_confusingRounds.txt", ios::out);
    if (!inspectionFile.is_open()) {
        cout << "Unable to open the inspection file." << endl;
        return 1;
    }

    inspectionFile << "   128-bit AES Encryption Tool Inspection Report   " << endl;
    inspectionFile << "==================================================" << endl;

    char message[1024];

    cout << "Enter the message to encrypt: ";
    cin.getline(message, sizeof(message));
    inspectionFile << "Entered message: " << message << endl;

    int originalLen = strlen((const char *)message);

    inspectionFile << "Message in hex: ";
    for (int j = 0; j < originalLen; j++) {
        inspectionFile << hex << setw(2) << setfill('0') << static_cast<int>(message[j]) << " ";
    }
    inspectionFile << endl;

    // Pad message to 16 bytes
    int paddedMessageLen = originalLen;
    if ((paddedMessageLen % 16) != 0) {
        paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
    }

    unsigned char *paddedMessage = new unsigned char[paddedMessageLen];

    for (int i = 0; i < paddedMessageLen; i++) {
        paddedMessage[i] = (i >= originalLen) ? 0 : message[i];
    }

    inspectionFile << "Message in hex of 16 chunks: ";
    for (int j = 0; j < paddedMessageLen; j++) {
        inspectionFile << hex << setw(2) << setfill('0') << static_cast<int>(paddedMessage[j]) << " ";
    }
    inspectionFile << endl << endl;

    unsigned char *encryptedMessage = new unsigned char[paddedMessageLen];
    
    
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    int numberOfRounds = generateRandomNumber(12, 20);
    

    // AESEncrypt function with inspectionFile as an additional parameter
    for (int i = 0; i < paddedMessageLen; i += 16) {
        AESEncrypt(paddedMessage + i, expandedKey, encryptedMessage + i, numberOfRounds,inspectionFile);
    }

    inspectionFile << "Encrypted message in hex: ";
    for (int i = 0; i < paddedMessageLen; i++) {
        inspectionFile << hex << setw(2) << setfill('0') << static_cast<int>(encryptedMessage[i]) << " ";
    }
    inspectionFile << endl << endl;

    // Write the encrypted string to the output file "output.txt"
    ofstream outfile("output.txt", ios::out | ios::binary);

    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(encryptedMessage), paddedMessageLen);
        outfile.close();
        inspectionFile << "Wrote encrypted message to the file 'output.txt'" << endl;
    } else {
        inspectionFile << "Unable to open the output file." << endl;
    }

    // Close the inspection file
    inspectionFile.close();

    // Free memory
    delete[] paddedMessage;
    delete[] encryptedMessage;

    return 0;
}
