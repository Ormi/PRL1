#!/bin/bash
# PRL Project 1
# @author Michal Ormos
# @email xormos00@stud.fit.vutbr.cz
# @date Match 2019

# function power2() { 
# 	echo "x=l($1)/l(2); scale=0; 2^((x+0.5)/1)" | bc -l; 
# }

# echo 'l(20)/l(2)' | bc -l


# function num_of_processes {
# 	z=$(log2 $1)
# }

if [ "$#" -gt 1 ]; then #there are more parameters than 1
    echo "Illegal number of parameters"
    exit 1
elif [ "$#" -eq 1 ]; then #there is one parameter
    numbers=$1;
else #there is no parameter
    numbers=5;
fi

#create random numbers file
dd if=/dev/urandom bs=1 count=$numbers of=numbers status=noxfer > /dev/null 2>&1
# dd if=/dev/random bs=1 count=$numbers of=numbers 2>/dev/null

#compile
mpic++ --prefix /usr/local/share/OpenMPI -o es es.cpp
mpic++ --prefix /usr/local/share/OpenMPI -o xormos00 xormos00.cpp

#number of processes neeed to be equal to number of numbers + 1
echo $numbers

float=1.23
int=${float%.*}
# get the logarith of 2 of array length
z=$(echo "l($numbers)/l(2)" | bc -l)
zz=${z%.*}
echo $z
echo $zz

# get the log of 2 second time
a=$(echo "l($zz)/l(2)" | bc -l)
aa=${a%.*}
echo $a
echo $aa

# round to higher int
# @TODO
b=$(echo "($a+0.999)/1" | bc)
echo $b

# round it to nearest higher power of 2
c=$(echo "2^$b" | bc -l; )
echo $c

# substract 1 from result
processes=$(echo "2*$c - 1" | bc -l;)
echo "====="
echo $processes
echo $numbers
echo "===="

#run
mpirun --prefix /usr/local/share/OpenMPI -np $numbers es
mpirun --prefix /usr/local/share/OpenMPI -np $processes xormos00

#remove
# rm -f es numbers