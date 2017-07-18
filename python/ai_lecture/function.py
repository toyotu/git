#!/usr/bin/env python

import sys, os, math
import matplotlib.pyplot as plt
import numpy as np 
# import numpy as np
# import scipy as sp
# import ROOT
# import pyfits as pf

dx = 0.1
x = np.arange(-5, 5, dx)

e = math.e
y_2 = 2**x
y_e = e**x
y_3 = 3**x

y_de = (e**(x+dx) - e**x) /dx
print y_de

# plt.plot(x, y_2)
plt.plot(x, y_e)
plt.plot(x, y_de)
# plt.plot(x, y_3)

plt.show()
