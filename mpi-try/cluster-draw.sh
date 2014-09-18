# [[ $@ ]] || { echo "usage: measure.sh [name-of-measurement]"; exit 1; }
# how to run this --------------------------------------------------------------
# 1. add the following to ~/.ssh/config :
# Host cluster
    # HostName cluster.wr.informatik.uni-hamburg.de
    # User <your-cluster-user-name>
# 2. Add your SSH-key to the cluster
# 2.1 if you don't have a ssh key:
#   ssh-keygen -t rsa -C "<your-email>"
# 2.2 ssh-copy-id cluster

_rsync() {
  rsync -a --info=progress2 "$@"
}

cluster_username=werner
echo -e "\n[1] copying source files to cluster ...\n"
_rsync --exclude log/ --exclude out/ --exclude draw/ --delete * cluster:/home/werner/mpi-try/

ssh cluster procs=$procs "bash -s" << 'ENDSSH'
cd mpi-try
echo -e "\n[2] Running on $procs nodes..\n"
salloc -n $procs 
echo ""
make cluster 
echo ""
# exit
ENDSSH

echo -e "\n[3] copying draw/data/* to local folder ...\n"
_rsync cluster:/home/$cluster_username/mpi-try/draw/data/* draw/data/
echo -e "\nFolder size draw/data: $(du -sh draw/data/ | awk ' { print $1 }').\n"
