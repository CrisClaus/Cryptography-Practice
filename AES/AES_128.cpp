#include<iostream>
#include<string>
#include <iomanip>
#include<bitset>
#include<sstream>
using namespace std;
short int decimal_1, decimal_2;
const unsigned char sbox[16][16] = 
{
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};
unsigned char hexcharToByte(char hex1, char hex2)
{
    if (hex1 >= 'a' && hex1 <= 'f')
    {
        decimal_1 = hex1 - 'a';
        decimal_1 += 10;
    }
    else
    {
        decimal_1 = hex1 - '0';
    }
    if (hex2 >= 'a' && hex2 <= 'f')
    {
        decimal_2 = hex2 - 'a';
        decimal_2 += 10;
    }
    else
    {
        decimal_2 = hex2 - '0';
    }
    return static_cast<unsigned char>((decimal_1 << 4) | decimal_2);
}
void SubBytes(unsigned char ch[]) {
    for (int i = 0; i < 16; i++)
    {
        // 获取字符的高四位和低四位
        ch[i] = sbox[(ch[i] & 0xF0)>>4][(ch[i] & 0x0F)];
    }
    
}
void AddRoundKey(unsigned char state[], unsigned char roundkey[])
{
    for (int i = 0; i < 16; i++)
        state[i] ^= roundkey[i];
}
void ShiftRows(unsigned char state[])
{
    /*for (int i = 0; i < 16; i++)
    {
        cout << static_cast<int>(state[i]) << " ";
        if (i != 0 && i % 4 == 3)
            cout << endl;
    }*/
    unsigned char temp_s[4];
    for (int i = 1; i < 4; ++i) {
        // 将当前行的数据复制到临时数组
        for (int j = 0; j < 4; ++j) {
            temp_s[j] = state[i * 4 + j];
        }
        // 循环左移当前行的数据
        for (int j = 0; j < 4; ++j) {
            state[i * 4 + j] = temp_s[(j + i) % 4];
        }
    }
    /*for (int i = 0; i < 16; i++)
    {
        cout << static_cast<int>(state[i]) << " ";
        if (i != 0 && i % 4 == 3)
            cout << endl;
    }*/
}
void SubWord(unsigned char& word) {
    word = sbox[(word & 0xF0) >> 4][word & 0x0F];
}

void RotWord(unsigned char word[]) {
    unsigned char temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}
void KeyExpansion(unsigned char initialKey[], int round, short int& check_s, unsigned char& check) {
    for (int i = 0; i < 4; ++i) {
        unsigned char temp[4];
        for (int j = 0; j < 4; ++j) {
            temp[j] = initialKey[i + (j << 2)];
        }
        if (i == 0) {
            unsigned char temp_init[4];
            for (int j = 0; j < 4; ++j) {
                temp_init[j] = initialKey[3 + (j << 2)];
            }
            RotWord(temp_init);
            for (int j = 0; j < 4; ++j) {
                SubWord(temp_init[j]);
            }
            if (round > 1)
            {
                check_s <<= 1;
                check <<= 1;
            }
            if (check_s > 255)
            {
                check ^= 0x1b;
                check_s ^= 0x001b;
                check_s &= 0x00ff;
                //cout << "ok" << endl;
            }
            //cout << hex << setw(2) << setfill('0') << static_cast<int>(check) << " " << check_s << endl;
            temp_init[0] ^= check;
            for (int j = 0; j < 4; ++j) {
                temp[j] = temp_init[j];
            }
            for (int j = 0; j < 4; ++j) {
                initialKey[i + (j << 2)] = initialKey[i + (j << 2)] ^ temp[j];
            }
        }
        else
        {
            for (int j = 0; j < 4; ++j) {
                initialKey[i + (j << 2)] = initialKey[i + (j << 2)] ^ initialKey[i - 1 + (j << 2)];
            }
        }
    }
}
bool isHighestBitSet(unsigned char ch) {
    return (ch & 0x80) != 0;
}
unsigned char Multiply(unsigned char x, unsigned char y) {
    unsigned char result = 0;
    bool highestBitSet = isHighestBitSet(y);
    while (x) {
        if (x & 1)
        {
            result ^= y;
        }
        y <<= 1;
        if (highestBitSet == true)
        {
            // 模运算，0x1b是AES中的特定多项式
            y ^= 0x1b;
        }
        x >>= 1;
    }
    return result;
}

void MixColumns(unsigned char state[])
{
    unsigned char temp[4];
    for (int c = 0; c < 4; ++c) {
        // 提取状态矩阵中的一列
        for (int r = 0; r < 4; ++r) {
            temp[r] = state[r * 4 + c];
        }
        // 执行混淆操作
        state[0 + c] = Multiply(0x02, temp[0]) ^ Multiply(0x03, temp[1]) ^ temp[2] ^ temp[3];
        state[4 + c] = temp[0] ^ Multiply(0x02, temp[1]) ^ Multiply(0x03, temp[2]) ^ temp[3];
        state[8 + c] = temp[0] ^ temp[1] ^ Multiply(0x02, temp[2]) ^ Multiply(0x03, temp[3]);
        state[12 + c] = Multiply(0x03, temp[0]) ^ temp[1] ^ temp[2] ^ Multiply(0x02, temp[3]);
    }
}
void AESencrypt(unsigned char state[], unsigned char roundkeys[])
{
    int Nr = 10;
    short int check_s = 0x0001;
    unsigned char check = 0x01;
    AddRoundKey(state, roundkeys);
    /*for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; j++) {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
        }
    }
    cout << endl;*/
    for (int round = 1; round < Nr; ++round) {
        SubBytes(state);
        /*cout << round << endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; j++) {
                cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
            }
        }
        cout << endl;*/
        ShiftRows(state);
        /*for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; j++) {
                cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
            }
        }
        cout << endl;*/
        MixColumns(state);
        /*for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; j++) {
                cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
            }
        }
        cout << endl;*/
        KeyExpansion(roundkeys, round, check_s, check);
        /*for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; j++) {
                cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
            }
        }
        cout << endl;*/
        AddRoundKey(state,roundkeys);
        /*for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; j++) {
                cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
            }
        }
        cout << endl;*/
    }
    //cout << "10" << endl;
    SubBytes(state);
    /*for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; j++) {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
        }
    }
    cout << endl;*/
    ShiftRows(state);
    /*for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; j++) {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
        }
    }
    cout << endl;*/
    KeyExpansion(roundkeys, 10, check_s, check);
    /*for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; j++) {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
        }
    }
    cout << endl;*/
    AddRoundKey(state, roundkeys);
    /*for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; j++) {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(state[i + (j << 2)]);
        }
    }
    cout << endl;*/
}
string charToString(unsigned char ch[]) {
    stringstream ss;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; j++) {
            ss << hex << setw(2) << setfill('0') << static_cast<int>(ch[i + (j << 2)]);
        }
    }
    string hexString = ss.str();
    // 将小写字母转换为大写字母
    for (char& ch : hexString) {
        ch = toupper(ch);
    }
    return hexString;
}
int main() 
{
    unsigned char state_temp[16], roundkey_temp[16];
    unsigned char state[16] = { 0x00 }, roundkey[16] = {0x00};
    unsigned char plaintext[32], key[32];
    //输入密钥
    for (int i = 0; i < 32; i++)
        cin >> key[i];
    for (int i = 0; i < 32; i += 2)
        roundkey_temp[i / 2] = hexcharToByte(key[i], key[i+1]);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            roundkey[i + (j << 2)] = roundkey_temp[(i << 2) + j];
    }
    //输入明文并初始化State
    for (int i = 0; i < 32; i++)
        cin >> plaintext[i];
    for (int i = 0; i < 32; i+=2)
        state_temp[i/2]= hexcharToByte(plaintext[i], plaintext[i + 1]);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            state[i + (j << 2)] = state_temp[(i << 2) + j];
    }
    //生成轮密钥
    AESencrypt(state, roundkey);
    /*for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4;j++) {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(toupper(state[i+(j<<2)]));
        }
    }
    cout << endl;*/
    string Ciphertext = charToString(state);
    cout << Ciphertext;
    return 0;
}
//000102030405060708090a0b0c0d0e0f
//00112233445566778899aabbccddeeff 
//mixcolumns：d4e0b81ebfb441275d52119830aef1e5
//keyex:2b28ab097eaef7cf15d2154f16a6883c
// 2b7e151628aed2a6abf7158809cf4f3c
//3243f6a8885a308d313198a2e0370734