import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import tensorflow as tf

###############
#Prep The Data#
###############

# Set random seed value
SEED = 1337
np.random.seed(SEED)
tf.random.set_seed(SEED)

# list of COLORS
COLORS = [
    "red",
    "orange",
    "yellow",
    "green",
    "blue",
    "purple"
]

SAMPLES_PER_COLOR = 50

NUM_COLORS = len(COLORS)

# One-hot encoded matrix, used in output
ONE_HOT_ENCODED_COLORS = np.eye(NUM_COLORS)

inputs = []
outputs = []

# read each csv file and push an input and output
for color_index in range(NUM_COLORS):
  color = COLORS[color_index]
  
  output = ONE_HOT_ENCODED_COLORS[color_index]
  
  df = pd.read_csv( color + ".csv")
  
  # Number of color recordings in the file
  num_recordings = int(df.shape[0] / SAMPLES_PER_COLOR)
  
  for i in range(num_recordings):
    tensor = []
    for j in range(SAMPLES_PER_COLOR):
      index = i * SAMPLES_PER_COLOR + j
      # normalize the input data, between 0 to 1:
      # RGB Value is between: 0 to 300
      tensor += [
          (df['R'][index]) / 300,
          (df['G'][index]) / 300,
          (df['B'][index]) / 300
      ]

    inputs.append(tensor)
    outputs.append(output)

# convert the list to numpy array
inputs = np.array(inputs)
outputs = np.array(outputs)

print("Data set parsed and preped.")

##############################
#Split and Randomize The Data#
##############################

# Randomize the order of the inputs, so they can be evenly distributed for training, testing, and validation
num_inputs = len(inputs)
randomize = np.arange(num_inputs)
np.random.shuffle(randomize)

# Swap the consecutive indexes (0, 1, 2, etc) with the randomized indexes
inputs = inputs[randomize]
outputs = outputs[randomize]

# Split the recordings (group of samples) into three sets: training, testing and validation
TRAIN_SPLIT = int(0.6 * num_inputs)
TEST_SPLIT = int(0.2 * num_inputs + TRAIN_SPLIT)

inputs_train, inputs_test, inputs_validate = np.split(inputs, [TRAIN_SPLIT, TEST_SPLIT])
outputs_train, outputs_test, outputs_validate = np.split(outputs, [TRAIN_SPLIT, TEST_SPLIT])

print("Data set randomized and split")

###########################
#Build and Train The Model#
###########################

# build the model and train it
model = tf.keras.Sequential()
model.add(tf.keras.layers.Dense(50, activation='relu')) # relu is used for performance
model.add(tf.keras.layers.Dense(15, activation='relu'))
model.add(tf.keras.layers.Dense(NUM_COLORS, activation='softmax')) # softmax is used, because we only expect one color to occur per input
model.compile(optimizer='rmsprop', loss='mse', metrics=['mae'])
history = model.fit(inputs_train, outputs_train, epochs=600, batch_size=1, validation_data=(inputs_validate, outputs_validate))
model.summary()

######################
#Graph The Loss & MAE#
######################

# increase the size of the graph
plt.rcParams["figure.figsize"] = (20,10)

# graph the loss
loss = history.history['loss']
val_loss = history.history['val_loss']
epochs = range(1, len(loss) + 1)
plt.plot(epochs, loss, 'g.', label='Training loss')
plt.plot(epochs, val_loss, 'b', label='Validation loss')
plt.title('Training and validation loss')
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.legend()
plt.show()

print(plt.rcParams["figure.figsize"])

# graph of mean absolute error
SKIP = 100
mae = history.history['mae']
val_mae = history.history['val_mae']
plt.plot(epochs[SKIP:], mae[SKIP:], 'g.', label='Training MAE')
plt.plot(epochs[SKIP:], val_mae[SKIP:], 'b.', label='Validation MAE')
plt.title('Training and validation mean absolute error')
plt.xlabel('Epochs')
plt.ylabel('MAE')
plt.legend()
plt.show()

###################################
#Convert Model to Tensor Flow Lite#
###################################

# Convert the model to the TensorFlow Lite format without quantization
converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

# Save the model to disk
open("color_model.tflite", "wb").write(tflite_model)
  
basic_model_size = os.path.getsize("color_model.tflite")
print("Model is %d bytes" % basic_model_size)

os.system("touch model.h")
os.system("echo 'const unsigned char model[] = {' > model.h")
os.system("cat color_model.tflite | xxd -i      >> model.h")
os.system("echo '};'                              >> model.h")