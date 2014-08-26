
run() {
  sbatch -N $1 -n $2 --output=slurm-out/$1-$2 mpi.slurm
}
cd mpi-try
rm -r slurm-out/*

for N in $(seq 2 2 4); do 
  for n in $(seq 2 2 12); do 
    run $N $n
  done 
done
