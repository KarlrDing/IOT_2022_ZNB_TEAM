from datetime import timedelta
import pandas as pd
from pandas import DataFrame, Series
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv('dataset_modified_v2.csv', sep = ',')
print(data.head())

data['date'] = data['date'].astype(str)
data['time'] = data['time'].astype(str)
data['type'] = data['type'].astype(str)
data['state'] = data['state'].astype(str)
data['location'] = data['location'].astype(str)
data['BE'] = data['BE'].astype(str)

data1 = data
count = 0
counter1 = 0
counter2 = 0
dflist = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'R1_Bed_to_Toilet') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'R1_Bed_to_Toilet') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data1 = data.iloc[counter1:counter2+1]
        df1 = pd.DataFrame(data1)
        df1.to_csv('sensor_data_Bed.csv')
        dflist.append(df1)
        appended_data = pd.concat(dflist)
        appended_data.to_csv('sensor_data_Bed.csv')
        count = 0
        counter1 = 0
        counter2 = 0
        #print(data1)

df2list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'R1_Personal_Hygiene') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'R1_Personal_Hygiene') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data2 = data.iloc[counter1:counter2 + 1]
        df2 = pd.DataFrame(data2)
        df2.to_csv('sensor_data_Hygiene.csv')
        df2list.append(df2)
        appended_data = pd.concat(df2list)
        appended_data.to_csv('sensor_data_Hygiene.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df3list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'R2_Bed_to_Toilet') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'R2_Bed_to_Toilet') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data3 = data.iloc[counter1:counter2 + 1]
        df3 = pd.DataFrame(data3)
        df3.to_csv('sensor_data_Bed2.csv')
        df3list.append(df3)
        appended_data = pd.concat(df3list)
        appended_data.to_csv('sensor_data_Bed2.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df4list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'Meal_Preparation') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'Meal_Preparation') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data4 = data.iloc[counter1:counter2 + 1]
        df4 = pd.DataFrame(data4)
        df4.to_csv('sensor_data_Meal.csv')
        df4list.append(df4)
        appended_data = pd.concat(df4list)
        appended_data.to_csv('sensor_data_Meal.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df5list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'R2_Personal_Hygiene') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'R2_Personal_Hygiene') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data5 = data.iloc[counter1:counter2 + 1]
        df5 = pd.DataFrame(data5)
        df5.to_csv('sensor_data_Hygiene2.csv')
        df5list.append(df5)
        appended_data = pd.concat(df5list)
        appended_data.to_csv('sensor_data_Hygiene2.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df6list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'Watch_TV') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'Watch_TV') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data6 = data.iloc[counter1:counter2 + 1]
        df6 = pd.DataFrame(data6)
        df6.to_csv('sensor_data_TV.csv')
        df6list.append(df6)
        appended_data = pd.concat(df6list)
        appended_data.to_csv('sensor_data_TV.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df7list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'R1_Work') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'R1_Work') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data7 = data.iloc[counter1:counter2 + 1]
        df7 = pd.DataFrame(data7)
        df7.to_csv('sensor_data_Work.csv')
        df7list.append(df7)
        appended_data = pd.concat(df7list)
        appended_data.to_csv('sensor_data_Work.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df8list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'R2_Work') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'R2_Work') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data8 = data.iloc[counter1:counter2 + 1]
        df8 = pd.DataFrame(data8)
        df8.to_csv('sensor_data_Work2.csv')
        df8list.append(df8)
        appended_data = pd.concat(df8list)
        appended_data.to_csv('sensor_data_Work2.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df9list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'R1_Sleep') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'R1_Sleep') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data9 = data.iloc[counter1:counter2 + 1]
        df9 = pd.DataFrame(data9)
        df9.to_csv('sensor_data_Sleep.csv')
        df9list.append(df9)
        appended_data = pd.concat(df9list)
        appended_data.to_csv('sensor_data_Sleep.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df10list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'R2_Sleep') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'R2_Sleep') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data10 = data.iloc[counter1:counter2 + 1]
        df10 = pd.DataFrame(data10)
        df10.to_csv('sensor_data_Sleep2.csv')
        df10list.append(df10)
        appended_data = pd.concat(df10list)
        appended_data.to_csv('sensor_data_Sleep2.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df11list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'Wash_Bathtub') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'Wash_Bathtub') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data11 = data.iloc[counter1:counter2 + 1]
        df11 = pd.DataFrame(data11)
        df11.to_csv('sensor_data_Bathtub.csv')
        df11list.append(df11)
        appended_data = pd.concat(df11list)
        appended_data.to_csv('sensor_data_Bathtub.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df12list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'Clean') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'Clean') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data12 = data.iloc[counter1:counter2 + 1]
        df12 = pd.DataFrame(data12)
        df12.to_csv('sensor_data_Clean.csv')
        df12list.append(df12)
        appended_data = pd.concat(df12list)
        appended_data.to_csv('sensor_data_Clean.csv')
        count = 0
        counter1 = 0
        counter2 = 0

df13list = []
for i in range(0, len(data)):
    if (data['location'].iloc[i] == 'Study') and (data['BE'].iloc[i] == 'begin'):#and (data['date'].iloc[i] == '02/02/2009')
        counter1 = i
        count = 1
    if (data['location'].iloc[i] == 'Study') and (data['BE'].iloc[i] == 'end'):
        counter2 = i
        count = 2
    if count == 2:
        data13 = data.iloc[counter1:counter2 + 1]
        df13 = pd.DataFrame(data13)
        df13.to_csv('sensor_data_Study.csv')
        df13list.append(df13)
        appended_data = pd.concat(df13list)
        appended_data.to_csv('sensor_data_Study.csv')
        count = 0
        counter1 = 0
        counter2 = 0

data1 = pd.read_csv('sensor_data_Bed.csv')
for i in range(len(data1)):
    data1['location'].iloc[i] = 'R1_Bed_to_Toilet'
data1.to_csv('sensor_data_Bed.csv')

data2 = pd.read_csv('sensor_data_Hygiene.csv')
for i in range(len(data2)):
    data2['location'].iloc[i] = 'R1_Personal_Hygiene'
data2.to_csv('sensor_data_Hygiene.csv')

data3 = pd.read_csv('sensor_data_Bed2.csv')
for i in range(len(data3)):
    data3['location'].iloc[i] = 'R2_Bed_to_Toilet'
data3.to_csv('sensor_data_Bed2.csv')

data4 = pd.read_csv('sensor_data_Meal.csv')
for i in range(len(data4)):
    data4['location'].iloc[i] = 'Meal_Preparation'
data4.to_csv('sensor_data_Meal.csv')

data5 = pd.read_csv('sensor_data_Hygiene2.csv')
for i in range(len(data5)):
    data5['location'].iloc[i] = 'R2_Personal_Hygiene'
data5.to_csv('sensor_data_Hygiene2.csv')

data6 = pd.read_csv('sensor_data_TV.csv')
for i in range(len(data6)):
    data6['location'].iloc[i] = 'Watch_TV'
data6.to_csv('sensor_data_TV.csv')

data7 = pd.read_csv('sensor_data_Work.csv')
for i in range(len(data7)):
    data7['location'].iloc[i] = 'R1_Work'
data7.to_csv('sensor_data_Work.csv')

data8 = pd.read_csv('sensor_data_Work2.csv')
for i in range(len(data8)):
    data8['location'].iloc[i] = 'R2_Work'
data8.to_csv('sensor_data_Work2.csv')

data9 = pd.read_csv('sensor_data_Sleep.csv')
for i in range(len(data9)):
    data9['location'].iloc[i] = 'R1_Sleep'
data9.to_csv('sensor_data_Sleep.csv')

data10 = pd.read_csv('sensor_data_Sleep2.csv')
for i in range(len(data10)):
    data10['location'].iloc[i] = 'R2_Sleep'
data10.to_csv('sensor_data_Sleep2.csv')

data11 = pd.read_csv('sensor_data_Bathtub.csv')
for i in range(len(data11)):
    data11['location'].iloc[i] = 'Wash_Bathtub'
data11.to_csv('sensor_data_Bathtub.csv')

data12 = pd.read_csv('sensor_data_Clean.csv')
for i in range(len(data12)):
    data12['location'].iloc[i] = 'Clean'
data12.to_csv('sensor_data_Clean.csv')

data13 = pd.read_csv('sensor_data_Study.csv')
for i in range(len(data13)):
    data13['location'].iloc[i] = 'Study'
data13.to_csv('sensor_data_Study.csv')

result = pd.concat([data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13], keys = ['data1', 'data2', 'data3', 'data4', 'data5', 'data6', 'data7', 'data8', 'data9', 'data10', 'data11', 'data12', 'data13'])
result.to_csv('sensor_data.csv')
