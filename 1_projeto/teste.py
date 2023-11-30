def ordenar_por_rrn(lines):
    # Extrai o RRN e o índice de cada linha
    rrn_indices = []

    for idx, line in enumerate(lines):
        elements = line.split('|')
        if len(elements) >= 4:
            rrn_indices.append((int(elements[3]), idx))
        else:
            print(f"A linha {idx + 1} não tem elementos suficientes: {line}")

    # Ordena as linhas com base no RRN
    rrn_indices.sort(key=lambda x: x[0])

    # Reconstrói as linhas ordenadas
    linhas_ordenadas = [lines[idx] for _, idx in rrn_indices]

    return linhas_ordenadas

# Ler linhas do arquivo
with open("c.txt", "r") as arquivo:
    linhas = arquivo.readlines()

# Ordenar as linhas
linhas_ordenadas = ordenar_por_rrn(linhas)

# Gravar linhas ordenadas em um novo arquivo
with open("cout.txt", "w") as novo_arquivo:
    for linha in linhas_ordenadas:
        novo_arquivo.write(linha)
