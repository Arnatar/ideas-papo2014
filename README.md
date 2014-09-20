# ideas-papro2014
This project simulates the movement of people in a grid-world. Those people have "ideas", wich have certain properties: they have a "quality", a "complexity", a "world view" index (which is an abstraction to distinguish ideas based on their "content"). Each human also has an own "world view" index, which is an abstraction for a general "attitude" or disposition one has. 

The human now try to convince other people of their ideas. That is, they wander around randomly and if they "see" another human in their direct periphery, they might communicate with him. Based on some involved factors, which are formulated  per C-code in `src/logic.c`, they might convince the other person or be convinced themselves by the other persons idea. The losing idea is then modified in such a way that it takes on some properties of the winning idea.

There is also a slight chance of ideas mutating themselves to account for some dynamic in the thinking individual on its own.

This whole simulation can be visualized, where people are points on a quadratic field, their ideas "world view index" (which ranges from 0 to 9 for the sake of visualization) is displayed as a unique color, and the quality of the idea determines the "brightness" of the point. You can then experiment with different parameters (for example different densities of ideas on the field) and see certain patterns emerge, e.g the formation of clusters of ideas with the same "world view", which stabilize themselves as they get a higher average "quality".


### dependencies / setup locally
- `mpich2`
- python 2.x
- `pygal` for measurement graphs
- `pygame` for visualization

### setup for running on cluster
To run/measure on cluster: 

1. Add the following to your ~/.ssh/config :
  1. Host cluster
    HostName cluster.wr.informatik.uni-hamburg.de
    User <your-cluster-user-name>
2. Add your SSH-key to the cluster
  1. If you don't have a ssh key: `ssh-keygen -t rsa -C "<your-email>"`
  2. `ssh-copy-id cluster`
3. Search for `cluster_username` in src/Makefile and put in your cluster username
4. Then look for `python_bin` (~10 lines down) and set it to a python bin that has `pygal`

### run
Run `make <config>` from `src`-directory, where `<config>` is one of the following:
- `debug`: run with an extraordinary amount of logging output, which displays the arrays from each proc under each other, per round. this is meant to be used for testing the ins and outs of the MPI communication and the integrity of the movement of the ideas over the field.
- `pygame`: run with visualization per pygame. optional gif output. 
- `pygame_cluster`: run the computation on the cluster, copy the output files to local.  unfortunately the big bottleneck here is sending back the rather large amount of output files to local per rsync.
- `cluster_measure`: this runs the program on the cluster with a configurable range of processes and graphs the results locally per python + pygal.

You can find all configurable parameters in the Makefile.
You can find some constants in constans.h if you desire to tweak the ideas behaviour yourself.