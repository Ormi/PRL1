/*
PRL Project 2
@author Michal Ormos
@email xormos00@stud.fit.vutbr.cz
@date March 2019
*/

#include <iostream>
#include <mpi.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <iterator>
using namespace std; 

#define DEBUG 1
/**
	Function for just displaying content of vector
	@param vector of int numbers
	@return display content to stdout
*/
void display_vector(const vector<int> &v)
{
    copy(v.begin(), v.end(),
        ostream_iterator<int>(cout, " "));
}

int main(int argc, char *argv[]) {
    int processesCount;
    int sumOfNumbers;
    int processId;
    int bucketSize;
    int leavesProcesses;


    string fileName = "numbers";
	const int EOS=-1;

	// Initialize the MPI
    MPI_Init(&argc, &argv);
    // Get number of process
    MPI_Comm_size(MPI_COMM_WORLD,&processesCount); //get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD,&processId); //get actual process id
    MPI_Status status;
    int MSG_SIZE;

    //master process who control all others
    if(processId == 0) {
    	cout << "I am the process 0!\n";

    	int num;
    	int c;
    	vector<int> numbers;
        ifstream inputFile;
        inputFile.open(fileName.c_str()); 
        // Read until no characters is left
        while(!inputFile.eof()) {
        	num = inputFile.get();
       		if(num != -1) {
       			cout << num << " ";
       			numbers.push_back(num);
       			sumOfNumbers++;
       		}
        }

        bucketSize = round(sumOfNumbers/((processesCount+1)/2));  
        leavesProcesses = (processesCount+1)/2;

        // Control prints
        if (DEBUG) {
	        cout << "\nNumber of numbers: " << sumOfNumbers << "\n"; 
	        cout << "Number of processes: " << processesCount << "\n";	
	        cout << "Bucket size: " << bucketSize << "\n";
	        cout << "Log2 number: " << leavesProcesses << "\n";
	        display_vector(numbers);
	    }
        //

       
        int proc = 1;
        for (int j = 0; j < sumOfNumbers; j++) {
        	MPI_Send(&numbers[j], 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        	if (proc == leavesProcesses) {
        		proc = 1;
        	} else {
        		proc++;
        	}
        }
        c=EOS;
        for (int z = 1; z <= leavesProcesses; z++) {
	        MPI_Send(&c, 1, MPI_INT, z, 0, MPI_COMM_WORLD);	 
        }

    } else {
    	//SLAVES
    	vector<int> numbersBucketSlave;
    	bool next = true;
        int recievedData;
        while(next) {
		    MPI_Recv(&recievedData, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    if(DEBUG) {
		    	printf("Process %d received number %d from process 0\n", processId, recievedData);   
		    }
		    if(recievedData==EOS) {
		    	next = false;
		    } 
		    numbersBucketSlave.push_back(recievedData);
		}	
		// Control prints
		if(DEBUG) {
			cout << "Displayed vector:\n";
			display_vector(numbersBucketSlave);
			cout << "I am done\n";
		}
		//

		sort(numbersBucketSlave.begin(), numbersBucketSlave.end());

		// Control prints
		if(DEBUG) {
			cout << "Displayed vector sorted:\n";
			display_vector(numbersBucketSlave);
			cout << "I am done sorted\n";
		}		
		//    
    }


    // releive all alocated processes
    MPI_Finalize();
    // finish programm
    return 0;    

}