import serial
import time
import csv
import os

# --- CONFIGURAÇÃO ---
PORTA_SERIE = 'COM7'  # ALTERAR para a porta do teu ESP32 (ex: 'COM3' ou '/dev/ttyUSB0')
VELOCIDADE = 115200  # Deve ser igual ao definido no Arduino
NOME_FICHEIRO = 'tecido_dataset.csv'  # MUDAR para o material em teste (ex: metal_dataset.csv)

MODO = ""   # "HSV" ou "VIB"
MAX_LINHAS = 10000

# O cabeçalho CSV deve ser o mesmo que o definido no Arduino
CABECALHO = ['CLASSE', 'Hue', 'Saturation', 'Value', 'Vibration', 'Metal']

def recolher_dados():
    print("-" * 50)
    print(f"A iniciar a aquisição de dados para: {NOME_FICHEIRO}")
    print(f"Modo: {MODO} | Linhas: {MAX_LINHAS}")
    print("-" * 50)

    ser = serial.Serial(PORTA_SERIE, VELOCIDADE)
    time.sleep(2)
    ser.reset_input_buffer()

    classe = NOME_FICHEIRO.split('_')[0].capitalize()
    linhas_lidas = 0
    dados_extra = []

    if MODO == "HSV":
        comando_start = b"START_HSV\n"
    elif MODO == "VIB":
        comando_start = b"START_VIB\n"
    else:
        comando_start = b"START_METAL\n"

    ser.write(comando_start)

    # ===== HSV =====
    if MODO == "HSV":
        with open(NOME_FICHEIRO, 'a', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)

            if os.path.getsize(NOME_FICHEIRO) == 0:
                writer.writerow(CABECALHO)

            while linhas_lidas < MAX_LINHAS:
                if ser.in_waiting:
                    linha = ser.readline().decode('utf-8', errors='ignore').strip()
                    if linha.startswith('?'):
                        h, s, v = linha[1:].split(',')
                        writer.writerow([classe, h, s, v, '', ''])
                        linhas_lidas += 1
                        print(f"HSV {linhas_lidas}/{MAX_LINHAS} Dados -> H:{h} | S:{s} | V:{v}")

    # ===== VIB ou METAL =====
    else:
        while linhas_lidas < MAX_LINHAS:
            if ser.in_waiting:
                linha = ser.readline().decode('utf-8', errors='ignore').strip()
                if linha.startswith('?'):
                    valor = linha[1:]
                    dados_extra.append(valor)
                    linhas_lidas += 1
                    print(f"{MODO} {linhas_lidas}/{MAX_LINHAS} -> {valor}")

        preencher_coluna_csv(dados_extra, MODO)

    ser.write(b"STOP\n")
    ser.close()

    print("-" * 50)
    print(f"Aquisição concluída. {linhas_lidas} linhas guardadas.")
    print("-" * 50)

def preencher_coluna_csv(dados, modo):
    with open(NOME_FICHEIRO, newline='', encoding='utf-8') as f:
        linhas = list(csv.reader(f))

    header = linhas[0]
    dados_csv = linhas[1:]

    coluna_idx = header.index('Vibration') if modo == "VIB" else header.index('Metal')

    start_idx = 0
    for i, linha in enumerate(dados_csv):
        if linha[coluna_idx] == '':
            start_idx = i
            break
    else:
        print("AVISO: Não há linhas livres.")
        return

    for i, valor in enumerate(dados):
        idx = start_idx + i
        if idx < len(dados_csv):
            dados_csv[idx][coluna_idx] = valor
        else:
            break

    with open(NOME_FICHEIRO, 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(header)
        writer.writerows(dados_csv)


if __name__ == "__main__":
    recolher_dados()