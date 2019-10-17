#%%
import numpy as numpy
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()
#%%
import os
#%%
PATH = "/mnt/programming/data/MO/tokopuyo/recognition_data10/clear.csv"

data = pd.read_csv(PATH)
#%%
data.plot(y=['H', 'S', 'V'], bins=50, alpha=0.5, figsize=(10, 4), stacked=True, kind='hist')
#%%
data.head()
#%%
clear_data = ((data["H"] < 45) & (100 < data["S"]) & (210 < data["V"]))
print(H_data.sum())
#%%
H_data = data["H"] < 45
print(H_data.sum())
#%%
S_data = data["S"] > 100
print(S_data.sum())
#%%
V_data = data["V"] > 200
print(V_data.sum())
#%%