#!/bin/bash
#SBATCH --job-name="Unknown"
#SBATCH --partition=debug
#SBATCH --nodes=10
#SBATCH --time=0-00:00:15
#SBATCH --ntasks-per-node=1
#SBATCH --mem=1992

mpirun -np 10 prog
