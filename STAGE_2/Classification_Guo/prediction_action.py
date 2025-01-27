import pandas as pd
import sklearn
from sklearn import svm
from sklearn.utils import shuffle
from sklearn.neighbors import KNeighborsClassifier
import numpy as np
from sklearn import linear_model, preprocessing
import matplotlib.pyplot as plt
import sklearn.metrics as metrics
from itertools import cycle
from sklearn.preprocessing import label_binarize
from sklearn.multiclass import OneVsRestClassifier
from interpolation import interp
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import mean_squared_error,accuracy_score, log_loss, confusion_matrix, precision_score, recall_score, f1_score

data = pd.read_csv('train_adjust.csv')
print(data.head())

le = preprocessing.LabelEncoder()
event = le.fit_transform(list(data['event']))
M35 = le.fit_transform(list(data['M35']))
AD1A = le.fit_transform(list(data['AD1-A']))
M34 = le.fit_transform(list(data['M34']))
M32 = le.fit_transform(list(data['M32']))
M33 = le.fit_transform(list(data['M33']))
M36 = le.fit_transform(list(data['M36']))
M30 = le.fit_transform(list(data['M30']))
M29 = le.fit_transform(list(data['M29']))
M37 = le.fit_transform(list(data['M37']))
M38 = le.fit_transform(list(data['M38']))
M39 = le.fit_transform(list(data['M39']))
M41 = le.fit_transform(list(data['M41']))
M40 = le.fit_transform(list(data['M40']))
M31 = le.fit_transform(list(data['M31']))
M10 = le.fit_transform(list(data['M10']))
M14 = le.fit_transform(list(data['M14']))
M16 = le.fit_transform(list(data['M16']))
M15 = le.fit_transform(list(data['M15']))
M17 = le.fit_transform(list(data['M17']))
M51 = le.fit_transform(list(data['M51']))
M18 = le.fit_transform(list(data['M18']))
AD1B = le.fit_transform(list(data['AD1-B']))
M09 = le.fit_transform(list(data['M09']))
D08 = le.fit_transform(list(data['D08']))
M45 = le.fit_transform(list(data['M45']))
M47 = le.fit_transform(list(data['M47']))
M48 = le.fit_transform(list(data['M48']))
M28 = le.fit_transform(list(data['M28']))
M27 = le.fit_transform(list(data['M27']))
M46 = le.fit_transform(list(data['M46']))
M49 = le.fit_transform(list(data['M49']))
M23 = le.fit_transform(list(data['M23']))
M19 = le.fit_transform(list(data['M19']))
M42 = le.fit_transform(list(data['M42']))
M08 = le.fit_transform(list(data['M08']))
M43 = le.fit_transform(list(data['M43']))
M13 = le.fit_transform(list(data['M13']))
M50 = le.fit_transform(list(data['M50']))
D14 = le.fit_transform(list(data['D14']))
M21 = le.fit_transform(list(data['M21']))
L11 = le.fit_transform(list(data['L11']))
M06 = le.fit_transform(list(data['M06']))
L06 = le.fit_transform(list(data['L06']))
M24 = le.fit_transform(list(data['M24']))
M25 = le.fit_transform(list(data['M25']))
AD1C = le.fit_transform(list(data['AD1-C']))
D15 = le.fit_transform(list(data['D15']))
D09 = le.fit_transform(list(data['D09']))
M07 = le.fit_transform(list(data['M07']))
M02 = le.fit_transform(list(data['M02']))
D10 = le.fit_transform(list(data['D10']))
I03 = le.fit_transform(list(data['I03']))
D07 = le.fit_transform(list(data['D07']))
M20 = le.fit_transform(list(data['M20']))
M11 = le.fit_transform(list(data['M11']))
D05 = le.fit_transform(list(data['D05']))
M05 = le.fit_transform(list(data['M05']))
M04 = le.fit_transform(list(data['M04']))
D03 = le.fit_transform(list(data['D03']))
M26 = le.fit_transform(list(data['M26']))
M01 = le.fit_transform(list(data['M01']))
M12 = le.fit_transform(list(data['M12']))
M03 = le.fit_transform(list(data['M03']))
M22 = le.fit_transform(list(data['M22']))
L09 = le.fit_transform(list(data['L09']))
D12 = le.fit_transform(list(data['D12']))
L10 = le.fit_transform(list(data['L10']))
L13 = le.fit_transform(list(data['L13']))
L12 = le.fit_transform(list(data['L12']))
L04 = le.fit_transform(list(data['L04']))

predict = 'action'
X = list(zip(M35, AD1A, M34, M32, M33, M36, M30, M29, M37, M38, M39, M41, M40, M31, M10, M14, M16, M15, M17, M51, M18, AD1B, M09, D08, M45,
             M47,M48,M28,M27,M46,M49,M23,M19,M42,M08,M43,M13,M50,D14,M21,L11,M06,L06,M24,M25,AD1C,D15,D09,M07,M02,D10,I03,D07,M20,M11,D05,
             M05,M04,D03,M26,M01,M12,M03,M22,L09,D12,L10,L13,L12,L04))
y = event

model = KNeighborsClassifier(n_neighbors=20)
x_train, x_test, y_train, y_test = train_test_split(X, y, test_size = 0.5)
model.fit(x_train, y_train)
predicted = model.predict(x_test)
acc_train_knn = model.score(x_train, y_train)
acc_test_knn = model.score(x_test, y_test)
mse_knn = mean_squared_error(predicted, y_test)
pre_knn = precision_score(predicted, y_test, average='weighted')
rec_knn = recall_score(predicted, y_test, average='weighted')
f1_knn = f1_score(predicted, y_test, average='weighted')
c = confusion_matrix(y_test, predicted)
#print(c)
print('KNN_MSE: ', mse_knn)
print('KNN_TRAIN_ACC: ',acc_train_knn)
print('KNN_TEST_ACC: ',acc_test_knn)
print('KNN_PRECISION: ', pre_knn)
print('KNN_RECALL: ', rec_knn)
print('KNN_F1_SCORE: ', f1_knn)

names = ['R1_Bed_to_Toilet','R1_Personal_Hygiene','R2_Bed_to_Toilet',
 'Meal_Preparation','R2_Personal_Hygiene','Watch_TV','R1_Work','R2_Work',
 'R1_Sleep','R2_Sleep','Wash_Bathtub','Clean' 'Study']

clf = RandomForestClassifier(max_depth=7, n_estimators=100, random_state=0)
x_train, x_test, y_train, y_test = train_test_split(X, y, test_size = 0.5)
clf.fit(x_train,y_train)

acc_train_rf = clf.score(x_train, y_train)
acc_test_rf = clf.score(x_test, y_test)
predicted = clf.predict(x_test)
mse_rf = mean_squared_error(predicted, y_test)
pre_rf = precision_score(predicted, y_test, average='weighted')
rec_rf = recall_score(predicted, y_test, average='weighted')
f1_rf = f1_score(predicted, y_test, average='weighted')
c = confusion_matrix(y_test, predicted)
#print(c)
print('RF_MSE: ', mse_rf)
print('RF_TRAIN_ACC: ', acc_train_rf)
print('RF_TEST_ACC: ', acc_test_rf)
print('RF_PRECISION: ', pre_rf)
print('RF_RECALL: ', rec_rf)
print('RF_F1_SCORE: ', f1_rf)

x_train, x_test, y_train, y_test = train_test_split(X, y, test_size = 0.5)
clf = LogisticRegression(penalty='l2', random_state=0)
clf.fit(x_train,y_train)
acc_train_lr = clf.score(x_train, y_train)
acc_test_lr = clf.score(x_test, y_test)
predicted = clf.predict(x_test)
mse_lr = mean_squared_error(predicted, y_test)
pre_lr = precision_score(predicted, y_test, average='weighted')
rec_lr = recall_score(predicted, y_test, average='weighted')
f1_lr = f1_score(predicted, y_test, average='weighted')
c = confusion_matrix(y_test, predicted)
#print(c)
print('LR_MSE: ', mse_lr)
print('LR_TRAIN_ACC: ', acc_train_lr)
print('LR_TEST_ACC: ', acc_test_lr)
print('LR_PRECISION: ', pre_lr)
print('LR_RECALL: ', rec_lr)
print('LR_F1_SCORE: ', f1_lr)

from sklearn.svm import SVC
x_train, x_test, y_train, y_test = train_test_split(X, y, test_size = 0.5)
clf = SVC(kernel='linear', probability=True, random_state=0)
clf.fit(x_train,y_train)
acc_train_svm = clf.score(x_train, y_train)
acc_test_svm = clf.score(x_test, y_test)
predicted = clf.predict(x_test)
mse_svm = mean_squared_error(predicted, y_test)
pre_svm = precision_score(predicted, y_test, average='weighted')
rec_svm = recall_score(predicted, y_test, average='weighted')
f1_svm = f1_score(predicted, y_test, average='weighted')
c = confusion_matrix(y_test, predicted)
#print(c)
print('SVM_MSE: ', mse_svm)
print('SVM_TRAIN_ACC: ', acc_train_svm)
print('SVM_TEST_ACC: ', acc_test_svm)
print('SVM_PRECISION: ', pre_svm)
print('SVM_RECALL: ', rec_svm)
print('SVM_F1_SCORE: ', f1_svm)
