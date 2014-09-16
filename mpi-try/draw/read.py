import os
from os.path import basename
from pprint import pprint
from draw import Draw

wd=os.path.dirname(os.path.realpath(__file__))
data_dir=os.path.join(wd,"data")


files = [data_dir + "/" + fname for fname in os.listdir(data_dir)]
files_sorted=sorted(files,key=lambda f: int(basename(f)))

file_contents = []
for fname in files_sorted:
    with open(fname) as f:
        file_contents.append(f.read().split("\n")[:-1])

step=[]
for f in file_contents:
    step_data = []
    for line in f:
        step_data.append([map(int,nums.strip().split(" ")) 
                    for nums in line.split(",")[:-1]])
    step.append(step_data)

Draw(step)
