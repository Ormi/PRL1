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
    	int recievedData;
    	int sendCounter = 0;
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
       
        int cpuCount = processesCount-leavesProcesses;
        int proc = cpuCount;
        for (int j = 0; j < sumOfNumbers; j++) {
        	MPI_Send(&numbers[j], 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        	sendCounter++;
        	if (proc == processesCount-1) {
        		proc = cpuCount;
        	} else {
        		proc++;
        	}
        }

        cout << "\nProcess " << processId << " sent " << sendCounter << " numbers\n======================================\n";
        c=EOS;
        for (int z = cpuCount; z <= processesCount; z++) {
	        MPI_Send(&c, 1, MPI_INT, z-1, 0, MPI_COMM_WORLD);	 
        }
    	vector<int> numbersBucketSlave;        
        bool next = true;
    	int  cCount = 0;	
        int recievedData2; 
        bool leftCount = true;
    	bool rightCount = true;
		while(next) {
			if (leftCount) {
		    	MPI_Recv(&recievedData2, 1, MPI_INT, (processId*2)+2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    }
		    if (rightCount) {
		    	MPI_Recv(&recievedData, 1, MPI_INT, (processId*2)+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    }
		    if(DEBUG) {
		    	printf("Process %d received number %d %d\n", processId, recievedData, recievedData2);   
		    }		    
		    if(recievedData != EOS) {		    
		    	numbersBucketSlave.push_back(recievedData);
		    } else {
		    	if(cCount == 1) {
		    		next = false;
		    		cCount = 0;
		    		leftCount = true;
		    	} else {
		    		cCount++;
		    		leftCount = false;
		    	}
		    }	
		    if(recievedData2 != EOS) {		    
		    	numbersBucketSlave.push_back(recievedData2);		    	
		    } else {
		    	if(cCount == 1) {
		    		next = false;
		    		cCount = 0;
		    		leftCount = true;
		    	} else {
		    		cCount++;
		    		leftCount = false;
		    	}
		    }			
		}
	    sort(numbersBucketSlave.begin(), numbersBucketSlave.end());
        cout << "========FNISHED============\n";
        display_vector(numbersBucketSlave);
    } else {
    	//SLAVES
    	vector<int> numbersBucketSlave;
    	bool next = true;
    	int c = EOS;    
    	int  cCount = 0;	
    	bool leftCount = true;
    	bool rightCount = true;
        int recievedData;
        int recievedData2;        
        bool amILeavesProc;

        if(processId >= ((processesCount/2))) {
        	amILeavesProc = true;
        } else {
        	amILeavesProc = false;
        }

        //Leaves
        while(next && amILeavesProc) {
		    MPI_Recv(&recievedData, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    // if(DEBUG) {
		    // 	printf("Process %d received number %d from process 0\n", processId, recievedData);   
		    // }
		    if(recievedData != EOS) {
		    	numbersBucketSlave.push_back(recievedData);
		    } else {
		    	next = false;
		    }
		}	

		//NotLeaves
		while(next && !amILeavesProc) {
			if (leftCount) {
		    	MPI_Recv(&recievedData2, 1, MPI_INT, (processId*2)+2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    }
		    if (rightCount) {
		    	MPI_Recv(&recievedData, 1, MPI_INT, (processId*2)+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    }
		    if(DEBUG) {
		    	printf("Process %d received number %d %d\n", processId, recievedData, recievedData2);   
		    }		    
		    if(recievedData != EOS) {		    
		    	numbersBucketSlave.push_back(recievedData);
		    } else {
		    	if(cCount == 1) {
		    		next = false;
		    		cCount = 0;
		    		leftCount = true;
		    	} else {
		    		cCount++;
		    		leftCount = false;
		    	}
		    }	
		    if(recievedData2 != EOS) {		    
		    	numbersBucketSlave.push_back(recievedData2);		    	
		    } else {
		    	if(cCount == 1) {
		    		next = false;
		    		cCount = 0;
		    		leftCount = true;
		    	} else {
		    		cCount++;
		    		leftCount = false;
		    	}
		    }			    		
		}

	    sort(numbersBucketSlave.begin(), numbersBucketSlave.end());

		if(DEBUG) {
			cout << "I am process " << processId << " and i have Displayed vector:\n";
			display_vector(numbersBucketSlave);
			cout << "\n";
		}
	    // cout << "Process " << processId << " sending data\n";
		if(DEBUG) {
			cout << "I am process " << processId << " and i have numbers: " << numbersBucketSlave.size() << "\n";
		}	    
		for (int i = 0; i < numbersBucketSlave.size(); ++i)
		{
			if(processId % 2 == 0) {
				cout << processId << " MPI SEND " << numbersBucketSlave[i] << " " << (processId/2)-1 << "\n";
	       		MPI_Send(&numbersBucketSlave[i], 1, MPI_INT, (processId/2)-1, 0, MPI_COMM_WORLD);
	       	}
	       	if(processId % 2 == 1) {
				cout << processId << " MPI SEND " << numbersBucketSlave[i] << " " << (processId/2) << "\n";
				MPI_Send(&numbersBucketSlave[i], 1, MPI_INT, (processId/2), 0, MPI_COMM_WORLD);
	       	}
		}

		if(processId % 2 == 0) {
			cout << processId << " MPI SEND " << c << " " << (processId/2)-1 << "\n";
       		MPI_Send(&c, 1, MPI_INT, (processId/2)-1, 0, MPI_COMM_WORLD);
       	}
       	if(processId % 2 == 1) {
			cout << processId << " MPI SEND " << c << " " << (processId/2) << "\n";
			MPI_Send(&c, 1, MPI_INT, (processId/2), 0, MPI_COMM_WORLD);
       	}
 
    }

    // releive all alocated processes
    MPI_Finalize();
    // finish programm
    return 0;    

}