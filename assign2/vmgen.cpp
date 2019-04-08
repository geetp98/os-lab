#include <iostream>
#include <cstdlib>

using namespace std;

int isdigit2(const char *s) {
    /* Convert a character array into integer,
       but only after verification that
       input contains integers only.
       Otherwise return -1. */
    unsigned int i = 0;
    unsigned int n = strlen(s);
    int sum = 0;
    for (;i < n; i++) {
        if( s[i] >='0' && s[i] <= '9') {
            sum = sum * 10 + (s[i] - '0');
        }
        else {
            return -1;
        }
    }
    return sum;
}

int main(){
    cout << "Enter the parameters in following sequence:" << endl;
    cout << " 1.Range of the frames (positive interger)" << endl;
    cout << " 2.Length of the file (positive interger)" << endl;
    cout << " 3.Filename for the file to be generated" << endl;
    cout << "For example, enter the following." << endl;
    cout << "20 100 vmrefs.dat" << endl;
    cout << ">";

    int range;
    int length;
    char filename[24];

    cin >> range >> length >> filename;

    if(range <= 2 || length <= 2){
        cout << ".." << endl;
        cout << "Enter proper sequences!!.. now exiting" << endl;
        cout << ".." << endl;
        return 0;
    }

    freopen(filename,"w",stdout);
    unsigned int i = 0;
    int a = 0;
    int prev;
    for( i = 0; i < length; i += 1 ){
        a = rand()%range;
        if(i == 0){
            cout << a << " ";
        }
        else if(a != prev){
            cout << a << " ";
        }
        else{
            i -= 1;
        }
        prev = a;
    }
    cout << "\b\b";

    return 0;
}