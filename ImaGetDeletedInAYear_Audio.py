import os
import random
import keyboard
import soundfile as sf
import sounddevice as sd

# Set up VB-Audio Virtual Cable as the output device
devices = sd.query_devices()
for i, device in enumerate(devices):
    if device['name'] == 'CABLE Output (VB-Audio Virtual Cable)':
        output_device_id = i
        break

# Load the MP3 files
mp3_dir = 'C:\\Users\\Captail\\Desktop\\random\\output'
mp3_files = [os.path.join(mp3_dir, f) for f in os.listdir(mp3_dir) if f.endswith('.mp3')]

# Define a function to play a random MP3 file
def play_random_mp3():
    mp3_file = random.choice(mp3_files)
    data, fs = sf.read(mp3_file, dtype='float32')
    sd.play(data, fs, device=output_device_id)
    sd.wait()

# Loop until the user presses F3 to stop
running = True
while running:
    if keyboard.is_pressed('f3'):
        running = False
    else:
        play_random_mp3()
