#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include<ctime>
using namespace std;
const short int s[] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
const short int p[] = { 1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16 };
short int max_output_diff[16] = { 0 }; // 存储每个输入差分对应的最多次数的输出差分
int max_count[16] = { 0 }; // 存储每个输入差分对应的最多次数
short int diff_table[16][16];
short int s1[4] = { 0 };
short int s2[4] = { 0 };
short int s3[4] = { 0 };
int probility = 0, sum = 1;
short int inverse_s_box[16];
void generate_inverse_s_box() {
    for (int i = 0; i < 16; ++i) {
        inverse_s_box[s[i]] = i;
    }
}
struct FourTuple {
    string binaryString;
    string xorString;
    string binaryString_out;
    string xorString_out;
};
vector<FourTuple> datas;
// SPN加密函数
void Keyupdate(int& start, string key, string& K)
{
    for (int i = 0; i < 16; i++)
    {
        K[i] = key[start + i];
    }
    start += 4;
}
void Whitening(string& w, string K)
{
    bitset<16> w_bits(w);
    bitset<16> K_bits(K);
    w_bits ^= K_bits;
    w = w_bits.to_string();
}
void Substitution(string& u)
{
    for (int i = 0; i < 16; i++)
    {
        string binaryString = u.substr(i * 4, 4);
        bitset<4> binary(binaryString);
        int index = s[binary.to_ulong()];
        u.replace(i * 4, 4, bitset<4>(index).to_string());
    }
}
void Permutation(string& v)
{
    string v_temp = v;
    for (int i = 0; i < 16; i++)
    {
        v[i] = v_temp[p[i] - 1];
    }
}
void SPN(string& w, string key)
{
    int start = 0;
    string K;
    for (int i = 0; i < 16; i++)
    {
        K += '0'; // Initialize K with zeros
    }
    for (int i = 0; i < 3; i++)
    {
        Keyupdate(start, key, K);
        Whitening(w, K);
        Substitution(w);
        Permutation(w);
    }
    Keyupdate(start, key, K);
    Whitening(w, K);
    Substitution(w);
    Keyupdate(start, key, K);
    Whitening(w, K);
}

//针对4位的S盒
short int Substitution_single_revise(short int v)
{
    generate_inverse_s_box();
    return inverse_s_box[v];
}
//针对4位的逆S盒
void Substitution_single(string& u)
{
    string binaryString = u.substr(0, 4);
    bitset<4> binary(binaryString);
    int index = s[binary.to_ulong()];
    u.replace(0, 4, bitset<4>(index).to_string());
}
// 生成差分分布表
void generate_table() {
    // 遍历所有可能的输入差分和输出差分
    for (int input_diff = 0; input_diff < 16; input_diff++) {
        for (int i = 0; i < 16; i++) {
            // 统计特定输入和输出差分的出现次数
            // 遍历所有可能的输入值
            string input_binary = bitset<4>(i).to_string();
            string output_binary = input_binary;
            Substitution_single(output_binary); // 获得S盒的输出
            int input_x = stoi(input_binary, nullptr, 2);
            int output_x = stoi(output_binary, nullptr, 2);
            int xor_result_input = input_x ^ input_diff;
            string input_binary_xor = bitset<4>(xor_result_input).to_string();
            string output_binary_xor = input_binary_xor;
            Substitution_single(output_binary_xor);
            int input_xor = stoi(input_binary_xor, nullptr, 2);
            int output_xor = stoi(output_binary_xor, nullptr, 2);
            int xor_result_output = output_x ^ output_xor;
            // 将统计结果存储到表中
            if (++diff_table[input_diff][xor_result_output] > max_count[input_diff]) {
                max_count[input_diff] = diff_table[input_diff][xor_result_output];
                max_output_diff[input_diff] = xor_result_output;
            }
        }
    }
}
void diff_chain_check(string& input_diff)
{
    string result;
    int index = 0;
    for (int i = 0; i < 4; i++)
    {
        string binaryString = input_diff.substr(i * 4, 4);
        bitset<4> binary(binaryString);
        if (binary.to_ulong() != 0)
        {
            index = max_output_diff[binary.to_ulong()];
            s1[i] = max_count[binary.to_ulong()];
            if (probility != 0)
                probility *= s1[i];
            else
                probility += s1[i];
            sum *= 16;
        }
        input_diff.replace(i * 4, 4, bitset<4>(index).to_string());
        index = 0;
    }
    Permutation(input_diff);
    for (int i = 0; i < 4; i++)
    {
        string binaryString = input_diff.substr(i * 4, 4);
        bitset<4> binary(binaryString);
        if (binary.to_ulong() != 0)
        {
            index = max_output_diff[binary.to_ulong()];
            s2[i] = max_count[binary.to_ulong()];
            probility *= s2[i];
            sum *= 16;
        }
        input_diff.replace(i * 4, 4, bitset<4>(index).to_string());
        index = 0;
    }
    Permutation(input_diff);
    for (int i = 0; i < 4; i++)
    {
        string binaryString = input_diff.substr(i * 4, 4);
        bitset<4> binary(binaryString);
        if (binary.to_ulong() != 0)
        {
            index = max_output_diff[binary.to_ulong()];
            s3[i] = max_count[binary.to_ulong()];
            probility *= s3[i];
            sum *= 16;
        }
        input_diff.replace(i * 4, 4, bitset<4>(index).to_string());
        index = 0;
    }
    Permutation(input_diff);
    cout << "u4 = " << input_diff << endl;

}

//四元组构造
string intToBinaryString(int n) {
    bitset<16> bits(n);
    return bits.to_string();
}
void generate_txt(string targetxor, int number) {
    // 指定特定值
    bitset<16>binaryxor(targetxor);
    short int value_1, value_2, value_xor1, value_xor2;
    int targetXOR = binaryxor.to_ulong();
    //cout << "targetXOR="<<targetXOR << endl;
    int count = 0;
    string key = "00111010100101001101011000111111";
    // 设置随机数生成器
    mt19937 rng(time(nullptr)); // 使用当前时间作为随机数种子
    uniform_int_distribution<int> distribution(0, 0xFFFF); // 生成 0 到 0xFFFF 之间的均匀分布的随机数
    // 打开文件
    ofstream outputFile("fourtuple_set.txt");
    // 写入随机生成的16位二进制数及其异或结果到文件
    if (outputFile.is_open()) {
        while (count < number)
        {
            // 计算与特定值异或的结果
            int randomNum = distribution(rng);
            int xorResult = randomNum ^ targetXOR;
            // 将随机数转换为16位的二进制字符串
            string binaryString = intToBinaryString(randomNum);
            string binaryString_out = binaryString;
            SPN(binaryString_out, key);
            // 将异或结果转换为16位的二进制字符串
            string xorString = intToBinaryString(xorResult);
            string xorString_out = xorString;
            SPN(xorString_out, key);
            //针对差分0000101100000000
            value_1 = bitset<4>(binaryString_out.substr(0, 4)).to_ulong();
            value_2 = bitset<4>(binaryString_out.substr(8, 4)).to_ulong();
            value_xor1 = bitset<4>(xorString_out.substr(0, 4)).to_ulong();
            value_xor2 = bitset<4>(xorString_out.substr(8, 4)).to_ulong();
            //针对差分0000111100000000
            /*value_1 = bitset<4>(binaryString_out.substr(0, 4)).to_ulong();
            value_2 = bitset<4>(binaryString_out.substr(12, 4)).to_ulong();
            value_xor1 = bitset<4>(xorString_out.substr(0, 4)).to_ulong();
            value_xor2 = bitset<4>(xorString_out.substr(12, 4)).to_ulong();*/
            //针对差分0110000000000000
            /*value_1 = bitset<4>(binaryString_out.substr(4, 4)).to_ulong();
            value_2 = bitset<4>(binaryString_out.substr(12, 4)).to_ulong();
            value_xor1 = bitset<4>(xorString_out.substr(4, 4)).to_ulong();
            value_xor2 = bitset<4>(xorString_out.substr(12, 4)).to_ulong();
            if (value_1 == value_xor1 && value_2 == value_xor2)*/
            {
                // 写入到文件
                outputFile << binaryString << " " << xorString << " " << binaryString_out << " " << xorString_out << endl;
                count++;
            }
            randomNum++;
        }
        outputFile.close();
        cout << "Data has been written to fourtuple_set.txt." << endl;
    }
    else {
        cout << "Unable to open file for writing." << endl;
    }
}
void readDataFromFile(const string& filename) {
    ifstream inputFile(filename);

    if (inputFile.is_open()) {
        // 用于存储读取的四元组数据

        while (!inputFile.eof()) {
            FourTuple tuple;
            // 尝试读取四个数据项
            if (inputFile >> tuple.binaryString >> tuple.xorString >> tuple.binaryString_out >> tuple.xorString_out) {
                // 如果成功读取四个数据项，则将其存储到 vector 中
                datas.push_back(tuple);
            }
        }
        inputFile.close();
        // 处理存储的数据

        cout << "Number of elements in vector: " << datas.size() << endl;
    }
    else {
        cout << "Unable to open file for reading." << endl;
    }
}
void differential_attack()
{
    short int count[16][16] = { 0 };
    readDataFromFile("fourtuple_set.txt");
    short int y[4] = { 0 };
    short int y_xor[4] = { 0 };
    short int v[4] = { 0 };
    short int u[4] = { 0 };
    short int v_xor[4] = { 0 };
    short int u_xor[4] = { 0 };
    short int u_mid[4] = { 0 };
    short int start = 0;
    short int key1, key2;
    unsigned long int num = 0;
    for (const auto& tuple : datas)
    {
        for (int i = 0; i < tuple.binaryString_out.size(); i += 4) {
            // 获取当前四位子串并转换为整数
            int value = bitset<4>(tuple.binaryString_out.substr(i, 4)).to_ulong();
            int value_xor = bitset<4>(tuple.xorString_out.substr(i, 4)).to_ulong();
            y[start] = value;
            y_xor[start] = value_xor;
            start++;
        }
        start = 0;
        /*if (y[0] == y_xor[0] && y[2] == y_xor[2])
        {*/
        ++num;
        //针对0000101100000000
        for (int l1 = 0; l1 < 16; l1++)
        {
            for (int l2 = 0; l2 < 16; l2++)
            {
                v[1] = l1 ^ y[1];
                v[3] = l2 ^ y[3];
                u[1] = Substitution_single_revise(v[1]);
                u[3] = Substitution_single_revise(v[3]);
                v_xor[1] = l1 ^ y_xor[1];
                v_xor[3] = l2 ^ y_xor[3];
                u_xor[1] = Substitution_single_revise(v_xor[1]);
                u_xor[3] = Substitution_single_revise(v_xor[3]);
                u_mid[1] = u[1] ^ u_xor[1];
                u_mid[3] = u[3] ^ u_xor[3];
                if (u_mid[3] == 0x6 && u_mid[1] == 0x6)
                {
                    count[l1][l2] += 1;
                }

            }
        }
        //针对0000111100000000
        /*for (int l1 = 0; l1 < 16; l1++)
        {
            for (int l2 = 0; l2 < 16; l2++)
            {
                v[1] = l1 ^ y[1];
                v[2] = l2 ^ y[2];
                u[1] = Substitution_single_revise(v[1]);
                u[2] = Substitution_single_revise(v[2]);
                v_xor[1] = l1 ^ y_xor[1];
                v_xor[2] = l2 ^ y_xor[2];
                u_xor[1] = Substitution_single_revise(v_xor[1]);
                u_xor[2] = Substitution_single_revise(v_xor[2]);
                u_mid[1] = u[1] ^ u_xor[1];
                u_mid[2] = u[2] ^ u_xor[2];
                if (u_mid[1] == 6 && u_mid[2] == 6)
                    count[l1][l2] += 1;
            }
        }*/
        //针对0110000000000000
        /*for (int l1 = 0; l1 < 16; l1++)
        {
            for (int l2 = 0; l2 < 16; l2++)
            {
                v[0] = l1 ^ y[0];
                v[2] = l2 ^ y[2];
                u[0] = Substitution_single_revise(v[0]);
                u[2] = Substitution_single_revise(v[2]);
                v_xor[0] = l1 ^ y_xor[0];
                v_xor[2] = l2 ^ y_xor[2];
                u_xor[0] = Substitution_single_revise(v_xor[0]);
                u_xor[2] = Substitution_single_revise(v_xor[2]);
                u_mid[0] = u[0] ^ u_xor[0];
                u_mid[2] = u[2] ^ u_xor[2];
                if (u_mid[0] == 13 && u_mid[2] == 13)
                    count[l1][l2] += 1;
            }
        }*/

    }
    int max = -1;
    for (int l1 = 0; l1 < 16; l1++)
    {
        for (int l2 = 0; l2 < 16; l2++)
        {
            if (count[l1][l2] > max)
            {
                max = count[l1][l2];
                key1 = l1;
                key2 = l2;
            }
        }
    }
    cout << "max=" << max << ", key1= " << key1 << ", key2:" << key2 << "  使用的有效明密文对: " << num << endl;
}
int main() {
    generate_table();
    // 输出差分分析表
    cout << "差分分析表: " << endl;
    for (int input_diff = 0; input_diff < 16; input_diff++) {
        cout << "输入差分: " << input_diff << ", 最大输出差分: " << max_output_diff[input_diff] << ", 统计次数: " << max_count[input_diff] << endl;
    }
    string x_diff;
    cin >> x_diff;
    generate_txt(x_diff, 100);
    diff_chain_check(x_diff);
    cout << " 差分统计总数: " << probility << " , 整体随机总数 : " << sum << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << "S1[" << i << "]:" << s1[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << "S2[" << i << "]:" << s2[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << "S3[" << i << "]:" << s3[i] << " ";
    }
    cout << endl;
    differential_attack();
    return 0;
}
//求解低16位密钥
//0000101100000000
//0000111100000000
//0110000000000000

//SPN测试
//0010011010110111
//00111010100101001101011000111111

//目前代码需要输入0000101100000000来进行子密钥的求解
//若想转成另外两个输入差分
//请在differential_attack()和generate_txt()两个函数中进行注释的转换
//其中后者可以调整产生的四元组个数
//不保证每次运行产生的子密钥都是对的，如果想进行数据集的重新生成或者保持数据集的不变动，可在主函数中相应部分进行注释切换