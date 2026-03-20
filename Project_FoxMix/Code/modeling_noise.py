"""
import numpy as np
import tkinter as tk


# Variáveis globais para controlar as configurações do ruído
noise_enabled = [False, False]  # Ativa/desativa ruído por música
noise_volume = [0.1, 0.1]  # Volume do ruído
am_mod_freq = [1.0, 1.0]  # Frequência da modulação AM
am_depth = [0.5, 0.5]  # Profundidade da modulação AM
fm_mod_freq = [1.0, 1.0]  # Frequência da modulação FM
filter_cutoff = [1000, 1000]  # Frequência de corte do filtro
filter_type = ["lowpass", "lowpass"]  # Tipo de filtro ("lowpass" ou "highpass")

# Função para gerar ruído branco
def generate_white_noise(duration, fs):
    return np.random.randn(fs * duration)


# Função para gerar ruído rosa
def generate_pink_noise(duration, fs):
    pink_noise = np.random.randn(fs * duration) / np.sqrt(np.arange(1, fs * duration + 1))
    return pink_noise


# Função para modulação de amplitude (AM)
def apply_am_modulation(signal, fs, index):
    t = np.linspace(0, len(signal) / fs, len(signal), endpoint=False)
    mod_signal = 1 + am_depth[index] * np.sin(2 * np.pi * am_mod_freq[index] * t)
    return signal * mod_signal


# Função para modulação de frequência (FM) no ruído rosa
def apply_fm_modulation(signal, fs, index):
    t = np.linspace(0, len(signal) / fs, len(signal), endpoint=False)
    modulated_signal = np.sin(2 * np.pi * (fm_mod_freq[index] * t + signal))
    return modulated_signal


# Função para aplicar filtro dinâmico
def apply_dynamic_filter(signal, fs, index):
    from scipy.signal import butter, lfilter

    nyquist = fs / 2
    cutoff = filter_cutoff[index] / nyquist
    b, a = butter(4, cutoff, btype=filter_type[index])
    return lfilter(b, a, signal)


# Função para gerar ruído modulado com as configurações do usuário
def generate_modulated_noise(index, duration=5, fs=44100):
    if not noise_enabled[index]:
        return np.zeros(fs * duration)

    # Gerar ruído
    noise = generate_white_noise(duration, fs) if index == 0 else generate_pink_noise(duration, fs)

    # Aplicar AM (modulação de volume)
    noise = apply_am_modulation(noise, fs, index)

    # Aplicar FM (modulação de pitch no ruído rosa)
    if index == 1:
        noise = apply_fm_modulation(noise, fs, index)

    # Aplicar filtro dinâmico
    noise = apply_dynamic_filter(noise, fs, index)

    # Aplicar volume do ruído
    return noise * noise_volume[index]


# Funções de controle da interface gráfica
def toggle_noise(index):
    noise_enabled[index] = not noise_enabled[index]
    print(f"Ruído {'ativado' if noise_enabled[index] else 'desativado'} para Música {index + 1}")


def adjust_noise_volume(index, value):
    noise_volume[index] = float(value) / 100.0
    print(f"Volume do Ruído {index + 1}: {noise_volume[index]}")


def adjust_am_mod_freq(index, value):
    am_mod_freq[index] = float(value)
    print(f"Frequência de Modulação AM Música {index + 1}: {am_mod_freq[index]} Hz")


def adjust_am_depth(index, value):
    am_depth[index] = float(value) / 100.0
    print(f"Profundidade de Modulação AM Música {index + 1}: {am_depth[index]}")


def adjust_fm_mod_freq(index, value):
    fm_mod_freq[index] = float(value)
    print(f"Frequência de Modulação FM Música {index + 1}: {fm_mod_freq[index]} Hz")


def adjust_filter_cutoff(index, value):
    filter_cutoff[index] = float(value)
    print(f"Frequência de Corte do Filtro Música {index + 1}: {filter_cutoff[index]} Hz")


def toggle_filter_type(index):
    filter_type[index] = "highpass" if filter_type[index] == "lowpass" else "lowpass"
    print(f"Filtro da Música {index + 1} alterado para {filter_type[index]}")


# Interface gráfica para ajustar os parâmetros do ruído
# Interface gráfica para ajustar os parâmetros do ruído
def noise_controls_root():
    def noise_controls_root():
        for i in range(2):
            x_offset = 70 if i == 0 else 450

            # Ativar/Desativar Ruído
            tk.Checkbutton(root, text=f"Ativar Ruído Música {i + 1}",
                           command=lambda i=i: toggle_noise(i)).place(x=x_offset, y=300)

            # Volume do Ruído
            tk.Scale(root, from_=0, to=100, orient="horizontal", label="Volume",
                     command=lambda v, i=i: adjust_noise_volume(i, v)).place(x=x_offset, y=350)

            # Modulação AM - Frequência
            tk.Scale(root, from_=0.1, to=10, resolution=0.1, orient="horizontal", label="Freq. AM",
                     command=lambda v, i=i: adjust_am_mod_freq(i, v)).place(x=x_offset, y=400)

            # Modulação AM - Profundidade
            tk.Scale(root, from_=0, to=100, orient="horizontal", label="Profundidade AM",
                     command=lambda v, i=i: adjust_am_depth(i, v)).place(x=x_offset, y=450)

            # Modulação FM - Frequência (apenas para ruído rosa)
            if i == 1:
                tk.Scale(root, from_=0.1, to=10, resolution=0.1, orient="horizontal", label="Freq. FM",
                         command=lambda v, i=i: adjust_fm_mod_freq(i, v)).place(x=x_offset, y=500)

            # Filtro - Frequência de Corte
            tk.Scale(root, from_=100, to=5000, orient="horizontal", label="Freq. Corte Filtro",
                     command=lambda v, i=i: adjust_filter_cutoff(i, v)).place(x=x_offset, y=550)

            # Tipo de Filtro (Lowpass/Highpass)
            tk.Button(root, text="Tipo de Filtro", command=lambda i=i: toggle_filter_type(i)).place(x=x_offset, y=600)


# Criando janela principal
root = tk.Tk()
root.title("Configuração de Ruído")
root.geometry("800x700")

noise_controls_root()
root.mainloop()

"""

