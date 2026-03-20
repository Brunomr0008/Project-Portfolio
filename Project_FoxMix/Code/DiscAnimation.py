import tkinter as tk
import Config_select
from PIL import Image, ImageTk

rotation_task = [None, None]
angle = [0, 0]
tk_image = [None, None]
img_item = [None, None]

root = tk.Tk()
root.geometry("1610x750")
root.title("FoxMix - IPCA - MEEC")

pil_image = Image.open("Disco.png")
resized_pil_image = pil_image.resize((300, 300), Image.LANCZOS)
canvas = tk.Canvas(root, width=1610, height=750)
canvas.grid(row=0, rowspan=30, column=0, columnspan=15)

def on_close():
    Config_select.stop_file(0)
    Config_select.stop_file(1)
    root.quit()  # Sair do loop principal de Tkinter
    root.destroy()  # Destruir a janela

# Função para atualizar a rotação no canvas
def update_rotation(index):
    global canvas
    # Rotaciona a imagem com o ângulo dado
    rotated_image = resized_pil_image.rotate(angle[index])

    # Atualiza a imagem no canvas
    tk_image[index] = ImageTk.PhotoImage(rotated_image)
    canvas.itemconfig(img_item[index], image=tk_image[index])

    # Armazenar referência para a imagem, senão o Tkinter a coleta
    canvas.image = tk_image[index]

    # Atualiza a rotação em 10 graus a cada ciclo
    angle[index] += 10
    rotation_task[index] = canvas.after(10, update_rotation, index)

# Função para parar a rotação
def stop_rotation(index):
    if rotation_task[index] is not None:
        canvas.after_cancel(rotation_task[index])
        rotation_task[index] = None


def canvas_disc():
    global bg_photo  # Torne bg_photo uma variável global para manter a referência
    root.protocol("WM_DELETE_WINDOW", on_close)

    # Load background image
    background_image = Image.open("Background.png")
    resized_background = background_image.resize((1610, 750), Image.LANCZOS)
    bg_photo = ImageTk.PhotoImage(resized_background)
    canvas.create_image(0, 0, image=bg_photo)

    tk_image[0] = ImageTk.PhotoImage(resized_pil_image)
    tk_image[1] = ImageTk.PhotoImage(resized_pil_image)

    #img_item[0] = canvas.create_image(170, 400, image=tk_image[0])
    #img_item[1] = canvas.create_image(1440, 400, image=tk_image[1])

    # Title
    title_label = tk.Label(root, text="🎵 FoxMix 🎶", font=("Arial", 35, "bold"), fg="black", bg="#7F7F7F")
    # Title Position
    title_label.place(x=650, y=20)

    # Load Image
    title_image = Image.open("FoxMix.png")
    resized_title_image = title_image.resize((183, 216), Image.LANCZOS)
    # Convert to PhotoImage
    title_photo = ImageTk.PhotoImage(resized_title_image)
    # Label
    #title_label1 = tk.Label(root, image=title_photo, borderwidth=0, highlightthickness=0, background="#1F303A")
    #title_label1.place(x=420, y=10)    # Position
    # Avoid garbage collector
    title_label.image = title_photo

    # Load Image 2
    title_image2 = Image.open("FoxMix.png")
    resized_title_image2 = title_image2.resize((183, 216), Image.LANCZOS)
    # Convert to PhotoImage
    title_photo2 = ImageTk.PhotoImage(resized_title_image2)
    # Label
    #title_label2 = tk.Label(root, image=title_photo2, borderwidth=0, highlightthickness=0, background="#1F303A")
    # Position
    #title_label2.place(x=1010, y=10)
    # Avoid garbage collector
    #title_label2.image = title_photo2

    # Load Image 4
    title_image4 = Image.open("IPCA.png")
    resized_title_image4 = title_image4.resize((163, 90), Image.LANCZOS)
    # Convert to PhotoImage
    title_photo4 = ImageTk.PhotoImage(resized_title_image4)
    # Label
    #title_label4 = tk.Label(root, image=title_photo4, borderwidth=0, highlightthickness=0, background="#7F7F7F")
    # Position
    #title_label4.place(x=725, y=77)
    # Avoid garbage collector
    #title_label4.image = title_photo4