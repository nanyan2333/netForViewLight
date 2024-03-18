import cv2
import numpy as np


def get_angle(v1, v2):
    mod1 = np.sqrt(v1[0] ** 2 + v1[1] ** 2)
    mod2 = np.sqrt(v2[0] ** 2 + v2[1] ** 2)
    angle = np.arccos(np.dot(v1, v2) / (mod1 * mod2)) * 180.0 / np.pi
    return angle


def identify_qr_code(img, qr_codes: list[np.ndarray]):
    mid_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)    #对图像进行灰度处理

    for _ in range(3):
        mid_img = cv2.GaussianBlur(mid_img, (3, 3), 0)     #对灰度图像进行高斯模糊处理

    mid_img = cv2.threshold(mid_img, 127, 255, cv2.THRESH_BINARY)

    contours, hierarchy = cv2.findContours(mid_img, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

    parent_index = -1
    mark = 0
    rect_points = []
    min_rect = -1
    min_rect_idx = -1

    for j, contour in enumerate(contours):
        if cv2.contourArea(contour) > 50000:
            continue
        if hierarchy[0, j, 2] != -1 and mark == 0:
            parent_index = j
            mark += 1
        elif hierarchy[0, j, 2] != -1 and mark == 1:
            mark += 1
        elif hierarchy[0, j, 2] == -1:
            parent_index = -1
            mark = 0

        if hierarchy[0, j, 2] != -1 and mark == 2:
            rect = cv2.minAreaRect(contours[parent_index])
            rect_points.append(rect[0])

            if min_rect == -1 or cv2.contourArea(contours[parent_index]) < min_rect:
                min_rect = cv2.contourArea(contours[parent_index])
                min_rect_idx = len(rect_points) - 1

            parent_index = -1
            mark = 0

    if len(rect_points) < 4:
        return

    P1 = [None] * 4
    P1[3] = rect_points[min_rect_idx]
    rect_points.pop(min_rect_idx)

    angles = [
        get_angle(rect_points[1] - rect_points[0], rect_points[2] - rect_points[0]),
        get_angle(rect_points[0] - rect_points[1], rect_points[2] - rect_points[1]),
        get_angle(rect_points[0] - rect_points[2], rect_points[1] - rect_points[2])
    ]

    mark = np.argmax(angles)
    P1[0] = rect_points[mark]

    mid_point = ((rect_points[(mark + 1) % len(rect_points)] + rect_points[(mark + 2) % len(rect_points)]) / 2)
    temp_vec1 = rect_points[(mark + 1) % len(rect_points)] - P1[0]
    temp_vec2 = rect_points[(mark + 2) % len(rect_points)] - P1[0]
    x1, y1 = temp_vec1 / np.linalg.norm(temp_vec1)
    x2, y2 = temp_vec2 / np.linalg.norm(temp_vec2)

    if P1[0][0] <= mid_point[0]:
        if y1 < y2:
            P1[1] = rect_points[(mark + 1) % len(rect_points)]
            P1[2] = rect_points[(mark + 2) % len(rect_points)]
        elif y1 > y2:
            P1[1] = rect_points[(mark + 2) % len(rect_points)]
            P1[2] = rect_points[(mark + 1) % len(rect_points)]
        elif y1 == y2:
            if P1[0][1] > mid_point[1]:
                if x1 < x2:
                    P1[1] = rect_points[(mark + 1) % len(rect_points)]
                    P1[2] = rect_points[(mark + 2) % len(rect_points)]
                else:
                    P1[1] = rect_points[(mark + 2) % len(rect_points)]
                    P1[2] = rect_points[(mark + 1) % len(rect_points)]
    else:
        if y1 > y2:
            P1[1] = rect_points[(mark + 1) % len(rect_points)]
            P1[2] = rect_points[(mark + 2) % len(rect_points)]
        elif y1 < y2:
            P1[1] = rect_points[(mark + 2) % len(rect_points)]
            P1[2] = rect_points[(mark + 1) % len(rect_points)]

    P2 = np.array([[35, 35], [745, 35], [35, 745], [715, 715]], dtype=np.float32)

    matrix = cv2.getPerspectiveTransform(np.array(P1, dtype=np.float32), P2)
    dst_img = cv2.warpPerspective(mid_img, matrix, (780, 780), flags=cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT,
                                  borderValue=(255, 255, 255))

    qr_codes.append(dst_img)