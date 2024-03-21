import os
import subprocess
import sys

import cv2
import numpy as np
# hhh

# 编码二维码

def read_bin_file(file_name):
    with open(file_name, 'rb') as f:
        data = f.read()
        # print(data)
        # print(str(data))
    return data

def draw_qr_code(data: str, img: list[np.ndarray]):
    key = 0
    while True:
        src = np.full((780, 780, 3), (255, 255, 255), dtype=np.uint8)

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
                if col < 8 and (row < 8 or row > 69) or (col > 69 and row < 8) or (
                        67 < col < 75 and 67< row < 75):
                    continue

                else:
                    if data[key] == '1':
                        cv2.rectangle(src, (col * 10, row * 10), (col * 10 + 9, row * 10 + 9), (0, 0, 0), -1)
                    else:
                        cv2.rectangle(src, (col * 10, row * 10), (col * 10 + 9, row * 10 + 9), (255, 255, 255), -1)
                    key += 1
        background = np.full((860, 860, 3), (255, 255, 255), dtype=np.uint8)
        background[40:820, 40:820] = src
        img.append(background)



def pic_to_video(img,frame_rate,output):
    for i, frame in enumerate(img):
        cv2.imwrite(f"./assets/img/frame_{i:04d}.png", frame)
    subprocess.run(
        ["ffmpeg", "-framerate", f"{frame_rate}", "-i", f"./assets/img/frame_%04d.png", "-c:v", "libx264", "-pix_fmt",
         "yuv420p", f"./assets/video/{output}"])
    for i in range(len(img)):
        os.remove(f"./assets/img/frame_{i:04d}.png")


def main():
    filename,output,frame_rate=sys.argv[1],sys.argv[2],sys.argv[3]
    string = read_bin_file(f"./assets/bin/{filename}")

    img = []

    draw_qr_code(str(string), img)
    print(len(img))

    pic_to_video(img,frame_rate,output)
    # 保存视频


if __name__ == "__main__":
    main()
