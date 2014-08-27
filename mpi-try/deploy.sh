source ./scripts/helpers.sh

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

# the python environment should have pygal installed
# recommended setup procedure:
#   - in mpi-try-folder: virtualenv .virtualenv
#   - in .bashrc: PATH=.virtualenv/bin:$PATH
#   - open new terminal
#   - in mpi-try-folder: pip install -r requirements.txt
python=./.virtualenv/bin/python
svg_viewer=google-chrome-stable

measurements_file=measurements/measurements.csv

# run config -------------------------------------------------------------------
# max is 120. starts with 2 ranks, increments by 2 until $max_ranks
max_ranks=30

# global field size
x=2000
y=60000

#-------------------------------------------------------------------------------
# sync the project folder to the cluster
rsync -a --delete --exclude presentation --exclude src --exclude '.git' ../* cluster:/home/$cluster_username/ideas/

cells=$(pprint $((x*y)))
echo -e "\n Running with $cells cells.\n"

ssh cluster x=$x y=$y max_ranks=$max_ranks measurements_file=$measurements_file "bash -s" << 'ENDSSH'
cd ideas
make slurm > /dev/null
cd mpi-try

echo ""
rm -f $measurements_file
salloc -n $max_ranks

for n in $(seq 2 2 $max_ranks); do 
  echo -ne "$n: " | tee -a $measurements_file
  x=$x y=$y mpiexec -n $n ./a.out | tee -a $measurements_file
done 

exit
ENDSSH

rsync -a cluster:/home/$cluster_username/ideas/mpi-try/$measurements_file $measurements_file

# graph output -----------------------------------------------------------------
$python ./measurements/graph.py $cells
$svg_viewer measurements/chart.svg

