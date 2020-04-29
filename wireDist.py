import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

fig,axs = plt.subplots ( ncols=2, sharey=True, figsize=(16,7) )
fig.subplots_adjust ( hspace=0.5, left=0.07, right=0.93 )
df = pd.read_csv('wireDist.csv')
df2 = pd.read_csv('capsDist.csv')

# plot the wire distribution
ax = axs[0]
hb = ax.hexbin(df.x,df.y,cmap='viridis')
ax.axis ( [df2.x.min(), df2.x.max(), df2.y.min(), df2.y.max() ] )
ax.set_title( 'Wire Map ( adaptec3 )' )

cb = fig.colorbar ( hb, ax = ax )
cb.set_label ('counts')

# plot the wire distribution
ax = axs[1]
hb = ax.hexbin(df2.x,df2.y,cmap='viridis')
ax.axis ( [df2.x.min(), df2.x.max(), df2.y.min(), df2.y.max() ] )
ax.set_title( 'Capacity Map ( adaptec3 )' )

cb = fig.colorbar ( hb, ax = ax )
cb.set_label ('counts')

plt.show()