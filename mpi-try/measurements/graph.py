import pygal
import csv
import sys
import os
# cells = sys.argv[1]

def read_file(path):
    with open(path) as f:
        return list(csv.reader(f, delimiter=':'))

csvs = {}
for f in os.listdir("measurements"):
    if f.endswith(".csv"):
        csvs[f.split(".")[0]] = read_file("measurements/" + f)


chart = pygal.Line()
chart.title = 'Performance'

for label, values in csvs.items():
    x_labels = [col[0] for col in values]
    chart.x_labels = x_labels

    y_values = [float(col[1]) for col in values]
    # chart.add(label + " absolute" , y_values)
    y_values = [(float(values[0][1])*int(values[0][0]))/(float(col[1])*int(col[0]))for col in values]
    chart.add(label + " relative" , y_values)

chart.render_to_file('measurements/chart.svg')
