import os
import random
import sys


def generate_binary_file(file_path, width, height):
    with open(file_path, 'wb') as file:
        for _ in range(height):
            row = bytes([random.randint(0, 255) for _ in range(width)])
            file.write(row)


output_dir = './assets/bin'  # 指定生成文件的目录
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

num = sys.argv[1]
file_name = 'in' + num + ".bin"
output_path = os.path.join(output_dir, file_name)  # 构建完整的文件路径

width = 640
height = 480
generate_binary_file(output_path, width, height)
print(f'in{num}.bin has build.\n')