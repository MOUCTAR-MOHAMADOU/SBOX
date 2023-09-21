#include <iostream>
#include <bitset>

std::bitset<32> hex_to_poly(std::string hex)
{
    std::bitset<32> poly;
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
            std::cerr << "Erreur : caractère hexadécimal invalide" << std::endl;
            exit(1);
        }
    }
    return poly;
}

std::bitset<32> poly_mul(std::bitset<32> p, std::bitset<32> q)
{
    std::bitset<32> r = 0;
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
            r ^= (0x11B << (i - 8));
        }
    }
    return r;
}

std::bitset<8> calculateResult(std::string hexp, std::string hexq)
{
    std::bitset<32> p = hex_to_poly(hexp);
    std::bitset<32> q = hex_to_poly(hexq);
    std::bitset<32> result = poly_mul(q, p);
    std::bitset<8> maskedResult = result.to_ulong() & 0xFF; // Appliquer un masque pour conserver les 8 bits de poids faible
    return maskedResult;
}

int main()
{
    std::string hex;
    //std::string hexq = "02";
    std::cout << "Entrez le chiffre hexadécimal : ";
    std::cin >> hex;
    std::bitset<8> result = calculateResult(hex, "03");
    std::cout << "Résultat : " << result << std::endl;
    return 0;
}
