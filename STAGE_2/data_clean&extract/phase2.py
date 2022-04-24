import pandas as pd
from pandas import DataFrame, Series
import numpy as np

data = pd.read_csv('dataset_modified.csv', sep = ',')
print(data.head())

data['date'] = data['date'].astype(str)
data['time'] = data['time'].astype(str)
data['type'] = data['type'].astype(str)
data['state'] = data['state'].astype(str)
data['location'] = data['location'].astype(str)
data['BE'] = data['BE'].astype(str)

#delete the rows without action from raw dataset
data1 = data[ ~ data['location'].isin(['nan'])]
print(data1.head())
df1 = pd.DataFrame(data1)
df1.to_csv('dataset_action.csv')

#delete repeated data from raw dataset
data2 = data.drop_duplicates(['date', 'time', 'type', 'state'], keep='first')
df2 = pd.DataFrame(data2)

for i in range(0,len(data2)-1):
    ts1 = data2['time'].iloc[i].split(':')
    ts2 = data2['time'].iloc[i+1].split(':') #difference of time smaller than 5 seconds
    ts1[2] = int(ts1[2])
    ts2[2] = int(ts2[2])
    if (ts2[2] - ts1[2] <= 5) and (data2['date'].iloc[i] == data2['date'].iloc[i+1]) and \
            (data2['type'].iloc[i] == data2['type'].iloc[i+1]) and (data2['state'].iloc[i] == data2['state'].iloc[i+1]):
        df2 = df2.drop([i+1])
        print(i+3)
#print(df2.str.split(':'))
df2.to_csv('dataset_no_repeat.csv')