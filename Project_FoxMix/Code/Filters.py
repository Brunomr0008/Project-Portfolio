import scipy.signal as signal
import threading
import tkinter as tk
import DiscAnimation

filters_enabled = {'low': [False, False], 'mid': [False, False], 'high': [False, False]}
global_gains = {'low': [1.0, 1.0], 'mid': [1.0, 1.0], 'high': [1.0, 1.0]}

scale = {0: [300, 3000], 1: [300, 3000]}
current_filters = [None, None]
filter_update_timer = [None, None]

# Função para criar filtros
def create_filters(samplerate, index):
    # Filtro passa-baixa para graves (< 300 Hz)
    list_low = signal.butter(4, int(scale[index][0]), btype='lowpass', fs=samplerate)
    low_b = list_low[0]
    low_a = list_low[1]

    # Filtro passa-banda para médios (300 Hz - 3000 Hz)
    list_mid = signal.butter(4, [int(scale[index][0]), int(scale[index][1])], btype='bandpass', fs=samplerate)
    mid_b = list_mid[0]
    mid_a = list_mid[1]

    # Filtro passa-alta para agudos (> 3000 Hz)
    list_high = signal.butter(4, int(scale[index][1]), btype='highpass', fs=samplerate)
    high_b = list_high[0]
    high_a = list_high[1]

    return {'low': (low_b, low_a), 'mid': (mid_b, mid_a), 'high': (high_b, high_a)}

def on_scale_change(filter_type, index, value):
    scale[index][filter_type] = value

# Função para aplicar filtros
def apply_filters(data, filters, index):
    if filters_enabled['low'][index]:
        data = signal.filtfilt(filters['low'][0], filters['low'][1], data)
        data *= global_gains['low'][index]  # Aplica o ganho global para graves
    if filters_enabled['mid'][index]:
        data = signal.filtfilt(filters['mid'][0], filters['mid'][1], data)
        data *= global_gains['mid'][index]  # Aplica o ganho global para médios
    if filters_enabled['high'][index]:
        data = signal.filtfilt(filters['high'][0], filters['high'][1], data)
        data *= global_gains['high'][index]  # Aplica o ganho global para agudos

    return data

# Ajustar ganho global
def adjust_global_gain(filter_type, index, value):
    global_gains[filter_type][index] = float(value)

# Função para ativar/desativar filtros
def toggle_filter(index, filter_type):
    filters_enabled[filter_type][index] = not filters_enabled[filter_type][index]

# Função para atualizar os filtros periodicamente
def update_filters(samplerate, index):
    global current_filters, filter_update_timer
    current_filters = create_filters(samplerate, index)
    # Use daemon=True to prevent thread blocking program exit
    filter_update_timer[index] = threading.Timer(1, update_filters, args=[samplerate, index])
    filter_update_timer[index].daemon = True
    filter_update_timer[index].start()

def stop_filter_update(index):
    global filter_update_timer
    if filter_update_timer[index] is not None:
        filter_update_timer[index].cancel() # Cancel the Timer
        filter_update_timer[index] = None


# noinspection PyUnresolvedReferences
def filters_root():
    filter_buttons = {'low': [None, None], 'mid': [None, None], 'high': [None, None]}
    buttons_scale = {'low': [None, None], 'high': [None, None]}
    gain_sliders = {'low': [None, None], 'mid': [None, None], 'high': [None, None]}

    # Botões de filtros para Música 1
    button_frame1 = tk.LabelFrame(DiscAnimation.root, text="Filters 1", padx=5, pady=5, borderwidth=1,
                                  font=("Arial", 10, "bold"), fg="white", background="#1F303A")
    button_frame1.place(x=340, y=230, width=280, height=320)  # Added width and height for frame

    filter_buttons['low'][0] = tk.Checkbutton(button_frame1, text="Low",
                                              command=lambda: toggle_filter(0, 'low'),
                                              font=("Arial", 10, "bold"), fg="purple", borderwidth=2, background="#1F303A",
                                              activebackground="#1F303A")
    filter_buttons['low'][0].place(x=45, y=5)
    filter_buttons['mid'][0] = tk.Checkbutton(button_frame1, text="Mid",
                                              command=lambda: toggle_filter(0, 'mid'),
                                              font=("Arial", 10, "bold"), fg="green", borderwidth=2, background="#1F303A",
                                              activebackground="#1F303A")
    filter_buttons['mid'][0].place(x=105, y=5)
    filter_buttons['high'][0] = tk.Checkbutton(button_frame1, text="High",
                                               command=lambda: toggle_filter(0, 'high'),
                                               font=("Arial", 10, "bold"), fg="red", borderwidth=2, background="#1F303A",
                                              activebackground="#1F303A")
    filter_buttons['high'][0].place(x=165, y=5)

    buttons_scale['low'][0] = tk.Scale(button_frame1, from_=100, to=1500, orient="horizontal", length=200,
                                       tickinterval=280,label="                     Low", command=lambda s:
                                       on_scale_change(0, 0, s), font=("Arial", 10, "bold"),
                                       fg="white", borderwidth=2, background="black")
    buttons_scale['low'][0].set(300)
    buttons_scale['low'][0].place(x=30, y=40)

    buttons_scale['high'][0] = tk.Scale(button_frame1, from_=1600, to=4000, orient="horizontal", length=200,
                                        tickinterval=480, label="                    High", command=lambda s:
                                        on_scale_change(1, 0, s), font=("Arial", 10, "bold"), fg="white",
                                        borderwidth=2, background="black")

    buttons_scale['high'][0].set(3000)
    buttons_scale['high'][0].place(x=30, y=130)

    # Criar Sliders para os controlos de ganho
    gain_sliders['low'][0] = tk.Scale(button_frame1, from_=0.01, to=5.0, resolution=0.01, length=80,
                                      orient="horizontal", label="Low Gain", command=lambda v:
                                      adjust_global_gain('low', 0, v), font=("Arial", 10, "bold"),
                                      fg="white", borderwidth=2, background="black")
    gain_sliders['low'][0].set(1.0)
    gain_sliders['low'][0].place(x=0, y=220)

    gain_sliders['mid'][0] = tk.Scale(button_frame1, from_=0.01, to=5.0, resolution=0.01, length=80,
                                      orient="horizontal", label="Mid Gain", command=lambda v:
                                      adjust_global_gain('mid', 0, v), font=("Arial", 10, "bold"),
                                      fg="white", borderwidth=2, background="black")
    gain_sliders['mid'][0].set(1.0)
    gain_sliders['mid'][0].place(x=90, y=220)

    gain_sliders['high'][0] = tk.Scale(button_frame1, from_=0.01, to=5.0, resolution=0.01, length=80,
                                       orient="horizontal", label="High Gain", command=lambda v:
                                       adjust_global_gain('high', 0, v), font=("Arial", 10, "bold"),
                                      fg="white", borderwidth=2, background="black")
    gain_sliders['high'][0].set(1.0)
    gain_sliders['high'][0].place(x=180, y=220)

    # Botões de filtros para Música 2
    button_frame2 = tk.LabelFrame(DiscAnimation.root, text="Filters 2", padx=5, pady=5,
                                  borderwidth=1, font=("Arial", 10, "bold"), fg="white", background="#1F303A")
    button_frame2.place(x=990, y=230, width=280, height=320) # Added width and height for frame

    filter_buttons['low'][1] = tk.Checkbutton(button_frame2, text="Low",command=lambda: toggle_filter(1, 'low'),
                                              font=("Arial", 10, "bold"), fg="purple", borderwidth=2, background="#1F303A",
                                              activebackground="#1F303A")
    filter_buttons['low'][1].place(x=45, y=5)
    filter_buttons['mid'][1] = tk.Checkbutton(button_frame2, text="Mid", command=lambda: toggle_filter(1, 'mid'),
                                              font=("Arial", 10, "bold"), fg="green", borderwidth=2, background="#1F303A",
                                              activebackground="#1F303A")
    filter_buttons['mid'][1].place(x=105, y=5)
    filter_buttons['high'][1] = tk.Checkbutton(button_frame2, text="High", command=lambda: toggle_filter(1, 'high'),
                                               font=("Arial", 10, "bold"), fg="red", borderwidth=2, background="#1F303A",
                                               activebackground="#1F303A")
    filter_buttons['high'][1].place(x=165, y=5)

    buttons_scale['low'][1] = tk.Scale(button_frame2, from_=100, to=1500, orient="horizontal", length=200,
                                       tickinterval=280, label="                     Low", command=lambda s:
                                       on_scale_change(0, 1, s), font=("Arial", 10, "bold"),
                                       fg="white", borderwidth=2, background="black")
    buttons_scale['low'][1].set(300)
    buttons_scale['low'][1].place(x=30, y=40)

    buttons_scale['high'][1] = tk.Scale(button_frame2, from_=1600, to=4000, orient="horizontal", length=200,
                                        tickinterval=480, label="                    High", command=lambda s:
                                        on_scale_change(1, 1, s), font=("Arial", 10, "bold"),
                                        fg="white", borderwidth=2, background="black")

    buttons_scale['high'][1].set(3000)
    buttons_scale['high'][1].place(x=30, y=130)

    # Criar Sliders para os controlos de ganho
    gain_sliders['low'][1] = tk.Scale(button_frame2, from_=0.01, to=5.0, resolution=0.01, length=80,
                                      orient="horizontal", label="Low Gain", command=lambda v:
                                      adjust_global_gain('low', 1, v), font=("Arial", 10, "bold"),
                                      fg="white", borderwidth=2, background="black")
    gain_sliders['low'][1].set(1.0)
    gain_sliders['low'][1].place(x=0, y=220)

    gain_sliders['mid'][1] = tk.Scale(button_frame2, from_=0.01, to=5.0, resolution=0.01, length=80,
                                      orient="horizontal", label="Mid Gain", command=lambda v:
                                      adjust_global_gain('mid', 1, v), font=("Arial", 10, "bold"),
                                      fg="white", borderwidth=2, background="black")
    gain_sliders['mid'][1].set(1.0)
    gain_sliders['mid'][1].place(x=90, y=220)

    gain_sliders['high'][1] = tk.Scale(button_frame2, from_=0.01, to=5.0, resolution=0.01, length=80,
                                       orient="horizontal", label="High Gain", command=lambda v:
                                       adjust_global_gain('high', 1, v), font=("Arial", 10, "bold"),
                                      fg="white", borderwidth=2, background="black")
    gain_sliders['high'][1].set(1.0)
    gain_sliders['high'][1].place(x=180, y=220)