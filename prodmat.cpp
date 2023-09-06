#include <iostream>
#include <vector>
#include <bitset>

typedef std::vector<std::vector<unsigned char>> Matrix;
typedef std::vector<unsigned char> Vector;

const Matrix A = {
    {0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 0, 0, 0, 1, 1, 1, 1}
};

const Vector c = {1,1,0,0,0,1,1,0};

Vector MatrixVectorProduct(const Matrix& A, const Vector& b) {
    Vector result(A.size(), 0);

    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[i].size(); j++) {
            result[i] ^= A[i][j] * b[j];
        }
    }

    return result;
}

Vector BinaryStringToVector(const std::string& binaryString) {
    Vector result;
    if (binaryString.size() != 8) {
        std::cerr << "Le vecteur b doit être un binaire de 8 bits." << std::endl;
        return result;
    }

    for (char c : binaryString) {
        if (c != '0' && c != '1') {
            std::cerr << "Le vecteur b doit être un binaire de 8 bits." << std::endl;
            return result;
        }
        result.push_back(c - '0');
    }

    return result;
}

void PrintVector(const Vector& vec) {
    for (int i = vec.size() - 1; i >= 0; i--) {
//    for (int i = 0; i < vec.size(); i++) {
        std::cout << static_cast<int>(vec[i]);
    }
    std::cout << std::endl;
}


int main() {
    std::string binaryString;
    std::cout << "Entrez le vecteur b en binaire de 8 bits : ";
    std::cin >> binaryString;

    Vector b = BinaryStringToVector(binaryString);

    if (b.empty()) {
        return 1;
    }

    if (b.size() != A[0].size()) {
        std::cerr << "Le vecteur b n'a pas la même taille que la matrice A." << std::endl;
        return 1;
    }

    Vector result = MatrixVectorProduct(A, b);

    std::cout << "Résultat du produit matriciel : ";
    PrintVector(result);

    if (result.size() != c.size()) {
        std::cerr << "Le vecteur c n'a pas la même taille que le résultat du produit matriciel." << std::endl;
        return 1;
    }

    for (int i = 0; i < result.size(); i++) {
    //for (int i = result.size() - 1; i >= 0; i--) {
        result[i] ^= c[i];
    } 

    std::cout << "Vecteur c : ";
    PrintVector(c);

    std::cout << "Résultat final après XOR avec le vecteur c : ";
    PrintVector(result);

    return 0;
}