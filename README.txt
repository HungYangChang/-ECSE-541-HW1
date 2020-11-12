Student: Hung-Yang Chang
McGill ID: 260899468 

To run the simulation, please run the following command:
g++-8 -o sad file_name.cpp -lsystemc -L$SYSTEMC_HOME/lib -I$SYSTEMC_HOME/include
Here file_name.cpp should be Memory.cpp for part 1 and Memory_RTL.cpp for part 2.

Then run the following command:
./sad mem_init.txt
Here mem_init.txt is the file to read the value out to do SAD computation.
Note: Type invalid command will remind you to type the invalid one!

Much Thanks to Charles Le, Jack Hu, Ilshat Sagitov, Danilo Vucetic, and other classmates!
