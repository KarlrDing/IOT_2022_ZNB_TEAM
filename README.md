# elec0130
## Group ZNB
![image](https://github.com/KarlrDing/elec0130/blob/main/Readme_pics/ZNB.PNG)
***
**introduction**
: This is the source code of IoT.
It's mainly divided into 2 parts, **stage 1** and **stage 2**. 

Stage 1 is the edge system and cloud part.

Stage 2 is the data analyzing part.

---
Stage 1 is divided into different scenarios we assumed: **1. patient forget to take medicine, 2. patient falls down in stairs, 3. patient falls down in bathroom, 4. patient lost in neighbourhood.** 

The code in patient's roommate's board can be find in the folders which names contains"receive".

'testfiles' folder contains all the old versions of our code, we keep this just in case the new version doesn't works well. 

---
Stage 2 is divided into **data clean, abnormal activities detection, predection and classification.**

The dataset which is cleaned and extracted for analyzing can be find in data_clean&extract, 'modifiedV2' is the final version of the cleaned data, other files are used for different tasks. 

The codes for different part is placed in corresponding folders, the outcome figures and some evaluation functions can be found in the result part. 

---
**File format:**

Satge 1 files contain codes in Arduino board(.ino), headers(.h) and the flows in Node-Red(.json).

Stage 2 files contain codes in python(.py) and jupyter(.ipynb), data sheet(.csv) which contains the cleaned data and data used for analyzing, some figures like hotmap in .png format

<img src = "https://github.com/KarlrDing/elec0130/blob/main/Readme_pics/thumb%EF%BC%81.PNG"  width="400" height = "400"> <img src = "https://github.com/KarlrDing/elec0130/blob/main/Readme_pics/thank_you_sticky_note.jpg"  width="400" height = "400">
