import sys
import os
import time
time.sleep(.1)
number = sys.argv[1]

os.system("tmux new-session -s socketSession -d \"./killParent ./socketDemo" + str(sys.argv[1]) + "\"")
os.system("tmux split-window -v \"./killParent ./socketDemo" + str(sys.argv[1]) + " client\"")
os.system("tmux -2 attach-session -d")

