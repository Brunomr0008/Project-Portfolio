import numpy as np
import tkinter as tk
import DiscAnimation

# Dictionary to store vibrato parameters for two tracks
vibrato_params = {'rate': [0.0, 0.0], 'depth': [0.0, 0.0]}


def apply_vibrato(data, samplerate, track_index):
    vibrato_rate = vibrato_params['rate'][track_index]
    vibrato_depth = vibrato_params['depth'][track_index]

    if vibrato_rate == 0 or vibrato_depth == 0:
        return data  # Retorna o som original se vibrato estiver desligado.

    # Criar vetor de tempo
    t = np.arange(len(data)) / samplerate

    # Gerar sinal de modulação do vibrato
    vibrato_modulation = vibrato_depth * np.sin(2 * np.pi * vibrato_rate * t)

    # Aplicar deslocamento no tempo
    new_time = t + vibrato_modulation
    new_time = np.clip(new_time, 0, len(data) / samplerate)

    # Aplicar interpolação para transição suave
    vibrato_signal = np.interp(new_time, t, data)

    return vibrato_signal

def set_vibrato_params(index, param_type, value):
    vibrato_params[param_type][index] = float(value)
    #print(f"Updated vibrato {param_type} for track {index}: {vibrato_params[param_type][index]}")


def vibrato_controls_root():
    vibrato_sliders = {'rate': [None, None], 'depth': [None, None]}

    # Vibrato controls for Track 1
    button_frame1 = tk.LabelFrame(DiscAnimation.root, text="Vibrato 1", padx=5, pady=5, borderwidth=1,
                                  font=("Arial", 10, "bold"), fg="white", background="#1F303A")
    button_frame1.place(x=400, y=560, width=220, height=175)

    # Vibrato Frequency Slider for Track 1
    vibrato_sliders['rate'][0] = tk.Scale(button_frame1, from_=0.0, to=8, resolution=0.1, orient="horizontal",
                                          length=200, label="Vibrato Frequency",
                                          command=lambda v: set_vibrato_params(0, 'rate', v),
                                          font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black",
                                          activebackground="black")
    vibrato_sliders['rate'][0].set(0)
    vibrato_sliders['rate'][0].place(x=0, y=10)

    # Vibrato Depth Slider
    vibrato_sliders['depth'][0] = tk.Scale(button_frame1, from_=0.000, to=0.02, resolution=0.0001, orient="horizontal",
                                           length=200, label="Vibrato Depth",
                                           command=lambda v: set_vibrato_params(0, 'depth', v),
                                           font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black",
                                           activebackground="black")
    vibrato_sliders['depth'][0].set(0.000)
    vibrato_sliders['depth'][0].place(x=0, y=80)

    # Vibrato controls for Track 2
    button_frame2 = tk.LabelFrame(DiscAnimation.root, text="Vibrato 2", padx=5, pady=5, borderwidth=1,
                                  font=("Arial", 10, "bold"), fg="white", background="#1F303A")
    button_frame2.place(x=1020, y=560, width=220, height=175)

    # Vibrato Frequency Slider for Track 2
    vibrato_sliders['rate'][1] = tk.Scale(button_frame2, from_=0.0, to=8, resolution=0.1, orient="horizontal",
                                          length=200, label="Vibrato Frequency",
                                          command=lambda v: set_vibrato_params(1, 'rate', v),
                                          font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black",
                                          activebackground="black")
    vibrato_sliders['rate'][1].set(0.0)
    vibrato_sliders['rate'][1].place(x=0, y=10)

    # Vibrato Depth Slider for Track 2
    vibrato_sliders['depth'][1] = tk.Scale(button_frame2, from_=0.000, to=0.02, resolution=0.0001, orient="horizontal",
                                           length=200, label="Vibrato Depth",
                                           command=lambda v: set_vibrato_params(1, 'depth', v),
                                           font=("Arial", 10, "bold"), fg="white", borderwidth=2, background="black",
                                           activebackground="black")
    vibrato_sliders['depth'][1].set(0.00)
    vibrato_sliders['depth'][1].place(x=0, y=80)

