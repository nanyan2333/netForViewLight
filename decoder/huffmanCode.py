class HuffmanNode:
    def __init__(self):
        self.lchild = None
        self.rchild = None
        self.parent = None
        self.weight = 0


def init_weight(p, weight, num_leaves):
    for i in range(num_leaves * 2 - 1):
        p[i].lchild = -1
        p[i].rchild = -1
        p[i].parent = -1
        p[i].weight = weight[i]


def search_min(p, num, min1, min2):   #寻找权重最小的两个节点
    for i in range(num):
        if p[i].parent == -1:
            min1[0] = i
            for j in range(num):
                if p[j].parent == -1 and p[j].weight < p[min1[0]].weight:
                    min1[0] = j
            break

    for i in range(num):
        if p[i].parent == -1 and i != min1[0]:
            min2[0] = i
            for j in range(num):
                if p[j].parent == -1 and j != min1[0] and p[j].weight < p[min2[0]].weight:
                    min2[0] = j
            break


def create_huffman(p, weight, num_leaves):
    min1 = [0]
    min2 = [0]
    init_weight(p, weight, num_leaves)

    for i in range(num_leaves - 1):
        search_min(p, num_leaves + i, min1, min2)
        p[num_leaves + i].weight = p[min1[0]].weight + p[min2[0]].weight
        p[num_leaves + i].lchild = min1[0]
        p[num_leaves + i].rchild = min2[0]
        p[min1[0]].parent = num_leaves + i
        p[min2[0]].parent = num_leaves + i


def huffman_code(p, num_leaves, codes):
    for i in range(num_leaves):
        j = i
        while True:
            if p[j].parent == -1:
                break
            parent = p[j].parent
            if p[parent].lchild == j:
                codes[i] = '0' + codes[i]
            else:
                codes[i] = '1' + codes[i]
            j = parent