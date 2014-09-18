source ./scripts/helpers.sh
[[ $@ ]] || { echo "usage: measure.sh [name-of-measurement]"; exit 1; }
# how to run this --------------------------------------------------------------
# 1. add the following to ~/.ssh/config :
# Host cluster
    # HostName cluster.wr.informatik.uni-hamburg.de
    # User <your-cluster-user-name>
# 2. Add your SSH-key to the cluster
# 2.1 if you don't have a ssh key:
#   ssh-keygen -t rsa -C "<your-email>"
# 2.2 ssh-copy-id cluster

# customize the following script options -----------------------------------------
cluster_username=werner

# run config -------------------------------------------------------------------
# max is 120. starts with 2 ranks, increments by 2 until $max_ranks
min_rank=2
max_ranks=12

# global field size
x=2000
y=6000
rounds=1
num_attributes=4
int_size=4
size_bytes=$((x*y*int_size*num_attributes))
size_gb=$(bc <<< "scale=2; $size_bytes/1024^3")
#-------------------------------------------------------------------------------
# sync the project folder to the cluster
rsync -a --delete * cluster:/home/$cluster_username/mpi-try/

cells=$(pprint $((x*y)))
echo -e "\n Running with $cells cells ($size_gb GB)"

measurements_file=measurements/$1.csv

ssh cluster rounds=$rounds x=$x y=$y min_rank=$min_rank max_ranks=$max_ranks measurements_file=$measurements_file "bash -s" << 'ENDSSH'
cd ideas
make cluster

echo ""
rm -f $measurements_file
salloc -n $max_ranks

for n in $(seq $min_rank 2 $max_ranks); do 
  echo -ne "$n: " | tee -a $measurements_file
  rounds=$rounds x=$x y=$y mpiexec -n $n ./a.out | tee -a $measurements_file
done 

exit
ENDSSH

rsync -a cluster:/home/$cluster_username/ideas/mpi-try/$measurements_file $measurements_file

# graph output -----------------------------------------------------------------
./draw.sh
