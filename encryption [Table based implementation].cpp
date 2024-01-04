#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <cmath>
#include "structures.h"
#include "table.h"

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
    
    unsigned char state[16];

    // Copy input message to the state array
    for (int i = 0; i < 16; i++) {
        state[i] = message[i];
    }

    // Print the initial state to the inspection file
    inspectionFile << "Each 16 divisions: ";
    printState(state, inspectionFile);

    // Number of encryption rounds
    int numberOfRounds = 9;

    for (int i = 0; i < numberOfRounds; i++) {

        inspectionFile << "---------------------------Round NO: " << i << " ---------------------------" << "\n";
        // Perform a single encryption round
        encryptionRound(state, expandedKey + (16 * i), Tbox_round, i, Ty_Table, xorTable, inspectionFile);

        inspectionFile << "State after round " << i << ": ";
        printState(state, inspectionFile);
    }

    inspectionFile << "---------------------------Final Round---------------------------" << "\n";

    // Perform the final encryption round
    finalRound(state, Tbox_final, inspectionFile);

    inspectionFile << "State after final round: ";
    printState(state, inspectionFile);

    // Copy the encrypted state to the output buffer
    for (int i = 0; i < 16; i++) {
        encryptedMessage[i] = state[i];
    }
}


int main() {
    ofstream inspectionFile("inspection.txt", ios::out);
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

    // AESEncrypt function with inspectionFile as an additional parameter
    for (int i = 0; i < paddedMessageLen; i += 16) {
        AESEncrypt(paddedMessage + i, expandedKey, encryptedMessage + i, inspectionFile);
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

