import pygal                                                       # First import pygal
import csv
import sys

cells = sys.argv[1]

def read_file(path):
    with open(path) as f:
        return list(csv.reader(f, delimiter=':'))

f = read_file("measurements/measurements.csv")

x_labels = [col[0] for col in f]
y_values = [float(col[1]) for col in f]
# print(y_values);

chart = pygal.Bar()
chart.title = 'Performance (in seconds)'

chart.x_labels = x_labels
chart.add(cells + " cells", y_values)
chart.render_to_file('measurements/chart.svg')
