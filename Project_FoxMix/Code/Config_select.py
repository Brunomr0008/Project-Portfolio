import tkinter as tk
from tkinter import filedialog
import BPM, Graph, DiscAnimation, MainCode

buttons_select = [None, None]
buttons_play = [None, None]
buttons_stop = [None, None]
buttons_label = [None, None]
button_restart = [None, None]

# Configurações
streams = [None, None]
filenames = [None, None]
volumes = [1.0, 1.0]
enabled_restart = [False, False]


# Selecionar música
def select_file(index):
    filename = filedialog.askopenfilename(filetypes=[("Audio files", "*.wav;*.flac;*.ogg;*.mp3")])
    if filename:
        filenames[index] = filename
        buttons_label[index].config(text=f"Music {index + 1} Selected")
        buttons_select[index].config(state=tk.DISABLED)
        buttons_play[index].config(state=tk.NORMAL)

        # Detect and set BPM
        #try:
           # detected_bpm = BPM.detect_bpm(filename)
           # BPM.bpm_values[index] = detected_bpm
            #BPM.bpm_slider[index].set(detected_bpm)
           # BPM.update_bpm_display(index)
       # except Exception as e:
           # print(f"Error detecting BPM: {e}")
            # Set default BPM if detection fails
           # BPM.bpm_values[index] = 120.0
           # BPM.bpm_slider[index].set(120.0)
           # BPM.update_bpm_display(index)


# Parar música
def stop_file(index):
    Graph.n_musica[index] = 0

    buttons_label[index].config(text=f"Music {index + 1} Stoped")
    buttons_select[index].config(state=tk.NORMAL)
    buttons_play[index].config(state=tk.NORMAL)
    buttons_stop[index].config(state=tk.DISABLED)

    DiscAnimation.stop_rotation(index)
    Graph.audio_queue.queue.clear()  # Limpar a fila

    if Graph.n_musica[0] == 0 and Graph.n_musica[1] == 0:
        Graph.graph_resume = False

    if streams[index]:
        streams[index].stop()
        streams[index].close()
        streams[index] = None


# Ajustar volume
def adjust_volume(index, value):
    volumes[index] = float(value) / 100.0

def restart_auto(index):
    global enabled_restart
    enabled_restart[index] = not enabled_restart[index]
    button_restart[index].config(relief=tk.SUNKEN if enabled_restart[index] else tk.RAISED)

def config_root():
    volume_slider = [None, None]
    
    # Controls Music 1
    config_frame1 = tk.LabelFrame(DiscAnimation.root, padx=5, pady=5, borderwidth=1, background="#1F303A")
    config_frame1.place(x=105, y=25, width=120, height=190)  # Added width and height for frame

    buttons_label[0] = tk.Label(config_frame1, text="Music 1", fg="white", font=("Arial", 10, "bold"),
                                borderwidth=0, background="#1F303A")
    buttons_label[0].grid()

    buttons_select[0] = tk.Button(config_frame1, text="Select Music 1", command=lambda: select_file(0),
                                  font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    buttons_select[0].place(x=3, y=30)
    buttons_play[0] = tk.Button(config_frame1, text="Play Music 1", command=lambda: MainCode.play_file(0),
                                state=tk.DISABLED, font=("Arial", 10, "bold"), fg="white", borderwidth=2,
                                background="black")
    buttons_play[0].place(x=8, y=70)
    buttons_stop[0] = tk.Button(config_frame1, text="Stop Music 1", command=lambda: stop_file(0),
                                state=tk.DISABLED, font=("Arial", 10, "bold"), fg="white", borderwidth=2,
                                background="black")
    buttons_stop[0].place(x=7, y=110)

    button_restart[0] = tk.Button(config_frame1, text="Restart Auto 1", command=lambda: restart_auto(0),
                                  font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    button_restart[0].place(x=3, y=150)

    volume_slider[0] = tk.Scale(DiscAnimation.root, from_=0, to=100, orient="horizontal", label="  Music Volume 1",
                                command=lambda v: adjust_volume(0, v), length=120,
                                font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    volume_slider[0].set(50)
    volume_slider[0].place(x=245, y=60)

    # Controls Music 2
    config_frame2 = tk.LabelFrame(DiscAnimation.root, padx=5, pady=5, borderwidth=1, background="#1F303A")
    config_frame2.place(x=1375, y=25, width=120, height=190)  # Added width and height for frame

    buttons_label[1] = tk.Label(config_frame2, text="Music 2", fg="white", font=("Arial", 10, "bold"),
                                borderwidth=0, background="#1F303A")
    buttons_label[1].grid()

    buttons_select[1] = tk.Button(config_frame2, text="Select Music 2", command=lambda: select_file(1),
                                  font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    buttons_select[1].place(x=3, y=30)
    buttons_play[1] = tk.Button(config_frame2, text="Play Music 2", command=lambda: MainCode.play_file(1),
                                state=tk.DISABLED, font=("Arial", 10, "bold"), fg="white", borderwidth=2,
                                background="black")
    buttons_play[1].place(x=8, y=70)
    buttons_stop[1] = tk.Button(config_frame2, text="Stop Music 2", command=lambda: stop_file(1),
                                state=tk.DISABLED, font=("Arial", 10, "bold"), fg="white", borderwidth=2,
                                background="black")
    buttons_stop[1].place(x=7, y=110)

    button_restart[1] = tk.Button(config_frame2, text="Restart Auto 2", command=lambda: restart_auto(1),
                                  font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black")
    button_restart[1].place(x=3, y=150)

    volume_slider[1] = tk.Scale(DiscAnimation.root, from_=0, to=100, length=120, orient="horizontal", label="  Music Volume 2",
                                command=lambda v: adjust_volume(1, v), font=("Arial", 10, "bold"), fg="white",
                                borderwidth=2, background="black")
    volume_slider[1].set(50)
    volume_slider[1].place(x=1225, y=60)