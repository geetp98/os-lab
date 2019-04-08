Operating Systems: Programming Assignment 2

This submission contains 3 c++ files. Each of them has different purposes.

vmgen.cpp:
This file generates an array of random numbers of given length between given range and saves it to a file. To utilize this file, there are instructions in the file itself.
To compile and run it, follow these commands.
$ g++ vmgen.cpp -o vmgen
$ ./vmgen

vmsim.cpp:
This file is a simulator for three page replacement algorithms. One is Optimal Page Replacement, second is First In First Out and the third is Least Recently Used. To run the simulator, you must provide number of physical frames available, filename (in which a sequence of numbers i.e. page access order is stored), name of the algorithm (opt, fifo or lru) and verbose.
To compile and run it, follow these commands.
$ g++ vmsim.cpp -o vmsim
$ ./vmsim <positive integer> <filename> <lru|opt|fifo> <0|1>
    For example:
    $ ./vmsim 3 vmrefs.dat fifo 1

vmgen.cpp:
This file has a slightly modified version of `vmsim.cpp`, and added onto that is a layer that uses takes inputs such as number of minimum and maximum page frames, leap size and filename that is supposed to be read. This file will call use the simulator multiple times and will generate output data, which will also be stored in a file named `vmrefs.dat`. To utilize this file properly, instructions are in the file itself.
To compile and run it, follow these commands.
$ g++ vmstats.cpp -o vmstats
$ ./vmstats

This assignment is also hosted on https://github.com/geetp98/os-lab/tree/master/lab2