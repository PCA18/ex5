#!/usr/bin/env bash
#SBATCH --partition=bench
#SBATCH --nodes 4

for var in 1000 10000 100000 1000000
do  
    for threads in 1 2 4 8 16 32
    do
		srun ./pi_approx.exe $threads $var
	done
done
		
