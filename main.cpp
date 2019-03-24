#include <iostream>
#include <string>

using namespace std;

unsigned long long int pow(unsigned long long int base, int one)
{
    unsigned long long int result = 1ULL;
    while (one)
    {
        if (one & 1)
        {
            result *= (unsigned long long int)base;
        }
        one >>= 1;
        base *= base;
    }
    return result;
}


string ConvertFromSymbolic(string str)
{
    string str1;
    string str2;
    string str3;

    bool more_digits = false;

    for (int i = 0; i < str.length(); i++)
    {

        if (isdigit(str[i]) && !more_digits)
        {
            str1 += str[i];
        }
        else
            if (i != 0)
            {
                if (str[i] == '.' && !more_digits)
                {
                    str1 += str[i];
                }
                else
                {
                    str2 += str[i];
                    more_digits = true;
                }

            }
    }

    double int1 = stod(str1);

    if (str2 == "M"){
        str3 = to_string((unsigned long long int)(int1 * pow(10, 6)));
        return str3;
    }
    else if (str2 == "B"){
        str3 = to_string((unsigned long long int)(int1 * pow(10, 9)));
        return str3;
    }
    else if (str2 == "T"){
        str3 = to_string((unsigned long long int)(int1 * pow(10, 12)));
        return str3;
    }
    else if (str2 == "Qa"){
        str3 = to_string((unsigned long long int)(int1 * pow(10, 12))) + to_string(pow(10, 3)).erase(0, 1);
        return str3;
    }
    else if (str2 == "Qi"){
        str3 = to_string((unsigned long long int)(int1 * pow(10, 12))) + to_string(pow(10, 6)).erase(0, 1);
        return str3;
    }
    else if (str2 == "Sx"){
        str3 = to_string((unsigned long long int)(int1 * pow(10, 12))) + to_string(pow(10, 9)).erase(0, 1);
        return str3;
    }
    else if (str2 == "Sp"){
        str3 = to_string((unsigned long long int)(int1 * pow(10, 12))) + to_string(pow(10, 12)).erase(0, 1);
        return str3;
    }
    else if (str2 == "Oc"){
        str3 = to_string((unsigned long long int)(int1 * pow(10, 12))) + to_string(pow(10, 16)).erase(0, 1);
        return str3;
    }
    else{
        return 0;
    }
}
int main(){

    cout << ConvertFromSymbolic("4M") << endl;
    cout << ConvertFromSymbolic("54T") << endl;
    cout << ConvertFromSymbolic("5.434B") << endl;

    return 1;
}