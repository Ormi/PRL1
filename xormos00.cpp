/*
PRL Project 1
@author Michal Ormos
@email xormos00@stud.fit.vutbr.cz
@date Match 2019
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

void display_vector(const vector<int> &v)
{
    copy(v.begin(), v.end(),
        ostream_iterator<int>(cout, " "));
}

int main(int argc, char *argv[]) {
	// cout << "=======================================";
    int processesCount;
    int numbersCount;
    int processId;
    int lastProcess;

    int MSG_SIZE;
	const int EOS=-1;
	// int arr_size; // = sizeof(arr)/sizeof(int);
	// double items_of_array; // = log2(arr_size);
	// int list_cpu;// ; = nextPowerOfTwo(items_of_array);
	// int number_of_cpu;// ; = (2*list_cpu)-1;
	// int bucket_size;// ; = arr_size/list_cpu;
	// cout << "\tArray size (arr_size) is: " << arr_size << "\n\tNumber of lists (list_cpu): " << list_cpu << "\n\tNumber of CPUs (number_of_cpu): " << number_of_cpu << "\n\tBucker size(bucket_size): " << bucket_size << "\n";
	// Initialize the MPI
    MPI_Init(&argc, &argv);
    // Get number of process
    MPI_Comm_size(MPI_COMM_WORLD,&processesCount); //get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD,&processId); //get actual process id
    MPI_Status status;
    int bucketSize;
    // cout << "\t processesCount: " << processesCount;
    // cout << "\n\t processId: " << processId;
    int number;
    //master process who control all others
    if(processId == 0) {
    	cout << "I am the process 0!\n";
  //   	char character;
    	// int number;
		// ifstream fin;
		// fin.open("numbers", ios::in);
   //  	while(!fin.eof()) {	
	  // //   	fin.open(input, ios::in);
			// // number= fin.get();
			// // if( iFile.eof() ) break;
			// fin.get(character);
			// // number = (int)character;
	  //       cout << +character;    	
	  //   }
    	//reading numbers from file
    	int num, numberOfItems=0;
    	int c;
    	string fileName = "numbers";
    	vector<int> numbers;
        ifstream inputFile;
        inputFile.open(fileName.c_str()); 
        // Read until no characters is left
        while(!inputFile.eof()) {
        	num = inputFile.get();
       		if(num != -1) {
       			cout << num << " ";
       			numbers.push_back(num);
       			numberOfItems++;
       		}
        }  
        // print count of numbers to stdout
        cout << "\nNumber of numbers: " << numberOfItems << "\n"; 
        cout << "Number of processes: " << processesCount << "\n";	
        // get bucket number
        bucketSize = round(numberOfItems/((processesCount+1)/2));
        cout << "Bucket size: " << bucketSize << "\n";
        cout << "Log2 number: " << (processesCount+1)/2 << "\n";
        display_vector(numbers);
        // MSG_SIZE = 
        number = -1;
        // send data to cpu

// for j = 1 to log m do
	// for all processors at level (log m)-j do in parallel
		// procesor spojí posloupnosti svých syn
	// endfor
// endfor

// posli 4 cisla za sebou do prveho
// tak dalsie 4 cisla v poradi do druheho
// atd.

        // for(int j = 1; j <= (processesCount+1)/2; j++) { // from 1 to 3
	       //  	for(int z = 0; z <= j*bucketSize; z++) {
	       //  	MPI_Send(&numbers[j], 1, MPI_INT, j+1, 0, MPI_COMM_WORLD);

	       //      // MPI_Send(&number, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
	       //      // MPI_Send(&number, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
		      //   c=EOS;
		      //   MPI_Send(&c, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);	            
        // 	}
        // }        
        int proc = 1;
        for (int j = 0; j < numberOfItems; j++) {
        	MPI_Send(&numbers[j], 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        	if (proc == 4) {
        		proc = 1;
        	} else {
        		proc++;
        	}
        }
        c=EOS;
        for (int z = 1; z <=4; z++) {
	        MPI_Send(&c, 1, MPI_INT, z, 0, MPI_COMM_WORLD);	 
        }

    } else {
    	//SLAVES
    	vector<int> numbersBucketSlave;
    	bool next = true;
        int recievedData;
        while(next) {
		    MPI_Recv(&recievedData, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    printf("Process %d received number %d from process 0\n", processId, recievedData);   
		    if(recievedData==EOS) {
		    	next = false;
		    } 
		    numbersBucketSlave.push_back(recievedData);
		}	
		cout << "Displayed vector:\n";
		display_vector(numbersBucketSlave);
		cout << "I am done\n";
		sort(numbersBucketSlave.begin(), numbersBucketSlave.end());
		cout << "Displayed vector sorted:\n";
		display_vector(numbersBucketSlave);
		cout << "I am done sorted\n";		
        // int recievedData;
        // MPI_Recv(&numbers, MSG_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        // cout << "Data:" << numbers;

    }


    // releive all alocated processes
    MPI_Finalize();
    // finish programm
    return 0;    

}