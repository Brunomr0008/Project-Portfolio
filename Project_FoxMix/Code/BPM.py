import librosa
import tkinter as tk
import MainCode, DiscAnimation, Config_select, Graph

# Add these at the top of your file with other global variables
bpm_values = [120.0, 120.0]  # Default BPM for both tracks
sync_enabled = False
master_track = 0  # 0 or 1, indicates which track others should sync to

# BPM sliders and labels
bpm_slider = [None, None]
bpm_labels = [None, None]
master_buttons = [None, None]
speed_slider = [None, None]
sync_button = None
speeds = [1.0, 1.0]  # New: Speed multipliers for each track


# Function to detect BPM from audio file
def detect_bpm(filename):
    y, sr = librosa.load(filename)
    tempo, _ = librosa.beat.beat_track(y=y, sr=sr)
    # Extract single value from numpy array and convert to float
    return float(tempo.item())

# Function to update BPM display
def update_bpm_display(index):
    bpm_labels[index].config(text=f"BPM: {bpm_values[index]:.1f}")

# Function to manually adjust BPM
def adjust_bpm(index, value):
    bpm_values[index] = float(value)
    update_bpm_display(index)
    if sync_enabled and index == master_track:
        # Update slave track's speed to match master BPM
        slave_index = 1 - master_track
        speed_ratio = bpm_values[master_track] / bpm_values[slave_index]
        speeds[slave_index] = speed_ratio
        speed_slider[slave_index].set(speed_ratio * 100)

        # If the stream is active, we need to restart it to apply the new speed
        if Config_select.streams[slave_index]:
            Config_select.stop_file(slave_index)
            MainCode.play_file(slave_index)

# Function to toggle BPM sync
def toggle_sync():
    global sync_enabled
    sync_enabled = not sync_enabled
    sync_button.config(relief=tk.SUNKEN if sync_enabled else tk.RAISED)
    if sync_enabled:
        # Immediately sync speeds based on master track
        speed_ratio = bpm_values[master_track] / bpm_values[1 - master_track]
        speeds[1 - master_track] = speed_ratio
        speed_slider[1 - master_track].set(speed_ratio * 100)

# Function to set master track
def set_master_track(index):
    global master_track
    master_track = index
    master_buttons[0].config(relief=tk.SUNKEN if index == 0 else tk.RAISED)
    master_buttons[1].config(relief=tk.SUNKEN if index == 1 else tk.RAISED)
    if sync_enabled:
        # Update sync when master changes
        slave_index = 1 - master_track
        speed_ratio = bpm_values[master_track] / bpm_values[slave_index]

        # Update BPM value and slider for the slave track
        bpm_values[slave_index] = bpm_values[master_track]
        bpm_slider[slave_index].set(bpm_values[master_track])
        update_bpm_display(slave_index)

        # Adjust speed accordingly
        speeds[slave_index] = speed_ratio
        speed_slider[slave_index].set(speed_ratio * 100)

def adjust_speed(index, value):
    speeds[index] = float(value) / 100.0

def auto_sync_bpm():
    global sync_enabled, master_track

    if Graph.n_musica < 2:
        return  # Need both tracks playing

    # Calculate the current average BPM
    avg_bpm = sum(bpm_values) / 2

    # Set master track to the current track with the higher BPM
    master_track = 0 if bpm_values[0] > bpm_values[1] else 1

    # Set all tracks to the same average BPM
    for i in range(2):
        bpm_values[i] = avg_bpm
        bpm_slider[i].set(avg_bpm)
        update_bpm_display(i)

    # Ensure sync is enabled
    sync_enabled = True
    sync_button.config(relief=tk.SUNKEN)


def sync_beats(filename1, filename2):
    # Load both audio files
    y1, sr1 = librosa.load(filename1)
    y2, sr2 = librosa.load(filename2)

    # Detect beat frames for both tracks
    beat_frames1 = librosa.beat.beat_track(y=y1, sr=sr1)[1]
    beat_frames2 = librosa.beat.beat_track(y=y2, sr=sr2)[1]

    # Convert beat frames to times
    beat_times1 = librosa.frames_to_time(beat_frames1, sr=sr1)
    beat_times2 = librosa.frames_to_time(beat_frames2, sr=sr2)

    # Find the first beat times for alignment
    first_beat1 = beat_times1[0]
    first_beat2 = beat_times2[0]

    # Calculate time offset to align first beats
    time_offset = abs(first_beat1 - first_beat2)

    return time_offset


def beat_sync_tracks():
    if Config_select.filenames[0] and Config_select.filenames[1]:
        try:
            # Calculate beat synchronization offset
            beat_offset = sync_beats(Config_select.filenames[0], Config_select.filenames[1])

            # Optionally adjust playback or speeds to synchronize
            # This is a simplified approach and might need more sophisticated implementation
            if beat_offset > 0:
                # Adjust speeds or introduce a delay
                speeds[1] = bpm_values[0] / bpm_values[1]
                speed_slider[1].set(speeds[1] * 100)

            # Enable sync
            global sync_enabled
            sync_enabled = True
            sync_button.config(relief=tk.SUNKEN)

            print(f"Beat synchronization offset: {beat_offset} seconds")
        except Exception as e:
            print(f"Beat sync error: {e}")

def bpm_root():
    global sync_button
    # Criar Sliders para os controlos de velocidade
    speed_slider[0] = tk.Scale(DiscAnimation.root, from_=50, to=150, orient="horizontal", length=140,
                               label="     Speed Music 1", command=lambda v: adjust_speed(0, v),
                               font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    speed_slider[0].set(100)  # 100 = normal speed
    speed_slider[0].place(x=235, y=130)  # Adjust position as needed


    speed_slider[1] = tk.Scale(DiscAnimation.root, from_=50, to=150, orient="horizontal", length=140,
                               label="    Speed Music 2", command=lambda v: adjust_speed(1, v),
                               font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    speed_slider[1].set(100)  # 100 = normal speed
    speed_slider[1].place(x=1215, y=130)  # Adjust position as needed

    # BPM control section
    bpm_frame = tk.LabelFrame(DiscAnimation.root, text="Controlo de BPMs", padx=5, pady=5,
                              borderwidth=2, highlightthickness=0, font=("Arial", 10, "bold"), fg="black",
                              background="#7F7F7F")
    bpm_frame.place(x=650, y=570, width=300, height=150)  # Added width and height for frame

    # Track 1 BPM controls
    bpm_labels[0] = tk.Label(bpm_frame, text="BPM: 120.0", font=("Arial", 10, "bold"), fg="black", borderwidth=2,
                             background="#7F7F7F")
    bpm_labels[0].place(x=20, y=5)

    bpm_slider[0] = tk.Scale(bpm_frame, from_=60, to=200, orient="horizontal", command=lambda v: adjust_bpm(0, v),
                             font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    bpm_slider[0].set(120)
    bpm_slider[0].place(x=0, y=30, width=100)

    master_buttons[0] = tk.Button(bpm_frame, text="Master 1", command=lambda: set_master_track(0),
                                  font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    master_buttons[0].place(x=20, y=90)

    # Track 2 BPM controls
    bpm_labels[1] = tk.Label(bpm_frame, text="BPM: 120.0", font=("Arial", 10, "bold"), fg="black", borderwidth=2,
                             background="#7F7F7F")
    bpm_labels[1].place(x=205, y=5)

    bpm_slider[1] = tk.Scale(bpm_frame, from_=60, to=200, orient="horizontal", command=lambda v: adjust_bpm(1, v),
                             font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    bpm_slider[1].set(120)
    bpm_slider[1].place(x=185, y=30, width=100)

    master_buttons[1] = tk.Button(bpm_frame, text="Master 2", command=lambda: set_master_track(1),
                                  font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    master_buttons[1].place(x=205, y=90)

    # Sync toggle buttons
    sync_button = tk.Button(bpm_frame, text="Sync BPM", command=toggle_sync, font=("Arial", 10, "bold"),
                            fg="white", borderwidth=2, background="black")
    sync_button.place(x=105, y=0)

    auto_sync_button = tk.Button(bpm_frame, text="Auto Sync", command=auto_sync_bpm, font=("Arial", 10, "bold"),
                                 fg="white", borderwidth=2, background="black")
    auto_sync_button.place(x=105, y=45)

    beat_sync_button = tk.Button(bpm_frame, text="Sync Beats", command=beat_sync_tracks, font=("Arial", 10, "bold"),
                                 fg="white", borderwidth=2, background="black")
    beat_sync_button.place(x=100, y=90)