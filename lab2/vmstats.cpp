#include "vmsim2.cpp"

int main(){
    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    cout << ">";

    char* opt = (char*)"opt";
    char* lru = (char*)"lru";
    char* fifo = (char*)"fifo";
    int min_num;
    int max_num;
    int leap;
    char filename[24];

    cin >> filename >> min_num >> max_num >> leap;

    if( max_num == -1 || leap == -1 || min_num < 2 || max_num > 100){
        cout << "Enter proper values in the field. Now exiting.." << endl;
        return 0;
    }
    unsigned int i = 0;
    unsigned int j = 0;
    for(i = min_num; i <= max_num; i += leap){
        main2(i, filename, opt);
        cout << "opt, " << i << " frames, Miss rate = " << misses_opt << "/" << total_access << " = " << (double)(misses_opt*100)/total_access << (char)37 << endl;
    }
    cout << "------------------------------------------" << endl;
    for(i = min_num; i <= max_num; i += leap){
        main2(i, filename, fifo);
        cout << "fifo, " << i << " frames, Miss rate = " << misses_fifo << "/" << total_access << " = " << (double)(misses_fifo*100)/total_access << (char)37 << endl;
    }
    cout << "-------------------------------------------" << endl;
    for(i = min_num; i <= max_num; i += leap){
        main2(i, filename, lru);
        cout << "lru, " << i << " frames, Miss rate = " << misses_lru << "/" << total_access << " = " << (double)(misses_lru*100)/total_access << (char)37 << endl;
    }

    freopen("vmrates.dat","w",stdout);
    for(i = min_num; i <= max_num; i += leap){
        cout << i << " ";
    }
    cout << "\b" << endl;
    for(i = min_num; i <= max_num; i += leap){
        main2(i, filename, opt);
        cout << (double)(misses_opt*100)/total_access << " ";
    }
    cout << "\b" << endl;
    for(i = min_num; i <= max_num; i += leap){
        main2(i, filename, fifo);
        cout << (double)(misses_fifo*100)/total_access << " ";
    }
    cout << "\b" << endl;
    for(i = min_num; i <= max_num; i += leap){
        main2(i, filename, lru);
        cout << (double)(misses_lru*100)/total_access << " ";
    }
    cout << "\b";

    return 0;
}