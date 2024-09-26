# Get the interface position and velocity at r = 0 and also save the interface that can be used for visualization
# this code triggers getFacet and getEtaNvz. getFacet is used to get the interface in the form of segments
# and getEtaNvz is used to get the interface position and velocity at r = 0. the latter will save the interface position and velocity in a csv file...

# Author: Vatsal Sanjay & Aman Bhargava
# vatsalsanjay@gmail.com, amanbhargava2000@gmail.com
# Physics of Fluids
# v1.0
# Last updated: Sep 26, 2024

import numpy as np
import os
import subprocess as sp
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
from matplotlib.ticker import StrMethodFormatter
import sys

matplotlib.rcParams['font.family'] = 'serif'
matplotlib.rcParams['text.usetex'] = True

def gettingFacets(filename):
    exe = ["./getFacet", filename]
    p = sp.Popen(exe, stdout=sp.PIPE, stderr=sp.PIPE)
    stdout, stderr = p.communicate()
    temp1 = stderr.decode("utf-8")
    temp2 = temp1.split("\n")
    segs = []
    skip = False
    if (len(temp2) > 1e2):
        for n1 in range(len(temp2)):
            temp3 = temp2[n1].split(" ")
            if temp3 == ['']:
                skip = False
                pass
            else:
                if not skip:
                    temp4 = temp2[n1+1].split(" ")
                    r1, z1 = np.array([float(temp3[1]), float(temp3[0])])
                    r2, z2 = np.array([float(temp4[1]), float(temp4[0])])
                    segs.append(((r1, z1),(r2,z2)))
                    segs.append(((-r1, z1),(-r2,z2)))
                    skip = True
    return segs

# ----------------------------------------------------------------------------------------------------------------------


Ldomain = 4.282
mean = 2.141
rmin, rmax, zmin, zmax = [0.0, Ldomain, -mean, Ldomain-mean]
lw = 4

writefile = "etaNvz.csv"
if os.path.exists(writefile):
    os.remove(writefile)

folder = 'VideoInterface'  # output folder
if not os.path.isdir(folder):
    os.makedirs(folder)

nGFS=1000

for ti in range(nGFS):
    t = ti*(1e-3)
    print("Time is %f" % t)

    place = "intermediate/snapshot-%5.4f" % t
    name = "%s/%8.8d.png" %(folder, int(t*1000000))

    if not os.path.exists(place):
        print("%s File not found!" % place)
    else:
        if os.path.exists(name):
            print("%s Image present!" % name)
        else:

            segs = gettingFacets(place)
            if (len(segs) == 0):
                print("Problem in the available file %s" % place)
            else:
                exe = ["./getEtaNvz", place, writefile]
                p = sp.Popen(exe, stdout=sp.PIPE, stderr=sp.PIPE)
                stdout, stderr = p.communicate()
                temp1 = stderr.decode("utf-8")
                temp2 = temp1.split("\n")
                print(temp2)
                temp3 = temp2[0].split(",")
                r, z, v = np.array([float(temp3[0]), float(temp3[1]), float(temp3[2])])
                # print(zminp, zmaxp, rminp, rmaxp)
                # Part to plot
                AxesLabel, TickLabel = [50, 20]
                fig, ax = plt.subplots()
                fig.set_size_inches(19.20, 10.80)

                ## Drawing Facets
                line_segments = LineCollection(segs, linewidths=4, colors='#1a9850', linestyle='solid')
                ax.add_collection(line_segments)

                ax.plot([0, 0], [zmin, zmax],'-.',color='grey',linewidth=lw)
                # ax.plot([rmin, rmax], [0, 0],'-',color='grey',linewidth=lw/2)
                ax.plot([-rmax, -rmax], [zmin, zmax],'-',color='black',linewidth=lw)
                ax.plot([-rmax, rmax], [zmin, zmin],'-',color='black',linewidth=lw)
                ax.plot([-rmax, rmax], [zmax, zmax],'-',color='black',linewidth=lw)
                ax.plot([rmax, rmax], [zmin, zmax],'-',color='black',linewidth=lw)
                ax.plot(r, z, 'o', color='red', markersize=10)

                ax.set_aspect('equal')
                ax.set_xlim(-rmax, rmax)
                ax.set_ylim(zmin, zmax)
                ax.set_title('$t$ = %5.4f' % t, fontsize=TickLabel)

                ax.axis('off')
                # plt.show()
                plt.savefig(name, bbox_inches="tight")
                plt.close()
