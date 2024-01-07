#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <cmath>
#include <vector>
#include "structures.h"
#include "table.h"
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


void AESEncrypt(unsigned char *message, unsigned char *expandedKey, unsigned char *encryptedMessage, ofstream &inspectionFile) {

    inspectionFile<<"Encryption Lab"<<"\n" ;
    unsigned char state[16];
    unsigned char temp[16];
    unsigned char xtemp[16];

	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}

     inspectionFile <<"Each 16 divisions : " ;
	printState(state,inspectionFile);

	int numberOfRounds = 9;



	for (int i = 0; i < numberOfRounds; i++) {



        inspectionFile<< "----------------------Round NO: " << i << " ----------------------" << "\n" ;
        if (i>0){
            for (int i = 0; i < 16; i++) {

                state[i]= inverseFunction(state[i]) ;
            }
            inspectionFile << "Inversing : " ;
            printState(state, inspectionFile);
        }

        inspectionFile<< "Starting state of this round: ";
        printState(state, inspectionFile);

		encryptionRound(state, expandedKey + (16 * i),Tbox_round,i,Ty_Table,xorTable, inspectionFile);

         inspectionFile<< "Before bijecting: ";
        printState(state, inspectionFile);
        for (int i = 0; i < 16; i++) {

            state[i]= evaluateFunction(state[i]) ;
        }

        inspectionFile<< "After bijecting: ";
        printState(state, inspectionFile);
	}

    inspectionFile<< "----------------------Final Round----------------------"<< "\n" ;


    for (int i = 0; i < 16; i++) {
        state[i]= inverseFunction(state[i]) ;
    }
	inspectionFile<< "Inversing function: ";
	printState(state,inspectionFile);

    finalRound(state,Tbox_final, inspectionFile);


	inspectionFile << "State after final round : "  ;
    printState(state, inspectionFile);

	// Copy encrypted state to buffer
	for (int i = 0; i < 16; i++) {
		encryptedMessage[i] = state[i];
	}
   inspectionFile <<"Encryption Done"<<"\n" ;

}

void incrementCounter(unsigned char counter[], int size) {

    for (int i = size - 1; i >= 0; --i) {
        counter[i]++;
        if (counter[i] != 0x00) {
            break;
        }
        counter[i] = 0x00;
    }
}


int main() {

        std::ofstream inspectionFile("inspection_CTR.txt", ios::out);
    if (!inspectionFile.is_open()) {
        inspectionFile << "Unable to open the inspection file." << endl;
        return 1;
    }

    inspectionFile << "   128-bit AES Encryption Tool Inspection Report   " << endl;
    inspectionFile << "==================================================" << endl;

	char message[1024];

	cout << "Enter the message to encrypt: ";
	cin.getline(message, sizeof(message));
	inspectionFile << message << endl;
    int originalLen = strlen((const char *)message);
    inspectionFile << "Firstly message is converted into hex : ";
    for (int j = 0; j < originalLen; j++) {
        inspectionFile << hex << setw(2) << setfill('0') << static_cast<int>(message[j]) << " ";
    }
    inspectionFile << endl;
	// Pad message to 16 bytes

	int paddedMessageLen = originalLen;

	if ((paddedMessageLen % 16) != 0) {
		paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
	}

	unsigned char * paddedMessage = new unsigned char[paddedMessageLen];
	for (int i = 0; i < paddedMessageLen; i++) {
		if (i >= originalLen) {
			paddedMessage[i] = 0;
		}
		else {
			paddedMessage[i] = message[i];
		}
	}
    inspectionFile << "Message in hex of 16 chunks: ";
    for (int j = 0; j < paddedMessageLen; j++) {
        inspectionFile << hex << setw(2) << setfill('0') << static_cast<int>(paddedMessage[j]) << " ";
    }
    inspectionFile << endl << endl;

    inspectionFile << "\n" ;
	unsigned char * encryptedMessage = new unsigned char[paddedMessageLen];
	unsigned char counter[16] = {0};


	for (int i = 0; i < paddedMessageLen; i += 16) {
		AESEncrypt(counter, expandedKey, encryptedMessage+i,inspectionFile);
		//AESEncrypt(paddedMessage+i, expandedKey, encryptedMessage+i);

		for (int j = 0; j < 16; j++) {
		//inspectionFile<< (i*16)+j <<"\n" ;



		encryptedMessage[(i*16)+j] = xorTable[encryptedMessage[(i*16)+j]][paddedMessage[(i*16)+j]];

		incrementCounter(counter, 16);
	          }
	}
    inspectionFile << "Encrypted message in hex: ";
    for (int i = 0; i < paddedMessageLen; i++) {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(encryptedMessage[i]) << " ";
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

