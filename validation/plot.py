#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import sys, os, glob
import numpy
import matplotlib.pyplot as plt

import on_key

globber = glob.glob(os.path.join("output", "*"))
globber.sort()

fig = on_key.figure()

colors  = ['b', 'r', 'm', 'c', 'g', 'y']
symbols = ['-', '--', ':', '-.']
line_width = 2

fi = 0
for prng_file in globber:

    # Skip folders
    if (not os.path.isfile(prng_file) or prng_file == "output/make_run.log"):
        continue

    print "prng_file =", prng_file

    data = numpy.loadtxt(prng_file, dtype=float)

    plt.hist(data, bins=100, label=prng_file.replace("output/",""), alpha=0.5)


plt.ylabel("Occurence")
plt.xlabel("Value")
plt.title("PRNG occurence")

plt.legend()

plt.show()

