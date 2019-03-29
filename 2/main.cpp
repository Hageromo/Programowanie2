#include <iostream>
#include <string>


using namespace std;

class Convert {

private:
    string str;

public:

    Convert(string str) {
        this->str = str;
    }

    string ConvertFromSymbolic() {
        string w;
        char string1;
        char string2;
        int dot = 0;
        int zeros;
        int zeros_final;
        for (int x = 0; x < str.length(); x++) {
            string1 = str[x];
            string2 = str[x + 1];
            if (string1 == '.') { dot = x; }
            else if (string1 == 'M') { zeros = 6; break; }
            else if (string1 == 'B') { zeros = 9; break; }
            else if (string1 == 'T') { zeros = 12; break; }
            else if (string1 == 'Q' && string2 == 'a') { zeros = 15; break; }
            else if (string1 == 'Q' && string2 == 'i') { zeros = 18; break; }
            else if (string1 == 'S' && string2 == 'x') { zeros = 21; break; }
            else if (string1 == 'S' && string2 == 'p') { zeros = 24; break; }
            else if (string1 == 'O' && string2 == 'c') { zeros = 27; break; }
            else w = w + string1;
        }
        if (dot != 0) {
            zeros_final = str.length() - dot - 2;
            zeros = zeros - zeros_final;
        }
        for (int x = 0; x < zeros; x++) {
            w = w + "0";
        }

        return w;
    }

};

int main(){

    Convert n1("4M") ;
    cout<< n1.ConvertFromSymbolic() <<endl;

    Convert n2("54T") ;
    cout<< n2.ConvertFromSymbolic() <<endl;

    Convert n3("5.434B") ;
    cout<< n3.ConvertFromSymbolic() <<endl;


    return 0;
}
