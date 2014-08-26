ssh cluster "bash -s" << 'ENDSSH'
cd ideas-papo2014/
git pull
make slurm
cd mpi-try
./run-slurm.sh
sleep 20
ENDSSH
rm -rf ./slurm-out
scp -r cluster:/home/werner/ideas-papo2014/mpi-try/slurm-out/ .

echo "Errors:"
ssh cluster "echo ideas-papo2014/mpi-try/job.err"
echo "Result files:"
ls slurm-out
