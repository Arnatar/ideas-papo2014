#!/bin/sh
	
# Time limit is one minute.
# See "man sbatch" for other time formats.
#SBATCH --time=1
# Run 12 tasks on 2 nodes.
#SBATCH -N 2 -n 12
# Output goes to job.out, error messages to job.err.
#SBATCH --error=job.err --output=job.out
	
mpiexec ./a.out
