#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include <string>
#include <bitset>
#include <algorithm>
#include <iomanip>
typedef unsigned char byte;

using namespace std;

string key = "2b7E151628AED2A6ABF7158809CF4F3C";

vector<vector<unsigned char>> rcon =
{
    {0x01,0x00,0x00,0x00},  //RCON[1]
    {0x02,0x00,0x00,0x00},  //RCON[2]
    {0x04,0x00,0x00,0x00},  //RCON[3]
    {0x08,0x00,0x00,0x00},  //RCON[4]
    {0x10,0x00,0x00,0x00},  //RCON[5]
    {0x20,0x00,0x00,0x00},  //RCON[6]
    {0x40,0x00,0x00,0x00},  //RCON[7]
    {0x80,0x00,0x00,0x00},  //RCON[8]
    {0x1B,0x00,0x00,0x00},  //RCON[9]
    {0x36,0x00,0x00,0x00}   //RCON[10]
};
////////////////////////////////////////////////////////////////////////////////////////
using namespace std;

typedef std::vector<std::vector<unsigned char>> Matrix;
typedef std::vector<unsigned char> Vector;

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
//const std::vector<std::string>& hexVector)
std::vector<std::string> CalculateMatrixVectorProduct(const std::string& hex) {
    std::vector<std::string> resultVector;

    bitset<32> poly = hex_to_poly(hex);
    bitset<32> inverse = poly_pow(poly, 254);

    std::vector<bool> vec = PolynomialToBooleanVector(inverse);
    Vector result = MatrixVectorProduct(A, vec);

    std::string hexValue = PrintVector(result);
    resultVector.push_back(hexValue);

    return resultVector;
}
////////////////////////////////////////////////////////////////////////////////////////
vector<vector<string>> ConvertKeyInto4Groups(string key) {
    vector<vector<string>> dword;
    vector<string> fourBytes;
    int k = 0;
    for (int i=0; i<key.length(); i+=8) {
        //i = 0, 8, 16, 24, 32
        string sub = key.substr(i,8);
        fourBytes.push_back(sub);
        k++;
    }
    for (int i = 0; i<fourBytes.size(); i++) {
        vector<string> eachFour;
        for (int k=0; k<fourBytes[i].length(); k+=2) {
            string eachByte = fourBytes[i].substr(k,2);
            eachFour.push_back(eachByte);
        }
        dword.push_back(eachFour);
    }
    return dword;
}
/*
int ConvertHexLetterToNumber(char hex) {
    int result = 0;
    if (hex == 'A') {
        result = 10;
    }else if (hex == 'B'){
        result = 11;
    }else if (hex == 'C'){
        result = 12;
    }else if (hex == 'D'){
        result = 13;
    }else if (hex == 'E'){
        result = 14;
    }else if (hex == 'F'){
        result = 15;
    }
    return result;
}
*/
vector<string> DoRotWordOnLast(vector<string> lastfour) {
    auto last4bytes = lastfour;  //
    auto firstByte = last4bytes[0];
    for (int k=0; k < last4bytes.size() - 1; k++) {
        last4bytes[k] = last4bytes[k+1];
    }
    last4bytes[3] = firstByte;
    return last4bytes;
}
vector<int> StringToInt(vector<string> bytes) {
    vector<int> nums;
    for (int i = 0; i < bytes.size(); i++) {
        string ch0 = bytes[i];
        int num = stoi(ch0, 0, 16);
        nums.push_back(num);
    }
    return nums;
}
vector<int> firstXOR(vector<unsigned char> subyte, vector<unsigned char> rcon, vector<int> firstWord) {
    vector<int> result;
    for (int i =0; i < subyte.size(); i++) {
        int r = subyte[i] ^ rcon[i] ^ firstWord[i];
        result.push_back(r);
    }
    return result;
}
vector<int> generateKey(vector<vector<string>> previousKey, vector<int> lastKeyFirst) {
    vector<int> key = lastKeyFirst;
    vector<int> prev = lastKeyFirst;
    for (int i=1; i<4; i++) {
        vector<int> newKey;
        for (int k=0; k<lastKeyFirst.size(); k++) {
            auto pk = previousKey[i];
            auto convertPk = StringToInt(pk);
            auto xorPL = convertPk[k] ^ prev[k];
            newKey.push_back(xorPL);
            prev.push_back(xorPL);
            key.push_back(xorPL);
        }
        prev = newKey;
    }
    return key;
}
template <class T>
string convertIntToHex(T t, ios_base & (*f)(ios_base&))
{
    ostringstream oss;
    oss << f << t;
    return oss.str();
}

string convertIntToHexString(vector<int> keyInInt) {
    string keystring;
    for (int i = 0; i < keyInInt.size(); i++) {
        string byte = convertIntToHex<int>(keyInInt[i], hex);
        if (byte.length() == 1) {
            byte = "0" + byte;
        }
        keystring += byte;
    }
    
    return keystring;
}

int main(int argc, const char * argv[]) {
        
    vector<string> allkeys;
    
    for (int i = 0; i < 10; i++) {
        
        auto keyvector = ConvertKeyInto4Groups(key);
        
        auto last4bytes = DoRotWordOnLast(keyvector[3]);
        
        std::vector<unsigned char> subyte;

        for (const auto& hexPair : last4bytes) {
            std::vector<std::string> result = CalculateMatrixVectorProduct(hexPair);
            for (const auto& hexValue : result) {
                unsigned char byte = static_cast<unsigned char>(std::stoi(hexValue, nullptr, 16));
                subyte.push_back(byte);
            }
        }
        //auto subyte = CalculateMatrixVectorProduct(last4bytes);
        //2. RCON
        auto getRcon = rcon[i]; //TODO: this 0 will be replaced with the number of rounds i.e. i
        //3. Apply this to each group of 4 bytes, starting from the first one
        auto firstFour = keyvector[0];
        //4. XOR subByte, GetRCON, FirstFour
        auto firstFourInts = StringToInt(firstFour);
    
        //xor the last word: subbyte, rcon, firstfour
        auto result = firstXOR(subyte, getRcon, firstFourInts);
        
        //generate a new key
        auto nextKey = generateKey(keyvector, result);
        
        //convert int key to string key
        auto newKeyString = convertIntToHexString(nextKey);
        
        //save key
        allkeys.push_back(newKeyString);
        
        //new key becomes last key
        key = newKeyString;
    }
    
    for (int i=0; i< allkeys.size(); i++) {
        cout << i << ": " << allkeys[i] << endl;
    }

    return 0;
}

/*
    2B7E151628AED2A6ABF7158809CF4F3C
 0: a0fafe1788542cb123a339392a6c7605
 1: f2c295f27a96b9435935807a7359f67f
 2: 3d80477d4716fe3e1e237e446d7a883b
 3: ef44a541a8525b7fb671253bdb0bad00
 4: d4d1c6f87c839d87caf2b8bc11f915bc
 5: 6d88a37a110b3efddbf98641ca0093fd
 6: 4e54f70e5f5fc9f384a64fb24ea6dc4f
 7: ead27321b58dbad2312bf5607f8d292f
 8: ac7766f319fadc2128d12941575c006e
 9: d014f9a8c9ee2589e13f0cc8b6630ca6
  
 */
