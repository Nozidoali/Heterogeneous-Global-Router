import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

fig,ax = plt.subplots ( ncols=1, sharey=True, figsize=(8,7) )
fig.subplots_adjust ( hspace=0.5, left=0.07, right=0.93 )

df = pd.read_csv('pinDist.csv')

hb = ax.hexbin(df.x,df.y,cmap='viridis')
ax.axis ( [df.x.min(), df.x.max(), df.y.min(), df.y.max() ] )
ax.set_title( 'Pin Map ( adaptec1 )' )

cb = fig.colorbar ( hb, ax = ax )
cb.set_label ('counts')

plt.show()