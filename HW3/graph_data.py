import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

filename = "red.csv"

df = pd.read_csv(filename)

index = range(1, len(df['R']) + 1)

plt.rcParams["figure.figsize"] = (20,10)

plt.plot(index, df['R'], 'r.', label='x', linestyle='solid', marker=',')
plt.plot(index, df['G'], 'g.', label='y', linestyle='solid', marker=',')
plt.plot(index, df['B'], 'b.', label='z', linestyle='solid', marker=',')
plt.title("Red")
plt.xlabel("Sample #")
plt.ylabel("RGB Level (RGB)")
plt.legend()
plt.show()