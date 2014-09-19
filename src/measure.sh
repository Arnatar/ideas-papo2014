

_rsync() {
  rsync -a --info=progress2 "$@"
}

echo -e "\n[1] copying source files to cluster ...\n"
_rsync --exclude log/ --exclude out/ --exclude draw/ --delete * cluster:/home/$cluster_username/mpi-try/

# num_attributes=4
# int_size=4
# size_bytes=$((x*y*int_size*num_attributes))
# size_gb=$(bc <<< "scale=2; $size_bytes/1024^3")
#-------------------------------------------------------------------------------

# cells=$(pprint $((x*y)))
# echo -e "\n Running with $cells cells ($size_gb GB)"

measurements_file=measurements/$outfile

ssh cluster proc_increase_step=$proc_increase_step x=$x y=$y ideas=$ideas rounds=$rounds min_procs=$min_procs max_procs=$max_procs measurements_file=$measurements_file "bash -s" << 'ENDSSH'
cd mpi-try
echo -e "\n[2] Building..\n"
make cluster_build_measure

echo ""
rm -f $measurements_file
echo -e "\n[3] Allocating a maximum of $max_procs procs.\n"
salloc -n $max_procs

echo -e "\n[4] Running tests.\n"
for n in $(seq $min_procs $proc_increase_step $max_procs); do 
  echo -ne "$n: " | tee -a $measurements_file
	x=$x y=$y ideas=$ideas rounds=$rounds mpirun -np $n ./a.out | tee -a $measurements_file
done 

exit
ENDSSH

echo -e "\n[5] copying measurements to mpi-try/$measurements_file\n"
rsync -a cluster:/home/$cluster_username/mpi-try/$measurements_file $measurements_file

