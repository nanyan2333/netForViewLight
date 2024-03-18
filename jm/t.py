from PIL import Image
import numpy as np

# 生成随机图像的尺寸
width = 640
height = 480

# 生成随机图像数据
random_image_data = np.random.randint(0, 256, (height, width, 3), dtype=np.uint8)

# 创建 PIL 图像对象
random_image = Image.fromarray(random_image_data)

# 将图像保存为 PNG 文件
random_image.save('random_image.png')

# 将图像数据写入二进制文件
with open('random_image.bin', 'wb') as f:
    f.write(random_image_data.tobytes())
