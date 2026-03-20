import queue
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import BPM, DiscAnimation

import matplotlib
matplotlib.use('TkAgg')  # Ensure this is before importing pyplot
import matplotlib.pyplot as plt

# Fila para armazenar blocos de áudio
audio_queue = queue.Queue()
n_musica = [0, 0]

fig, ax = plt.subplots(1, figsize=(3.5, 4), sharex=True, sharey=True)
graph_resume = False

# Função para atualização contínua do gráfico
def update_graph(frames):
    if not audio_queue.empty() and graph_resume == True:
        ax.clear()

        if n_musica[0] == 1:
            # Pega o bloco de áudio para a primeira música
            filtered_block1, samplerate1, index1 = audio_queue.get()

            # Atualizar o display dos BPMs em tempo real
            BPM.bpm_labels[index1].config(text=f"BPM: {BPM.bpm_values[index1]:.1f}")

            # Número total de frames no bloco atual para a primeira música
            num_samples1 = filtered_block1.shape[0]

            # Tempo total do bloco para ambas as músicas
            duration1 = num_samples1 / samplerate1

            # Vetores de tempo para ambas as músicas
            t1 = np.linspace(0, duration1, num_samples1)

            # Primeira música (ajustar para a escala de tempo)
            ax.plot(t1, filtered_block1.T, ".", color="blue")

        if n_musica[1] == 1:
            filtered_block2, samplerate2, index2 = audio_queue.get()
            BPM.bpm_labels[index2].config(text=f"BPM: {BPM.bpm_values[index2]:.1f}")
            num_samples2 = filtered_block2.shape[0]
            duration2 = num_samples2 / samplerate2
            t2 = np.linspace(0, duration2, num_samples2)
            ax.plot(t2, filtered_block2.T, ".", color="red")

        # Personalização do Gráfico
        ax.set_title('Audio Signal Frequencies', color='black')  # Cor do título
        ax.set_xlabel('Time (s)', color='black')  # Cor do eixo X
        ax.set_ylabel('Amplitude', color='black')  # Cor do eixo Y
        ax.set_facecolor("#7F7F7F")  # Fundo da área do gráfico
        fig.patch.set_facecolor("#7F7F7F")  # Fundo geral do gráfico
        ax.spines['bottom'].set_color('black')  # Eixo X
        ax.spines['left'].set_color('black')  # Eixo Y
        ax.spines['top'].set_color('black')  # Remove borda superior
        ax.spines['right'].set_color('black')  # Remove borda direita
        ax.tick_params(axis='x', colors='black')  # Cor dos números do eixo X
        ax.tick_params(axis='y', colors='black')  # Cor dos números do eixo Y

        ax.set_xlim([0, 0.75])
        ax.set_ylim([-1, 1])
        ax.label_outer()
        plt.draw()


def graph_root():
    # Personalização do Gráfico
    ax.set_title('Audio Signal Frequencies', color='black')  # Cor do título
    ax.set_xlabel('Time (s)', color='black')  # Cor do eixo X
    ax.set_ylabel('Amplitude', color='black')  # Cor do eixo Y
    ax.set_facecolor("#7F7F7F")  # Fundo da área do gráfico
    fig.patch.set_facecolor("#7F7F7F")  # Fundo geral do gráfico
    ax.spines['bottom'].set_color('black')  # Eixo X
    ax.spines['left'].set_color('black')  # Eixo Y
    ax.spines['top'].set_color('black')  # Remove borda superior
    ax.spines['right'].set_color('black')  # Remove borda direita
    ax.tick_params(axis='x', colors='black')  # Cor dos números do eixo X
    ax.tick_params(axis='y', colors='black')  # Cor dos números do eixo Y

    ax.set_xlim([0, 0.75])
    ax.set_ylim([-1, 1])
    ax.label_outer()
    plt.draw()

    # Integrar Matplotlib com Tkinter usando FigureCanvasTkAgg
    canvas_graph = FigureCanvasTkAgg(fig, master=DiscAnimation.canvas)
    canvas_graph.draw()
    canvas_graph.get_tk_widget().place(x=630, y=160)