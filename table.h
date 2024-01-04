/* "structures.h" defines the look-up tables and KeyExpansion function
 * used in encrypt.cpp and decrypt.cpp
 */
#ifndef TABLE_H
#define TABLE_H

#include "structures.h"
unsigned char** generateXorTable() {
    unsigned char** xorTable = new unsigned char*[256];
    for (int i = 0; i < 256; i++) {
        xorTable[i] = new unsigned char[256];
        for (int j = 0; j < 256; j++) {
            xorTable[i][j] = static_cast<unsigned char>(i ^ j);
        }
    }
    return xorTable;
}


unsigned char*** TboxRound(unsigned char* key) {

    const int TboxSize1 = 9;
    const int TboxSize2 = 16;
    const int TboxSize3 = 256;
    unsigned char*** Ttable = new unsigned char**[TboxSize1];

    for (int i = 0; i < TboxSize1; i++) {
        Ttable[i] = new unsigned char*[TboxSize2];
        ShiftKeys(key+(16*i));
        for (int j = 0; j < TboxSize2; j++) {

            Ttable[i][j] = new unsigned char[TboxSize3];

            std::bitset<8> temp(key[(16 * i) + j]);

            std::bitset<8> binaryNumber1(temp);

            for (int k = 0; k < TboxSize3; k++) {
                std::bitset<8> binaryNumber2(k);
                std::bitset<8> temp = binaryNumber1 ^ binaryNumber2;

                int tempAsInt = static_cast<int>(temp.to_ulong());

                Ttable[i][j][k] = static_cast<unsigned char>(s[tempAsInt]);


            }



        }

    }

    return Ttable;
}

unsigned char*** Tytable (){

    unsigned char*** Ty = new unsigned char**[4];
    for (int i = 0; i < 4; ++i) {
        Ty[i] = new unsigned char*[256];
        for (int j = 0; j < 256; ++j) {
            Ty[i][j] = new unsigned char[4];
        }
    }


    for (int i = 0; i < 256; i++) {


    Ty[0][i][0]= mul2[i];
    Ty[0][i][1]=static_cast<unsigned char>(i);
    Ty[0][i][2]=static_cast<unsigned char>(i);
    Ty[0][i][3]=mul3[i];

    Ty[1][i][0]=mul3[i];
    Ty[1][i][1]=mul2[i];
    Ty[1][i][2]=static_cast<unsigned char>(i);
    Ty[1][i][3]=static_cast<unsigned char>(i);

    Ty[2][i][0]=static_cast<unsigned char>(i);
    Ty[2][i][1]=mul3[i];
    Ty[2][i][2]=mul2[i];
    Ty[2][i][3]=static_cast<unsigned char>(i);

    Ty[3][i][0]=static_cast<unsigned char>(i);
    Ty[3][i][1]=static_cast<unsigned char>(i);
    Ty[3][i][2]=mul3[i];
    Ty[3][i][3]=mul2[i];


    }


return Ty ;

}

unsigned char*** TboxFinalGenerator(unsigned char* key) {

    const int TboxSize1 = 1;
    const int TboxSize2 = 16;
    const int TboxSize3 = 256;
    unsigned char*** Ttable = new unsigned char**[TboxSize1];

    for (int i = 0; i < TboxSize1; i++) {
        Ttable[i] = new unsigned char*[TboxSize2];
        ShiftKeys(key+(16*i));
        for (int j = 0; j < TboxSize2; j++) {

            Ttable[i][j] = new unsigned char[TboxSize3];

            std::bitset<8> temp(key[(16 * i) + j]);
            unsigned char temp1= (key[(16 * i) + j+16]);
            std::bitset<8> binaryNumber1(temp);


            for (int k = 0; k < TboxSize3; k++) {
                std::bitset<8> binaryNumber2(k);
                std::bitset<8> temp = binaryNumber1 ^ binaryNumber2;

                int tempAsInt = static_cast<unsigned char >(temp.to_ulong());

                Ttable[i][j][k] = static_cast<unsigned char>(s[tempAsInt]^temp1);

            }



        }

    }


    return Ttable;
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




#endif /* TABLE_H */
