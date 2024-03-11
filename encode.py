import cv2
import numpy as np

import huffmanCode


def read_bin_file(file_name):
    with open(file_name, 'rb') as f:
        data = f.read()
    return data


def encode(data: str, read: str):
    num = 0
    key = ""
    for i in range(len(read)):
        j = 0
        while j < len(key):
            if key[j] == read[i]:
                break
            j += 1
        if j == len(key):
            num += 1
            key += read[i]

    weight = [0 * num]
    for i in range(len(read)):
        for j in range(len(key)):
            if read[i] == key[j]:
                weight[j] += 1
                break

    node = huffmanCode.HuffmanNode()
    codes = ["" * num]

    huffmanCode.create_huffman(node, weight, num)
    huffmanCode.huffman_code(node, num, codes)

    for i in range(num):
        ma = ord(key[i])
        for _ in range(8):
            if ma % 2 == 1:
                data += '1'
            else:
                data += '0'
            ma /= 2
    data += "01111000"

    len_of_data = len(data)
    len_bin = ""
    while len_of_data:
        if len_of_data % 2 == 1:
            len_bin += "1"
        else:
            len_bin += "0"
        len_of_data /= 2

    data = len_bin + "00000000" + data


def draw_qr_code(data: str, img: [cv2.Mat]):
    key = 0
    while True:
        src = np.full((780, 780, 3), (255, 255, 255), dtype=np.uint8)

        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (10, 10), (59, 59), (255, 255, 255), -1)
        cv2.rectangle(src, (20, 20), (49, 49), (0, 0, 0), -1)

        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)

        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)

        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)
        cv2.rectangle(src, (0, 0), (69, 69), (0, 0, 0), -1)

        cv2.putText(src, f"Data: {data[key: key + 10]}", (50, 250), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)

        img.append(src)
        key += 10
        if key >= len(data):
            break
    return img


def pic_to_video(img, file_name):
    frame_rate = 10
    height, width, _ = img[0].shape
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(file_name, fourcc, frame_rate, (width, height))

    for frame in img:
        out.write(frame)

    out.release()


def main():
    data = read_bin_file("in.bin")
    huffman_codes = {}  # Populate this dictionary with Huffman codes

    huffmanCode.huffman_code(data, huffman_codes)

    img = []
    draw_qr_code(data, img)

    # Convert the list of images to video using ffmpeg
    for i, frame in enumerate(img):
        cv2.imwrite(f"frame_{i:03d}.png", frame)

    # subprocess.run(['ffmpeg', '-framerate', '10', '-i', 'frame_%03d.png', '-c:v', 'libx264', '-pix_fmt', 'yuv420p', 'output.mp4'])

    # Clean up temporary files
    # for i in range(len(img)):
    #     os.remove(f"frame_{i:03d}.png")


if __name__ == "__main__":
    main()
