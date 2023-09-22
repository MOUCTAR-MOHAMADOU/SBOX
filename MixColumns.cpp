#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>
#include <sstream>

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
void mix_columns(unsigned char* state){ 
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

	for(int i = 0; i<16; i++)
	{
		state[i] = tmp[i];
	}
}

int main()
{
    unsigned char state[] = {
        0x63, 0x7c, 0x77, 0x7b,
        0xf2, 0x6b, 0x6f, 0xc5,
        0x30, 0x01, 0x67, 0x2b,
        0xfe, 0xd7, 0xab, 0x76
    };

    mix_columns(state);

    // Afficher le résultat
    for (int i = 0; i < 16; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(state[i]) << " ";
        if ((i + 1) % 4 == 0) {
            std::cout << std::endl;
        }
    }

    return 0;
}
