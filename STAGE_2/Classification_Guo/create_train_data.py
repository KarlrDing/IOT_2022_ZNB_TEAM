import pandas as pd
from pandas import DataFrame, Series
from datetime import timedelta
import numpy as np
import matplotlib.pyplot as plt

data1 = pd.read_csv('sensor_data.csv')
column_names = data1['type'].unique()
action_names = data1['location'].unique()
print(column_names)

data2 = np.zeros((1000, 71))
df = pd.DataFrame(data2, columns=column_names)
df['action'] = np.nan
df.to_csv('sensor_frequency.csv')

ind = 0
a = 0
dflist = []
for m in range(0, len(column_names)-1):
    for i in range(0, len(data1)):
        if column_names[m] == data1['type'].iloc[i]:
            a = a + 1
            df[column_names[m]].iloc[ind] = a #frequency or appear
            df.to_csv('sensor_frequency.csv')
        if data1['BE'].iloc[i] == 'begin':#and (data['date'].iloc[i] == '02/02/2009')
            counter1 = i
            count = 1
        if data1['BE'].iloc[i] == 'end':
            counter2 = i
            count = 2
        if count == 2:
            df['action'].iloc[ind] = data1['location'].iloc[i]
            ind = ind + 1
            df.to_csv('sensor_frequency.csv')
            #dflist.append(df)
            #appended_data = pd.concat(dflist)
            #appended_data.to_csv('sensor_appear.csv')
            count = 0
            counter1 = 0
            counter2 = 0
            a = 0
            print(df)
    ind = 0

'''for m in range(len(column_names)):#column num
    for i in range(len(data1)): #observation num
        if column_names[m] == data1['type'].iloc[i]:
            ind = ind + 1
            df[column_names[m]].iloc[ind] = 1
            df['action'].iloc[ind] = data1['location'].iloc[i]
df.to_csv('sensor_appear.csv')'''