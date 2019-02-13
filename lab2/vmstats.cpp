#include "vmsim.cpp"

int main(int argc, char** argv){

    char* opt = (char*)"opt";
    char* lru = (char*)"lru";
    char* fifo = (char*)"fifo";
    //cout << opt << endl << lru << endl << fifo << endl;
    //cout << argv[1] << endl;
    int min_num = isdigit2(argv[1]);
    //cout << min_num << endl;
    //cout << argv[2] << endl;
    int max_num = isdigit2(argv[2]);
    //cout << max_num << endl;
    //cout << argv[3] << endl;
    int leap = isdigit2(argv[3]);
    //cout << leap << endl;
    const char* filename = argv[4];
    //cout << filename << endl;;

    if( max_num == -1 || leap == -1 || min_num < 2 || max_num > 100){
        cout << "Enter proper values in the field. Now exiting.." << endl;
        return 0;
    }
    unsigned int i = 0;
    unsigned int j = 0;
    verbose__ = 0;
    for(i = min_num; i < max_num; i += leap){
        cout << "Total pageframes: " << i << endl;
        main2(i, filename, opt);
        main2(i, filename, fifo);
        main2(i, filename, lru);
        //cout << "misses_opt: " << misses_opt << endl;
        //cout << "misses_lru: " << misses_fifo << endl;
        //cout << "misses_lru: " << misses_lru << endl;
    }

    return 0;
}