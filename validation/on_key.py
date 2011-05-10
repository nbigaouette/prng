#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import pylab, sys


###################################################################
def on_key(event):
    #print 'you pressed', event.key, event.xdata, event.ydata
    if   (event.key == 'q'):
        sys.exit(0)
    #
    elif (event.key == 'w'):
        pylab.close(pylab.gcf())
    #
    elif (event.key == 'd'):
        print "##############################################################"
        print "Please click two points to get the distance and slope."
        points = pylab.ginput(n=2, show_clicks=True, timeout=0)
        dx = points[1][0] - points[0][0]
        dy = points[1][1] - points[0][1]
        dy_log = pylab.log10(points[0][1]) - pylab.log10(points[1][1])
        dy_ln = pylab.log(points[0][1]) - pylab.log(points[1][1])
        angle = pylab.arctan2(dy,dx)
        print "points = ", points
        print "distance (x) =", dx
        print "distance (y) =", dy
        print "distance =", pylab.sqrt( dx**2 + dy**2 )
        print "Angle: theta = atan2(y/x) =", angle, "rad =", angle*180.0/pylab.pi, "deg"
        print "Slope: ", dy/dx, "  1/slope =", dx/dy
        print "Slope (log10 scale):", dy_log/dx
        print "Slope (ln scale):",    dy_ln/dx
        pylab.plot([points[0][0], points[1][0]], [points[0][1], points[1][1]], '--r', lw=1.0)
        pylab.plot([points[0][0], points[1][0]], [points[0][1], points[1][1]],  '+r', lw=1.0)
        pylab.draw()
    #
    elif (event.key == 'a'):
        print "##############################################################"
        print "Please click a point to get the position."
        #from matplotlib.widgets import Cursor
        #cursor = Cursor(pylab.gca(), useblit=True, color='red', linewidth=2 )
        point = pylab.ginput(n=1, show_clicks=False, timeout=0)
        print "point = ", point
        pylab.gca().plot(point[0][0], point[0][1],  '+r', lw=1.0)
        pylab.draw()
#
###################################################################

def figure():
    fig = pylab.figure()
    fig.canvas.mpl_connect('key_press_event', on_key)
    return fig
