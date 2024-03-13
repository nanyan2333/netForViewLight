import huffmanCode
import cv2
import numpy as np
import subprocess

def video_to_pic(fileName: str):
    subprocess.run(["ffmpeg", "-i", f"./assets/video/{fileName}", "-vf", "fps=25", f"./assets/img/output_%04d.png", ])


def QRCode_to_data(img: list[np.ndarray]):
    data = ""
    page = -1
    for i in range(len(img)):
        bits = 8
        number = 0
        while bits:
            color = img[i][5][(8 + 8 - bits) * 10 + 5]
            if color[0] < 128:
                number += 2 ** (8 - bits)
            bits -= 1

        if number != page + 1:  # 判断页数
            continue
        else:
            page += 1

        for col in range(78):
            for row in range(78):
                if (col < 8 and (row < 8 or row > 69)) or (col > 69 and row < 8) or (
                        67 < col < 75 and 67 < row < 75) or (col == 0 and row < 16):
                    continue
                else:
                    color = img[i][col * 10 + 5, row * 10 + 5]
                    if color[0] > 128:
                        data += '0'
                    else:
                        data += '1'
    return data


def huffman_decode(data: str):
    length = 0
    index = data.find("00000000", 0)
    for i in range(index):
        if data[i] == '1':
            length += 2 ** i
    key = ""
    ascii = 0
    index += 8
    length += index
    while True:
        for i in range(8):
            if data[index] == '1':
                ascii += 2 ** i
            index += 1
        if ascii == 30:
            break
        else:
            key += chr(ascii)
            ascii = 0
    num = len(key)
    weight = [0] * num
    for i in range(num):
        for j in range(32):
            if data[index] == '1':
                weight[i] += 2 ** j
            index += 1

    node = [huffmanCode.HuffmanNode() for _ in range(num * 2 - 1)]
    huffmanCode.create_huffman(node, weight, num)

    current = num * 2 - 2
    str = ""
    while index <= length:
        if data[index] == '0' and p[current].lchild != -1:
            current = p[current].lchild
            index += 1
        elif data[index] == '1' and p[current].rchild != -1:
            current = p[current].rchild
            index += 1
        elif p[current].lchild == -1 and p[current].rchild == -1:
            str += key[current]
            current = num * 2 - 2
    return str


def write_bin_file(string, file_name):
    try:
        with open(file_name, 'wb') as out_file:
            out_file.write(string.encode('utf-8'))
    except IOError:
        print("Unable to open file!")
        exit(0)
