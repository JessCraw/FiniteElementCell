#!/bin/bash

#SBATCH --job-name=acemodel
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=00:30:00
#SBATCH --mem=5980mb

ml foss

./build_debug/acemodel $SLURM_ARRAY_TASK_ID
