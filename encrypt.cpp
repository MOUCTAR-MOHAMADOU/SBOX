#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <bitset>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef std::vector<std::vector<unsigned char>> Matrix;
typedef std::vector<unsigned char> Vector;
using Matrix = std::vector<std::vector<unsigned char>>;

const Matrix A = {
    {1, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 1}    
};
const Vector c = {1,1,0,0,0,1,1,0};
// Définition de la constante m(x)
const std::bitset<9> mx = 0b100011011;

unsigned char rcon[256] = {
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
	0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
	0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
	0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
	0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
	0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
	0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
	0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
	0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
	0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
	0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
	0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
	0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
	0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
	0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
	0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};
// Fonction pour convertir un hexadécimal en polynôme dans F_2^8[x]
bitset<32> hex_to_poly(string hex)
{
    bitset<32> poly;
    for (char c : hex)
    {
        poly <<= 4;
        if (c >= '0' && c <= '9')
        {
            poly |= c - '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            poly |= c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f')
        {
            poly |= c - 'a' + 10;
        }
        else
        {
            cerr << "Erreur : caractère hexadécimal invalide" << endl;
            exit(1);
        }
    }
    return poly;
}
// Fonction pour calculer le produit de deux polynômes dans F_2^8[x] modulo m(x)
bitset<32> poly_mul(bitset<32> p, bitset<32> q)
{
    bitset<32> r = 0;
    for (int i = 0; i < 32; i++)
    {
        if (q[i] == 1)
        {
            r ^= (p << i);
        }
    }
    for (int i = 31; i >= 8; i--)
    {
        if (r[i] == 1)
        {
            r ^= (mx.to_ulong() << (i - 8));
        }
    }
    return r;
}
// Fonction pour calculer la puissance d'un polynôme dans F_2^8[x] modulo m(x)
bitset<32> poly_pow(bitset<32> p, int n)
{
    bitset<32> r = 1;
    while (n > 0)
    {
        if (n % 2 == 1)
        {
            r = poly_mul(r, p);
        }
        p = poly_mul(p, p);
        n /= 2;
    }
    return r;
}
Vector MatrixVectorProduct(const Matrix& A, const std::vector<bool>& b) {
    Vector result(A.size(), 0);
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[i].size(); j++) {
            result[i] ^= A[i][j] * b[j];
        }
    }
    for (int i = result.size() - 1; i >= 0; i--) {
        result[i] ^= c[i];
    }

    return result;
}
std::vector<bool> PolynomialToBooleanVector(const std::bitset<32>& polynomial) {
    std::vector<bool> vec;
    for (int i = 0; i <= 7; i++) {
        bool bit = polynomial[i];
        vec.push_back(bit);
    }
    return vec;
}
std::string PrintVector(const Vector& vec) {
    // Lire les 4 derniers bits dans l'ordre croissant 
    int value2 = 8 * vec[7] + 4 * vec[6] + 2 * vec[5] + vec[4];   
    // Lire les 4 premiers bits dans l'ordre décroissant
    int value1 = 8 * vec[3] + 4 * vec[2] + 2 * vec[1] + vec[0];
    // Convertir les valeurs hexadécimales en une seule chaîne de caractères
    std::stringstream ss;
    ss << std::hex << std::setw(2) << value2 << value1;
    return ss.str();
}
std::string ToHex(unsigned char value) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value);
    return ss.str();
}
unsigned char invToHex(const std::string& hexValue) {
    std::istringstream iss(hexValue);
    unsigned int value = 0;
    iss >> std::hex >> value; 
    return static_cast<unsigned char>(value);
}
unsigned char CalculateMatrixVectorProduct(unsigned char hex) {
//std::string CalculateMatrixVectorProduct(unsigned char hex) {
	std::string hexss = ToHex(hex);
    // Convertir hex en bitset<32> poly
    bitset<32> poly = hex_to_poly(hexss);
    // Calculer l'inverse de poly
    bitset<32> inverse = poly_pow(poly, 254);
    // Convertir inverse en vecteur booléen
    std::vector<bool> vec = PolynomialToBooleanVector(inverse);
    // Calculer le produit matrice-vecteur
    Vector result = MatrixVectorProduct(A, vec);
    // Convertir le résultat en hexadécimal
    std::string hexValue = PrintVector(result);
	
	unsigned char uc = invToHex(hexValue);
    return uc;
}
unsigned char calculateResult(std::string hexp, unsigned char hexq)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hexq);
    std::string hex = ss.str();
    std::bitset<32> p = hex_to_poly(hexp);
    std::bitset<32> q = hex_to_poly(hex);
    std::bitset<32> result = poly_mul(q, p);
    std::bitset<8> maskedResult = result.to_ulong() & 0xFF;
    return maskedResult.to_ulong();
}
// Auxiliary function for KeyExpansion
void KeyExpansionCore(unsigned char * in, unsigned char i) {
	// Rotate left by one byte: shift left 
	unsigned char t = in[0];
	in[0] = in[1];
	in[1] = in[2];
	in[2] = in[3];
	in[3] = t;

	in[0] = CalculateMatrixVectorProduct(in[0]);
	in[1] = CalculateMatrixVectorProduct(in[1]);
	in[2] = CalculateMatrixVectorProduct(in[2]);
	in[3] = CalculateMatrixVectorProduct(in[3]);

	// RCon
	in[0] ^= rcon[i];
}

void KeyExpansion(unsigned char inputKey[16], unsigned char expandedKeys[176]) {
	// The first 128 bits are the original key
	for (int i = 0; i < 16; i++) {
		expandedKeys[i] = inputKey[i];
	}
	int bytesGenerated = 16; // Bytes we've generated so far
	int rconIteration = 1; // Keeps track of rcon value
	unsigned char tmpCore[4]; // Temp storage for core
	while (bytesGenerated < 176) {
		for (int i = 0; i < 4; i++) {
			tmpCore[i] = expandedKeys[i + bytesGenerated - 4];
		}
		if (bytesGenerated % 16 == 0) {
			KeyExpansionCore(tmpCore, rconIteration++);
		}
		for (unsigned char a = 0; a < 4; a++) {
			expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - 16] ^ tmpCore[a];
			bytesGenerated++;
		}
	}
}
using namespace std;
void AddRoundKey(unsigned char * state, unsigned char * roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundKey[i];
	}
}
void SubBytes(unsigned char * state) {
	for (int i = 0; i < 16; i++) {
		state[i] = CalculateMatrixVectorProduct(state[i]);
	}
}

// Shift left, adds diffusion
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
void MixColumns(unsigned char * state) {
	unsigned char tmp[16];

	tmp[0] = (unsigned char)(calculateResult("02", state[0])^calculateResult("03", state[1])^state[2]^state[3]);
	tmp[1] = (unsigned char)(state[0]^calculateResult("02", state[1])^calculateResult("03", state[2])^state[3]);
	tmp[2] = (unsigned char)(state[0]^state[1]^calculateResult("02", state[2])^calculateResult("03", state[3]));
	tmp[3] = (unsigned char)(calculateResult("03", state[0])^state[1]^state[2]^calculateResult("02", state[3]));

	tmp[4] = (unsigned char)(calculateResult("02", state[4])^calculateResult("03", state[5])^state[6]^state[7]);
	tmp[5] = (unsigned char)(state[4]^calculateResult("02", state[5])^calculateResult("03", state[6])^state[7]);
	tmp[6] = (unsigned char)(state[4]^state[5]^calculateResult("02", state[6])^calculateResult("03", state[7]));
	tmp[7] = (unsigned char)(calculateResult("03", state[4])^state[5]^state[6]^calculateResult("02", state[7]));

	tmp[8] = (unsigned char)(calculateResult("02", state[8])^calculateResult("03", state[9])^state[10]^state[11]);
	tmp[9] = (unsigned char)(state[8]^calculateResult("02", state[9])^calculateResult("03", state[10])^state[11]);
	tmp[10] = (unsigned char)(state[8]^state[9]^calculateResult("02", state[10])^calculateResult("03", state[11]));
	tmp[11] = (unsigned char)(calculateResult("03", state[8])^state[9]^state[10]^calculateResult("02", state[11]));

	tmp[12] = (unsigned char)(calculateResult("02", state[12])^calculateResult("03", state[13])^state[14]^state[15]);
	tmp[13] = (unsigned char)(state[12]^calculateResult("02", state[13])^calculateResult("03", state[14])^state[15]);
	tmp[14] = (unsigned char)(state[12]^state[13]^calculateResult("02", state[14])^calculateResult("03", state[15]));
	tmp[15] = (unsigned char)(calculateResult("03", state[12])^state[13]^state[14]^calculateResult("02", state[15])); 

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}
void Round(unsigned char * state, unsigned char * key) {
	SubBytes(state);
	ShiftRows(state);
	MixColumns(state);
	AddRoundKey(state, key);
}
 // Same as Round() except it doesn't mix columns
void FinalRound(unsigned char * state, unsigned char * key) {
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, key);
}

void AESEncrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage) {
	unsigned char state[16]; // Stores the first 16 bytes of original message
	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}
	int numberOfRounds = 9;
	AddRoundKey(state, expandedKey); // Initial round
	for (int i = 0; i < numberOfRounds; i++) {
		Round(state, expandedKey + (16 * (i+1)));
	}
	FinalRound(state, expandedKey + 160);
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
	// Pad message to 16 bytes
	int originalLen = strlen((const char *)message);
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
	unsigned char * encryptedMessage = new unsigned char[paddedMessageLen];
    std::string str;
    std::cout << "Enter the key: ";
    std::getline(std::cin, str);
    std::istringstream hex_chars_stream(str);	
	unsigned char key[16];
	int i = 0;
	unsigned int c;
	while (hex_chars_stream >> hex >> c)
	{
		key[i] = c;
		i++;
	}
	unsigned char expandedKey[176];
	KeyExpansion(key, expandedKey);
	for (int i = 0; i < paddedMessageLen; i += 16) {
		AESEncrypt(paddedMessage+i, expandedKey, encryptedMessage+i);
	}
	cout << "Encrypted message in hex:" << endl;
	for (int i = 0; i < paddedMessageLen; i++) {
		cout << hex << (int) encryptedMessage[i];
		cout << " ";
	}
	cout << endl;
	// Free memory
	delete[] paddedMessage;
	delete[] encryptedMessage;

	return 0;
}
