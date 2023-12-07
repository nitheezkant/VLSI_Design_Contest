#!/usr/bin/env python
# coding: utf-8


import numpy as np
import pandas as pd
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import r2_score
import matplotlib.pyplot as plt
import matplotlib.pyplot as plt

def create_windowed_rows(data, window_size):
    windowed_data = [data[i:i+window_size] for i in range(0, len(data),1)]
    return windowed_data

input_file_path = 'Desktop/nitheez/in.csv'
output_file_path = 'Desktop/nitheez/out.csv'

data = pd.read_csv(input_file_path)

column_name = 'Value'

column_data = data['b'].tolist()

window_size = 100
windowed_data = create_windowed_rows(column_data, window_size)

new_data = pd.DataFrame(windowed_data)

new_data.to_csv(output_file_path, index=False, header=False)

train_data = pd.read_csv('Desktop/nitheez/out.csv', header=None) 

test_data = pd.read_csv('Desktop/nitheez/out.csv', header=None)  

X_train = train_data.iloc[:35000, :-1].values
y_train = train_data.iloc[:35000, -1:].values
print(y_train)
X_test = test_data.iloc[35000:, :-1].values
y_test = test_data.iloc[35000:,-1:].values
print(y_test)
# Standardize the data
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)


X_train = X_train.reshape(X_train.shape[0], X_train.shape[1], 1)

X_test = X_test.reshape(X_test.shape[0], X_test.shape[1], 1)

model = keras.Sequential([
    layers.Input(shape=(X_train.shape[1], 1)),  # Input shape with number of features
    layers.Conv1D(64,3,activation='relu'), # 1D Convolutional layer
    layers.MaxPooling1D(pool_size=2),  # Max-pooling layer
    layers.Flatten(),  # Flatten to connect to fully connected layers
    layers.Dense(64, activation='relu'), 
    layers.Dense(64, activation='relu'),# Hidden layer with 64 units and ReLU activation
   layers.Dense(32, activation='relu'), # Hidden layer with 32 units and ReLU activation
    layers.Dense(1)  # Output layer with 5 units (for the 5 labels)
])

model.compile(optimizer='adam', loss='mean_squared_error', metrics=['mae'])

model.fit(X_train, y_train, epochs=100,batch_size=64,validation_split=0.2)

y_pred = model.predict(X_test)

r2_scores = []
for i in range(1):
    r2 = r2_score(y_test[:, i], y_pred[:, i])
    r2_scores.append(r2)
    print(f'R2 Score for Label {i + 1}: {r2:.4f}')

average_r2 = np.mean(r2_scores)
print(f'Average R2 Score: {average_r2:.4f}')



model.save('Desktop/model.h5')


label_index = 0

plt.figure(figsize=(8, 6))
plt.scatter(y_test[:, label_index], y_pred[:, label_index], s=20)
plt.plot([y_test[:, label_index].min(), y_test[:, label_index].max()], [y_test[:, label_index].min(), y_test[:, label_index].max()], color='red', linestyle='--', linewidth=2, label='Ideal Prediction')
plt.xlabel(f'Actual Label {label_index + 1}')
plt.ylabel(f'Predicted Label {label_index + 1}')
plt.title(f'Actual vs. Predicted for Label {label_index + 1}')
plt.legend()
plt.show()

label_index = 0

plt.figure(figsize=(10, 6))
plt.plot(range(14000), y_test[:14000, label_index], label='Actual Angle at t+100Ts', marker='o',markersize=1)
plt.plot(range(14000), y_pred[:14000, label_index], label='Predicted angle at t+100Ts', marker='.',markersize=1)
plt.xlabel('Time')
plt.ylabel(f'Angle')
plt.title(f'Actual vs. Predicted Angle')
plt.legend()
plt.grid()
plt.show()







