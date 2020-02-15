import os
import numpy as np
files = np.array(['main', 'Global', 'Utilities', 'Entity', 'Game', 'BackgroundBack', 'BackgroundSide', 'Player', 'Platforms']) # FILL IN THE FILES NAMES #
nfiles = files.size

os.system('rm ./game.out')

for i in range (0, nfiles):
    os.system('g++ -c '+files[i]+'.cpp')

string = ''
for i in range (0, nfiles):
    string += files[i] + '.o '   
os.system('g++ '+string+'-o game.out -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network')

for i in range (0, nfiles):
    os.system('rm ' + files[i] + '.o')
    
os.system('./game.out')