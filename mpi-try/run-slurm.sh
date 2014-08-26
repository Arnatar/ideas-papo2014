sbatch -N $1 -n $2 --output=slurm-out/out-$1-$2.out mpi.slurm
sleep 3
cat slurm-out/out-$1-$2.out
