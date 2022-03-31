# -*- coding: utf-8 -*-
"""
Created on Fri Mar 18 11:28:11 2022

@author: ajitb
"""

import matplotlib.pyplot as plt
#import collections

file_txt1 = open('rand_std', 'r')
lines1 = [line.split(' ') for line in file_txt1.readlines()]
column1 = [int(line[0]) for line in lines1]
values1 = [float(line[1]) for line in lines1]
nb_lines = len(column1)


file_txt2 = open('rand_naive', 'r')
lines2 = [line.split(' ') for line in file_txt2.readlines()]
column2 = [int(line[0]) for line in lines2]
values2 = [float(line[1]) for line in lines2]


plt.plot(column1,values1,label='std')
plt.plot(column1,values2,label='naive')

plt.xlabel('Set size')
plt.ylabel('Avg no of rotations')
plt.title('random')
plt.legend()
plt.scale('log')
plt.show()