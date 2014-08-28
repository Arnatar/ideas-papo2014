# the python environment should have pygal installed
# recommended setup procedure:
#   - in mpi-try-folder: virtualenv .virtualenv
#   - in .bashrc: PATH=.virtualenv/bin:$PATH
#   - open new terminal
#   - in mpi-try-folder: pip install -r requirements.txt
python=./.virtualenv/bin/python
svg_viewer=google-chrome-stable

$python ./measurements/graph.py
$svg_viewer measurements/chart.svg
