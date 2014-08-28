import os
from pprint import pprint
from draw import Draw

wd=os.path.dirname(os.path.realpath(__file__))
data_dir=os.path.join(wd,"data")


files = [open(data_dir + "/" + fname) for fname in os.listdir(data_dir)]
files=[f.read().split("\n")[:-1] for f in files]

step=[]
for f in files:
    step_data = []
    for line in f:
        step_data.append([map(int,nums.strip().split(" ")) 
                    for nums in line.split(",")[:-1]])
    step.append(step_data)


for i, s in enumerate(step):
    if i>0:
        print(step[i]==step[i-1])
