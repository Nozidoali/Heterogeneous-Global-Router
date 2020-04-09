import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('static.csv')
print( 'TOF: ' + str(df.overflow.sum()) )
print( 'TWL: ' + str(df.wirelength.sum()) )

# df.plot.scatter( x = 'wirelength', y = 'overflow' )
df[ df.overflow > 2 ].overflow.hist( bins = 100 )
# df[ df.Area < 100 ].Area.hist( bins = 100 )
plt.show()