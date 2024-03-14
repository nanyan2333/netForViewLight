import subprocess
import cv2
import numpy as np
import huffmanCode


def read_bin_file(file_name):
    with open(file_name, 'rb') as f:
        data = f.read()
    return data


def encode(data: str, read: str):
    num = 0    #记录字符集中不同字符的数量
    key = ""   #存储字符集中的不同字符
    for i in range(len(read)):
        j = 0
        while j < len(key):
            if key[j] == read[i]:
                break
            j += 1
        if j == len(key):
            num += 1
            key += read[i]

    #给每个字符赋予权重，权重为每个字符的出现次数
    weight = [0 for _ in range(num * 2 - 1)]
    for i in range(len(read)):
        for k in range(len(key)):
            if read[i] == key[k]:
                weight[k] += 1
                break

    node = [huffmanCode.HuffmanNode() for _ in range(num * 2 - 1)]
    codes = ["" for _ in range(num)]  #存储每个字符的哈夫曼编码
    huffmanCode.create_huffman(node, weight, num)
    huffmanCode.huffman_code(node, num, codes)

    for i in range(num):
        ma = ord(key[i])
        for _ in range(8):
            if ma % 2 == 1:
                data += '1'
            else:
                data += '0'
            ma //= 2
    data += "01111000"   #特定结束标识符

    for i in range(num):
        w = weight[i]
        for j in range(32):
            if w % 2 == 1:
                data += '1'
            else:
                data += '0'
            w //= 2

    for i in range(len(read)):
        for j in range(len(key)):
            if key[j] == read[i]:
                data += codes[j]
                break

    len_of_data = len(data)
    len_bin = ""
    while len_of_data:
        if len_of_data % 2 == 1:
            len_bin += "1"
        else:
            len_bin += "0"
        len_of_data //= 2

    data = len_bin + "00000000" + data

    return data  # 返回编码后的数据


def draw_qr_code(data: str, img: list[np.ndarray]):
    key = 0
    while True:
        src = np.full((780, 780, 3), (255, 255, 255), dtype=np.uint8)

        #定位点黑白比为 1:1:3:1:1
        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (10, 10), (59, 59), (255, 255, 255), -1)
        cv2.rectangle(src, (20, 20), (49, 49), (0, 0, 0), -1)

        cv2.rectangle(src, (710, 0), (779, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (720, 10), (769, 59), (255, 255, 255), -1)
        cv2.rectangle(src, (730, 20), (759, 49), (0, 0, 0), -1)

        cv2.rectangle(src, (0, 710), (69, 779), (0, 0, 0), -1)
        cv2.rectangle(src, (10, 720), (59, 769), (255, 255, 255), -1)
        cv2.rectangle(src, (20, 730), (49, 759), (0, 0, 0), -1)

        cv2.rectangle(src, (690, 690), (739, 739), (0, 0, 0), -1)
        cv2.rectangle(src, (700, 700), (729, 729), (255, 255, 255), -1)
        cv2.rectangle(src, (710, 710), (719, 719), (0, 0, 0), -1)

        bits = 8
        number = len(img)
        for col in range(78):
            for row in range(78):
                if key >= len(data):
                    background = np.full((860, 860, 3), (255, 255, 255), dtype=np.uint8)
                    background[40:820, 40:820] = src
                    img.append(background)
                    return
                if col < 8 and (row < 8 or row > 69) or (col > 69 and row < 8) or (67 < col < 75 and row > 67 or row < 75):
                    continue
                elif bits != 0:
                    if number % 2 == 1:
                        cv2.rectangle(src, (col * 10, row * 10), (col * 10 + 9, row * 10 + 9), (0, 0, 0), -1)
                    else:
                        cv2.rectangle(src, (col * 10, row * 10), (col * 10 + 9, row * 10 + 9), (255, 255, 255), -1)
                    number //= 2  # 修正除法操作为整数除法
                    bits -= 1
                else:
                    if data[key] == '1':
                        cv2.rectangle(src, (col * 10, row * 10), (col * 10 + 9, row * 10 + 9), (0, 0, 0), -1)
                    else:
                        cv2.rectangle(src, (col * 10, row * 10), (col * 10 + 9, row * 10 + 9), (255, 255, 255), -1)
                    key += 1
        cv2.imwrite('temp.png',src)
        background = np.full((860, 860, 3), (255, 255, 255), dtype=np.uint8)
        background[40:820, 40:820] = src
        cv2.imwrite('temp2.png',background)
        img.append(background)



def pic_to_video(img):
    for i, frame in enumerate(img):
        cv2.imwrite(f"./assets/img/frame_{i:04d}.png", frame)
    subprocess.run(
        ["ffmpeg", "-framerate", "25", "-i", f"./assets/img/frame_%04d.png", "-c:v", "libx264", "-pix_fmt",
         "yuv420p", "out.mp4"])


def main():
    string = read_bin_file("../assets/bin/in.bin")
    data = ""
    encoded_data = encode(data, str(string))  # 编码数据
    img = []

    draw_qr_code(encoded_data, img)

    pic_to_video(img)

    # 保存视频
    # pic_to_video(img, "output.mp4")


if __name__ == "__main__":
    main()


