#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;

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

// Fonction pour afficher un polynôme dans F_2^8[x]
void print_poly(bitset<32> poly)
{
    bool non_zero = false;
    for (int i = 31; i >= 0; i--)
    {
        if (poly[i] == 1)
        {
            if (non_zero)
            {
                cout << " + ";
            }
            non_zero = true;
            if (i == 0)
            {
                cout << "1";
            }
            else if (i == 1)
            {
                cout << "x";
            }
            else
            {
                cout << "x^" << i;
            }
        }
    }
    if (!non_zero)
    {
        cout << "0";
    }
    cout << endl;
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

// Fonction pour calculer l'inverse d'un polynôme dans F_2^8[x]modulo m(x)
bitset<32> poly_inv(bitset<32> p)
{
    bitset<32> r = poly_pow(p, 254); // 254 est l'ordre de m(x) dans F_2^8[x]
    return r;
}

int main()
{
    cout << "Entrez un polynôme hexadécimal : ";
    string hex;
    cin >> hex;

    // Conversion de l'hexadécimal en polynôme
    bitset<32> poly = hex_to_poly(hex);

    cout << "Polynôme en F_2^8[x] : ";
    print_poly(poly);

    cout << "Inverse du polynôme : ";
    bitset<32> inverse = poly_inv(poly);
    print_poly(inverse);

    return 0;
}