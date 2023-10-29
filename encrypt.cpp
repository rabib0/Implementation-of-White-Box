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

using namespace std;








void printState (unsigned char * state){
    for (int i = 0; i < 16; i++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(state[i]) << " ";
        }
        std::cout << std::endl;
        cout << "\n" ;
}



void ShiftRows(unsigned char * state) {
	unsigned char tmp[16];

	/* Column 1 */
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];

	/* Column 2 */
	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	/* Column 3 */
	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];

	/* Column 4 */
	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}


unsigned TBoxestyiTables(unsigned char * state , unsigned char *** Ttable, int tracker,unsigned char *** Ty,unsigned char ** xorTable){

    unsigned char temp[16] ;

    for (int i = 0; i < 16; i++) {

                state[i]= Ttable[tracker][i][state[i]] ;
    }

    for (int i = 0; i < 16; i+=4){

        temp[i]= xorTable[xorTable[xorTable[Ty[0][state[i]][0]][Ty[1][state[i+1]][0]]][Ty[2][state[i+2]][0]]][Ty[3][state[i+3]][0]];
        temp[i+1]= xorTable[xorTable[xorTable[Ty[0][state[i]][1]][Ty[1][state[i+1]][1]]][Ty[2][state[i+2]][1]]][Ty[3][state[i+3]][1]];
        temp[i+2] = xorTable[xorTable[xorTable[Ty[0][state[i]][2]][Ty[1][state[i+1]][2]]][Ty[2][state[i+2]][2]]][Ty[3][state[i+3]][2]];
        temp[i+3] = xorTable[xorTable[xorTable[Ty[0][state[i]][3]][Ty[1][state[i+1]][3]]][Ty[2][state[i+2]][3]]][Ty[3][state[i+3]][3]];

    }

    for (int i = 0; i < 16; i++) {
		state[i] = temp[i];
	}

}


void TboxFinal(unsigned char * state , unsigned char *** TboxTable){

    unsigned char temp[16] ;
    for (int i = 0; i < 16; i++) {

        state[i]= TboxTable[0][i][state[i]] ;

    }

}



void Round(unsigned char * state, unsigned char * key , unsigned char *** Ttable, int tracker,unsigned char *** Ty,unsigned char ** xorTable) {

    cout << "key for this round: ";
    printState(key);


    ShiftRows(state);
	cout<< "After Shiftrows: ";
	printState(state);

    cout << "After looking up TBoxesTyiTables: " ;
    TBoxestyiTables(state,Ttable,tracker,Ty,xorTable);
    printState(state);

}


void FinalRound (unsigned char * state , unsigned char*** TboxTable){

    ShiftRows(state);
	cout<< "After Shiftrows: ";
	printState(state);

    cout << "After looking up Tbox Table: " ;
    TboxFinal(state,TboxTable) ;
}


// Bijective function using Galois field
unsigned char bijectiveFunction(unsigned char x) {
    return x ^ 0x04;
}



/* The AES encryption function
 * Organizes the confusion and diffusion steps into one function
 */
void AESEncrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage) {

    unsigned char *** Tbox_round=TboxRound(expandedKey);
    unsigned char ** xorTable = generateXorTable();
    unsigned char *** Ty_Table=Tytable();
    unsigned char *** Tbox_final = TboxFinalGenerator(expandedKey + (16 * 9));
	unsigned char state[16];
	unsigned char temp[16];
    unsigned char xtemp[16];

	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}

    cout <<"Each 16 divisions : " ;
	printState(state);

	int numberOfRounds = 9;



	for (int i = 0; i < numberOfRounds; i++) {



        cout << "----------------------Round NO: " << i << " ----------------------" << "\n" ;
        if (i>0){
            for (int i = 0; i < 16; i++) {

                state[i]= bijectiveFunction(state[i]) ;
            }
            cout << "Inversing : " ;
            printState(state);
        }

        cout << "Starting state of this round: ";
        printState(state);
		Round(state, expandedKey + (16 * i),Tbox_round,i,Ty_Table,xorTable);

        cout<< "Before bijecting: ";
        printState(state);
        for (int i = 0; i < 16; i++) {

            state[i]= bijectiveFunction(state[i]) ;
        }

        cout<< "After bijecting: ";
        printState(state);




	}

	cout << "----------------------Final Round----------------------"<< "\n" ;


    for (int i = 0; i < 16; i++) {
        state[i]= bijectiveFunction(state[i]) ;
    }
	cout<< "Inversing function: ";
	printState(state);

    FinalRound(state,Tbox_final);










	cout << "State after final round : "  ;
    printState(state);

	// Copy encrypted state to buffer
	for (int i = 0; i < 16; i++) {
		encryptedMessage[i] = state[i];
	}
}





int main() {

	cout << "=============================" << endl;
	cout << " 128-bit AES Encryption Tool   " << endl;
	cout << "=============================" << endl;

	char message[1024];

	cout << "Enter the message to encrypt: ";
	cin.getline(message, sizeof(message));
	cout << message << endl;
    int originalLen = strlen((const char *)message);
    cout << "Firstly message is converted into hex : ";
    for (int j = 0; j < originalLen; j++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(message[j]) << " ";
    }
    std::cout << std::endl;
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
    cout << "The message is converted into hex of 16 chunks: ";
    for (int j = 0; j < paddedMessageLen; j++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(paddedMessage[j]) << " ";
    }
    std::cout << std::endl;

    cout << "\n" ;
	unsigned char * encryptedMessage = new unsigned char[paddedMessageLen];

	string str;
	ifstream infile;
	infile.open("keyfile", ios::in | ios::binary);

	if (infile.is_open())
	{
		getline(infile, str); // The first line of file should be the key
		infile.close();
	}

	else cout << "Unable to open file";

	istringstream hex_chars_stream(str);
	unsigned char key[16];
	int i = 0;
	unsigned int c;
	while (hex_chars_stream >> hex >> c)
	{
		key[i] = c;
		i++;
	}

	unsigned char expandedKey[176];


    cout << "Keys read from Key file" << "\n" ;
    for (int j = 0; j < 16; j++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(key[j]) << " ";
    }
    std::cout << std::endl;

    cout << "\n" ;

	KeyExpansion(key, expandedKey);

	cout << "EXPANDED KEY" << "\n" ;
    for (int i = 0; i < 176; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(expandedKey[i]) << " ";
    }
    std::cout << std::endl;

    cout << "\n" ;


	for (int i = 0; i < paddedMessageLen; i += 16) {
		AESEncrypt(paddedMessage+i, expandedKey, encryptedMessage+i);
	}

	cout << "Encrypted message in hex:" << endl;
	for (int i = 0; i < paddedMessageLen; i++) {
		cout << hex << (int) encryptedMessage[i];
		cout << " ";
	}

	cout << endl;

	// Write the encrypted string out to file "message.aes"
	ofstream outfile;
	outfile.open("message.aes", ios::out | ios::binary);
	if (outfile.is_open())
	{
		outfile << encryptedMessage;
		outfile.close();
		cout << "Wrote encrypted message to file message.aes" << endl;
	}

	else cout << "Unable to open file";

	// Free memory
	delete[] paddedMessage;
	delete[] encryptedMessage;

	return 0;
}
