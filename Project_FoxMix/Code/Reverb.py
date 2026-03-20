import numpy as np
import tkinter as tk

# Parâmetros do Reverb para 2 faixas (Comb + All-Pass)
reverb_params = {
    'mix': [0.0, 0.0],
    'decay': [0.0, 0.0],
    'comb_delay': [10, 10],
    'comb_feedback': [0, 0],
    'apf_delay': [10, 10],
    'apf_feedback': [0, 0]
}


def all_pass_filter(data, delay_samples, feedback):
    output = np.zeros_like(data)
    buffer = np.zeros(int(delay_samples))

    for n in range(len(data)):
        delayed_sample = buffer[n % int(delay_samples)]
        buffer[n % int(delay_samples)] = data[n] + feedback * delayed_sample
        output[n] = -feedback * data[n] + delayed_sample

    return output


def comb_filter(input_signal, delay_samples, feedback):
    output_signal = np.zeros_like(input_signal)
    buffer = np.zeros(int(delay_samples))

    for n in range(len(input_signal)):
        delayed_sample = buffer[n % int(delay_samples)]
        output_signal[n] = input_signal[n] + feedback * delayed_sample
        buffer[n % int(delay_samples)] = input_signal[n]

    return output_signal



def apply_reverb(data, samplerate, track_index):
    # Retrieve reverb parameters for the given track
    decay = reverb_params['decay'][track_index]  # Decay control for reverberation
    mix = reverb_params['mix'][track_index]      # Mix control for reverb effect
    comb_delay = reverb_params['comb_delay'][track_index]
    comb_feedback = reverb_params['comb_feedback'][track_index]
    apf_delay = reverb_params['apf_delay'][track_index]
    apf_feedback = reverb_params['apf_feedback'][track_index]

    # Apply comb filter for initial reverb effect
    comb_signal = comb_filter(data, comb_delay, comb_feedback)

    # Apply all-pass filter to simulate a more complex reverb effect
    reverb_signal = all_pass_filter(comb_signal, apf_delay, apf_feedback)

    # Apply decay to reduce the intensity of the reverberation over time.
    # This will control how quickly the reverberation fades out.
    #decay_factor = np.exp(-decay * np.arange(len(reverb_signal)) / samplerate)
    #reverb_signal = reverb_signal * decay_factor

    # Mix the original signal with the reverberated signal without altering the original volume
    # We mix using the `mix` parameter, which controls how much of the reverb signal to add
    output_signal = (1 - mix) * data + mix * reverb_signal

    # Ensure the output signal is normalized
    #if np.max(np.abs(output_signal)) > 1:
        #output_signal /= np.max(np.abs(output_signal))

    return output_signal


def set_reverb_params(index, param_type, value):
    reverb_params[param_type][index] = float(value)


def reverb_controls_root():
    reverb_sliders = {}

    # Frame para o primeiro reverb (Track 1)
    frame1 = tk.LabelFrame(text="Reverb 1", padx=5, pady=5, borderwidth=1,
                           font=("Arial", 10, "bold"), fg="white", background="#1F303A")
    frame1.place(x=0, y=530, width=400, height=200)

    # Frame para o segundo reverb (Track 2)
    frame2 = tk.LabelFrame(text="Reverb 2", padx=5, pady=5, borderwidth=1,
                           font=("Arial", 10, "bold"), fg="white", background="#1F303A")
    frame2.place(x=1200, y=530, width=400, height=200)  # Ajuste a posição para o segundo conjunto de controles

    slider_positions = [20, 80, 140, 200, 260, 320]
    slider_labels = ["Mix", "Decay", "Comb Delay", "Comb Feedback", "APF Delay", "APF Feedback"]
    slider_params = ["mix", "decay", "comb_delay", "comb_feedback", "apf_delay", "apf_feedback"]
    min_values = [0.0, 0.0, 10, 0.0, 10, -0.7]
    max_values = [1.0, 1.0, 300, 1.0, 100, 0.7]
    resolutions = [0.01, 0.01, 1, 0.01, 1, 0.01]

    # Loop para os sliders do primeiro reverb (Track 1)
    for i in range(len(slider_positions)):
        label = tk.Label(frame1, text=slider_labels[i], font=("Arial", 10, "bold"), fg="white", background="#1F303A")
        label.place(x=slider_positions[i], y=5)

        reverb_sliders[slider_params[i]] = tk.Scale(frame1, from_=min_values[i], to=max_values[i],
                                                    resolution=resolutions[i],
                                                    orient="vertical", length=150,
                                                    command=lambda v, param=slider_params[i]: set_reverb_params(0,
                                                                                                                param,
                                                                                                                v),
                                                    font=("Arial", 10, "bold"), fg="white", borderwidth=2,
                                                    background="black",
                                                    activebackground="black")
        reverb_sliders[slider_params[i]].set(reverb_params[slider_params[i]][0])
        reverb_sliders[slider_params[i]].place(x=slider_positions[i], y=30)

    # Loop para os sliders do segundo reverb (Track 2)
    for i in range(len(slider_positions)):
        label = tk.Label(frame2, text=slider_labels[i], font=("Arial", 10, "bold"), fg="white", background="#1F303A")
        label.place(x=slider_positions[i], y=5)

        reverb_sliders[slider_params[i] + "_2"] = tk.Scale(frame2, from_=min_values[i], to=max_values[i],
                                                           resolution=resolutions[i],
                                                           orient="vertical", length=150,
                                                           command=lambda v, param=slider_params[i]: set_reverb_params(
                                                               1, param, v),
                                                           font=("Arial", 10, "bold"), fg="white", borderwidth=2,
                                                           background="black",
                                                           activebackground="black")
        reverb_sliders[slider_params[i] + "_2"].set(reverb_params[slider_params[i]][1])
        reverb_sliders[slider_params[i] + "_2"].place(x=slider_positions[i], y=30)

    return reverb_sliders