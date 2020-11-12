#include "systemc.h"
#include <stdio.h> 
#include <string.h>
#include <iostream> 
#include <fstream> 
#include <sstream>

#define NUM_BLOCKS 64
#define BLOCK_SIZE 64
#define INPUT1_ADDR 0
#define INPUT2_ADDR 16384
#define SAD_OUTPUT_ADDR 32768
#define MEM_SIZE 200000

class simple_mem_if: virtual public sc_interface 
{
  public:
    virtual bool Write(unsigned int addr, unsigned int data) = 0; // write data into address
    virtual bool Read(unsigned int addr, unsigned int& data) = 0; // read data into address
};


class Memory: public sc_module, public simple_mem_if
{
public:
	unsigned int MEM[MEM_SIZE];
	int i=0;
	//int MEM[MEM_SIZE];

	Memory (sc_module_name name, char* filename) : sc_module(name)
	{
		cout << "open file now......" << endl;
	    std::ifstream file(filename);
	   	std::string MEM_temp;

		if(file.is_open())
			{
				cout << "your file is opened successfully" <<endl;
				getline(file, MEM_temp);
				// cout << MEM_temp <<endl;
				std::stringstream file_temp(MEM_temp);
				while (getline(file_temp, MEM_temp,' '))
			    {
					MEM[i]= std::stoi(MEM_temp);
					i ++;
			    }
	   			file.close(); 	
			}
		else 
			{
				cout << "File Cannot find! usage: sad memfile! "<<endl;
				sc_stop();
			}
	}

	bool Write(unsigned int addr, unsigned int data)
	{

	    if (addr >= SAD_OUTPUT_ADDR)
	    {
		//cout << "Write" << data <<endl;
	    MEM[addr] = data;
	    return true;
	    }
	    cout << "Address now is " << addr << ", it's out of range! "
		     << "Address should be larger than" << SAD_OUTPUT_ADDR << endl;
	    return false;
	}

	bool Read(unsigned int addr, unsigned int& data)
	{

		if ((SAD_OUTPUT_ADDR >= addr) && (addr >= (INPUT1_ADDR)))
	    {
	    data = MEM[addr];
	    //cout << "Read" << data << endl;
	   	return true;
		}
		cout << "Address now is " << addr << ", it's out of range! "
		     << "Address should be larger than " << INPUT1_ADDR << " or be smaller than " << SAD_OUTPUT_ADDR << endl;
		return false;

	} 

};

class SAD: public sc_module
{
	public:
	int i, v; 
	unsigned int block; 
	unsigned int sad;
	unsigned int data1;
	unsigned int data2;
    sc_port<simple_mem_if> sig;
    SC_HAS_PROCESS(SAD);
    SAD(sc_module_name name) : sc_module(name)
    {
       SC_THREAD(SAD_cal);
    }

	void SAD_cal()
	{
		for (block=0; block<NUM_BLOCKS; block++)
		{
		    sad = 0;
		    for (i=0; i<BLOCK_SIZE; i++)
		    {
		    	sig->Read(INPUT1_ADDR+(block*BLOCK_SIZE)+i, data1);
		    	sig->Read(INPUT2_ADDR+(block*BLOCK_SIZE)+i, data2);	
		        v = data1 - data2;
		        // sc_start(1,SC_NS);
		        if( v < 0 ) v = -v;
		        sad += v;
		        // sc_start(1,SC_NS);
		    }
		   	sig->Write(SAD_OUTPUT_ADDR+block, sad); 
		   	cout << "@" << sc_time_stamp()  
		   		 << "sec, sad_value is" << sad<< endl;	
		}
	}

};

class top : public sc_module
{
public:
    Memory *memory_inst;
    SAD *sad_inst;
    top(sc_module_name name, char* filename) : sc_module(name)
    {
       memory_inst = new Memory("Memory", filename);
       sad_inst = new SAD("SAD");
       sad_inst-> sig(*memory_inst);
    }
};	 

//address
int sc_main(int argc, char* argv[])
{
	if (argc == 2)
	{
		top top1("Top1", argv[1]);
		sc_start();
	}
	else 
	{
		cout<< "number of command line arguments entered are incorrect! Please Check you enter 2 argument!" << endl;
	}
	return 0;
}

