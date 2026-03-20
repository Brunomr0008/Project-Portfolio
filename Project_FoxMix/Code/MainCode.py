import soundfile as sf
import sounddevice as sd
import threading
import numpy as np
import tkinter as tk
from matplotlib.animation import FuncAnimation
from scipy.signal import resample
import Filters, DiscAnimation, Config_select, Graph, BPM, Reverb, Vibrato
from cffi import FFI  # to deal with buffer

# Configurações
block_size = 16384
ffi = FFI()  # buffering

def _play(sound, index):
    event = threading.Event()

    def callback(outdata, frames, time, status):
        # Calculate how many input frames we need based on speed
        input_frames = int(frames * BPM.speeds[index])

        try:
            data = wf.buffer_read(input_frames, dtype='float32')
        except Exception:
            data = np.zeros((frames * 2,), dtype='float32')  # Preencher com zeros se houver erro
            outdata[:] = data
            raise sd.CallbackStop  # Parar o callback se houver erro

        if len(data) == 0:
            outdata[:] = np.zeros_like(outdata)  # Preencher com zeros quando o áudio terminar
            raise sd.CallbackStop  # Parar o stream quando acabar o áudio

        else:
            # Convert to numpy array
            indata = np.frombuffer(data, dtype=np.float32)

            # Split channels
            indata_l = indata[0::2]
            indata_r = indata[1::2]

            # Resample to match desired output length
            resampled_l = resample(indata_l, frames)
            resampled_r = resample(indata_r, frames)

            # Apply volume
            resampled_l *= Config_select.volumes[index]
            resampled_r *= Config_select.volumes[index]

            # Apply filters
            filtered_l = Filters.apply_filters(resampled_l, Filters.current_filters, index)
            filtered_r = Filters.apply_filters(resampled_r, Filters.current_filters, index)

            # Aplicar vibrato aos sinais filtrados
            vibrato_left = Vibrato.apply_vibrato(filtered_l, wf.samplerate, index)
            vibrato_right = Vibrato.apply_vibrato(filtered_r, wf.samplerate, index)

            #  Apply Reverb
            reverb_left = Reverb.apply_reverb(vibrato_left, wf.samplerate, index)
            reverb_right = Reverb.apply_reverb(vibrato_right, wf.samplerate, index)

            # Create stereo output array
            filtered_block = np.empty((frames * 2,), dtype=np.float32)
            filtered_block[0::2] = reverb_left
            filtered_block[1::2] = reverb_right

            # Send to queue for visualization
            Graph.audio_queue.put((filtered_block, wf.samplerate, index))

            # Assign to output
            outdata[:] = filtered_block.reshape(frames, 2)

    def finished_callback():
        Config_select.stop_file(index)
        if Config_select.enabled_restart[index]:
            play_file(index)  # Reinicia a música

    with sf.SoundFile(sound) as wf:
        if index is not None:
            Filters.update_filters(wf.samplerate, index)

        # Adjust blocksize based on speed
        adjusted_blocksize = block_size
        if BPM.speeds[index] > 1.0:
            adjusted_blocksize = int(block_size * BPM.speeds[index])

        Config_select.streams[index] = sd.RawOutputStream(
            samplerate=wf.samplerate,
            channels=wf.channels,
            callback=callback,
            blocksize=adjusted_blocksize,
            finished_callback=finished_callback)

        with Config_select.streams[index]:
            event.wait()

        if index is not None:
            Filters.stop_filter_update(index)

# Reproduzir música
def play_file(index):
    Graph.n_musica[index] = 1

    DiscAnimation.update_rotation(index)
    Graph.graph_resume = True

    Config_select.buttons_label[index].config(text=f"Music {index + 1} Playing")
    Config_select.buttons_select[index].config(state=tk.DISABLED)
    Config_select.buttons_play[index].config(state=tk.DISABLED)
    Config_select.buttons_stop[index].config(state=tk.NORMAL)
    new_thread = threading.Thread(target=_play, args=(Config_select.filenames[index], index))
    new_thread.start()

# GUI da interface
if __name__ == "__main__":
    DiscAnimation.canvas_disc()
    Reverb.reverb_controls_root()
    Vibrato.vibrato_controls_root()
    Config_select.config_root()
    Filters.filters_root()
    BPM.bpm_root()
    Graph.graph_root()

    # Iniciar animação para o gráfico com intervalos de 500ms
    #ani = FuncAnimation(Graph.fig, Graph.update_graph, interval=1000, blit=False, cache_frame_data=False)

    DiscAnimation.root.mainloop()