# Whitebox Cryptography Implementation

White-box cryptography involves securing cryptographic algorithms and keys even when the internal details are fully exposed to potential attackers. This implementation focuses on the importance of white-box cryptography in untrusted environments, resistance to reverse engineering, and application security.

## Implemented Modes of AES 128-bits
- ECB
- CTR

## C++ Source Files Overview

### 1. `encrypt.cpp`

`encrypt.cpp` is the standard implementation of the AES-128 encryption algorithm. It serves as the baseline for AES functionality, providing a fundamental understanding of the encryption process.

### 2. `encryptTablebased.cpp`

`encryptTablebased.cpp` improves upon the standard AES-128 encryption by utilizing lookup tables (TBox, TyiTable, XorTable) from the `table.h` header file. This optimization enhances performance for a more sophisticated AES implementation.

### 3. `encryptEncoding.cpp`

`encryptEncoding.cpp` incorporates an additional encoding function with the help of the `encoding.h` header file. It introduces bijection functions for internal and external encoding, enhancing security and flexibility in the encryption process.

### 4. `encryptCTR.cpp`

`encryptCTR.cpp` focuses on the Counter (CTR) mode of operation. It implements a table-based approach using functionalities from `table.h` for secure and efficient encryption.

### 5. `encryptConfusingRound.cpp`

`encryptConfusingRound.cpp` enhances the AES implementation with complex rounds, considering security measures. It uses functionalities from header files like `encoding.h`, `table.h`, and possibly other header files for comprehensive encryption.

## Header Files

In addition to the mentioned source files, the implementation may depend on various header files:

- `table.h`: Contains lookup tables (TBox, TyiTable, XorTable) for optimized encryption.
- `encoding.h`: Provides additional encoding functions for enhanced security.
- Other relevant header files that contribute to the functionality and security of the encryption process.

## Compilation and Output

To compile the C++ code in Google Colab, a bash script is utilized for streamlined execution. The compilation process ensures that the code is translated into machine-readable instructions. After successful compilation, the encrypted message is overwritten in a file named "output.txt."

## Inspection Files

Each C++ file generates an inspection file (e.g., `inspection_encrypt.txt`) providing step-by-step details and intermediate states during the encryption process. Examining these inspection files provides insights into the internal workings of each component.

## Execution Script

A bash script is employed to compile and execute the C++ code. The script ensures that the necessary dependencies are configured, and the compilation process is initiated. Below is a simplified example of the script:

```bash
#!/bin/bash

g++ -o encryptExecutable encrypt.cpp
./encryptExecutable > output.txt
