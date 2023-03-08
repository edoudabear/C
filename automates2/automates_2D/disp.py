import matplotlib.pyplot as plt
import matplotlib.animation as ani
import re
import numpy as np
import itertools as it
import time

tab = np.zeros((125, 250), dtype=int)

cols = {" ": 0, "1": 50, "2": 230, "3": 130, "4": 150, "5": 15, "6": 255, "7": 80,
                "+": 50, "*": 230, "@": 130, "x": 150, "o": 15, "#": 255, "%": 80,
                ">": 50, "^": 230, "<": 130, "v": 150,
                         ".": 230,           "O": 150,};

ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')

def filtered_input():
    line = input()
    return "H" in line, ansi_escape.sub('', line)

def update_array(tab):
    for i in it.count():
        if i<len(tab):
            vals = [cols[c] for c in curr[1]][:len(tab[i])]
       	    tab[i,:len(vals)] = vals
       	curr[:] = filtered_input()
        if curr[0]:
            return

curr = list(filtered_input())

def UpdateState(_) :
    update_array(tab)
    state.set_data(tab)
    return (state,)

fig = plt.figure("Affichage de l'automate", figsize=(11.5, 6))

ax1 = fig.add_subplot(111)
state = ax1.matshow(tab, cmap="nipy_spectral", vmin=0, vmax=255)
ax1.set_xticks([])
ax1.set_yticks([])

anim = ani.FuncAnimation(fig, UpdateState, it.count(0), interval=20, blit=True)

plt.show()

while True:
    time.sleep(1)
