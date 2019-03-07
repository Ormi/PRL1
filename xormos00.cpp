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
using namespace std; 

#include <algorithm>
#include <iterator>
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
    	int num, i=0;
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
       			i++;
       		}
        }  
        // print count of numbers to stdout
        cout << "\nNumber of numbers: " << i << "\n"; 
        cout << "Number of processes: " << processesCount << "\n";	
        // get bucket number
        bucketSize = round(i/((processesCount+1)/2));
        cout << "Bucket size: " << bucketSize << "\n";
        display_vector(numbers);
        // MSG_SIZE = 
        number = -1;
        // send data to cpu
        for(int i = 1; i < processesCount; i++) {
        	MPI_Send(&numbers[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            // MPI_Send(&number, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
            // MPI_Send(&number, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
        }        
    } else {
    	//SLAVES
        int recievedData;
	    MPI_Recv(&recievedData, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Process received number %d from process 0\n", recievedData);    	
        // int recievedData;
        // MPI_Recv(&numbers, MSG_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        // cout << "Data:" << numbers;

    }


    // releive all alocated processes
    MPI_Finalize();
    // finish programm
    return 0;    

}