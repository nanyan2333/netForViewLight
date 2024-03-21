import cv2
import numpy as np

import huffmanCode
import identify


def video_to_pic(file_name, img, rate):
    video = cv2.VideoCapture(file_name)
    frame_num = int(video.get(cv2.CAP_PROP_FRAME_COUNT))
    fps = video.get(cv2.CAP_PROP_FPS)

    frame_interval = int(fps / rate)  # 计算帧间隔
    frame_counter = 0

    for i in range(frame_num):
        ret, frame = video.read()
        if ret:
            frame_counter += 1
            if frame_counter % frame_interval == 0:
                img.append(frame)
        else:
            break


def qrcode_to_data(img: list[np.ndarray], data: str):
    page = -1
    print(len(img))
    for i in range(len(img)):
        for col in range(78):
            for row in range(78):
                if (col < 8 and (row < 8 or row > 69)) or (col > 69 and row < 8) or (
                        67 < col < 75 and 67 < row < 75) or (col == 0 and row < 16):
                    continue
                else:
                    color = img[i][col * 10 + 5, row * 10 + 5]
                    if color > 128:
                        data += '0'
                    else:
                        data += '1'
    return data


# def huffman_decode(data: str):
#     length = 0
#     index = data.find("00000000", 0)
#     for i in range(index):
#         if data[i] == '1':
#             length += 2 ** i
#     key = ""
#     ascii_value = 0
#     index += 8
#     length += index
#     while True:
#         for i in range(8):
#             if data[index] == '1':
#                 ascii_value += 2 ** i
#             index += 1
#         if ascii_value == 30:
#             break
#         else:
#             key += chr(ascii_value)
#             ascii_value = 0
#     num = len(key)
#     weight = [0] * (num * 2 - 1)
#     for i in range(num):
#         for j in range(32):
#             if data[index] == '1':
#                 weight[i] += 2 ** j
#             index += 1
#
#     node = [huffmanCode.HuffmanNode() for _ in range(num * 2 - 1)]
#     huffmanCode.create_huffman(node, weight, num)
#
#     current = num * 2 - 2
#     str = ""
#     while index <= length:
#         if data[index] == '0' and node[current].lchild != -1:
#             current = node[current].lchild
#             index += 1
#         elif data[index] == '1' and node[current].rchild != -1:
#             current = node[current].rchild
#             index += 1
#         elif node[current].lchild == -1 and node[current].rchild == -1:
#             str += key[current]
#             current = num * 2 - 2
#     return str


def write_bin_file(string, file_name):
    with open(file_name, 'w') as file:
        file.write(string)


def main():
    video_img = []
    video_to_pic("./assets/video/out.mp4", video_img,10)

    qrcode = []
    for qrimg in video_img:
        identify.identify_qr_code(qrimg, qrcode)
    for idx,qr in enumerate(qrcode):
        cv2.imwrite(f"./assets/img/test_{idx:02d}.png",qr)

    video_data = ""
    video_data = qrcode_to_data(qrcode, video_data)
    # video_string = huffman_decode(video_data)
    # print(video_string)
    write_bin_file(video_data, "./assets/bin/out.bin")


if __name__ == "__main__":
    main()
