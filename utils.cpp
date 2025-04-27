#include <string>
using namespace std;

int hex_to_int(string hex_num)
{
    string bin_num = "";
    int i = 0;
    while(hex_num[i])
    {
        switch (hex_num[i])
        {
            case '0':
                bin_num += "0000";
                i++;
                break;

            case '1':
                bin_num += "0001";
                i++;
                break;
            
            case '2':
                bin_num += "0010";
                i++;
                break;

            case '3':
                bin_num += "0011";
                i++;
                break;

            case '4':
                bin_num += "0100";
                i++;
                break;

            case '5':
                bin_num += "0101";
                i++;
                break;

            case '6':
                bin_num += "0110";
                i++;
                break;

            case '7':
                bin_num += "0111";
                i++;
                break;

            case '8':
                bin_num += "1000";
                i++;
                break;

            case '9':
                bin_num += "1001";
                i++;
                break;

            case 'A':
            case 'a':
                bin_num += "1010";
                i++;
                break;

            case 'B':
            case 'b':
                bin_num += "1011";
                i++;
                break;

            case 'C':
            case 'c':
                bin_num += "1100";
                i++;
                break;

            case 'D':
            case 'd':
                bin_num += "1101";
                i++;
                break;

            case 'E':
            case 'e':
                bin_num += "1110";
                i++;
                break;

            case 'F':
            case 'f':
                bin_num += "1111";
                i++;
                break;
            
            default:
                break;
        }
    }
    if (bin_num.length() < 32) {
        int diff = 32 - bin_num.length();
        for (int j = 0; j < diff; j++) {
            bin_num = "0" + bin_num;
        }
    }
    return bin_num;
}
