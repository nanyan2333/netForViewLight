CRC_32_Tbl = [0] * 256
crc = 0


def gen_crc32tbl():
    for i in range(256):
        crc = i
        for j in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xEDB88320
            else:
                crc >>= 1
        CRC_32_Tbl[i] = crc


def crc_main(file_path: str):
    with open(file_path, 'rb') as file:
        file_data = file.read()
        gen_crc32tbl()
        tmp = bytearray(100)
        end = ""
        for i in range(len(file_data)):
            if i % 100 == 0 and i != 0:
                CRC32 = 0xffffffff
                for j in range(100):
                    end += chr(tmp[j])
                    # print(chr(tmp[j]))
                    CRC32 = CRC_32_Tbl[(CRC32 ^ tmp[j]) & 0xff] ^ (CRC32 >> 8)
                end += chr((CRC32 & 0xff000000) >> 24)
                end += chr((CRC32 & 0xff0000) >> 16)
                end += chr((CRC32 & 0xff00) >> 8)
                end += chr(CRC32 & 0xff)
            tmp[i % 100] = file_data[i]
            if i == len(file_data) - 1:
                CRC32 = 0xffffffff
                for j in range(i % 100 + 1):
                    end+=chr(tmp[j])
                    CRC32 = CRC_32_Tbl[(CRC32 ^ tmp[j]) & 0xff] ^ (CRC32 >> 8)
                end += chr((CRC32 & 0xff000000) >> 24)
                end += chr((CRC32 & 0xff0000) >> 16)
                end += chr((CRC32 & 0xff00) >> 8)
                end += chr(CRC32 & 0xff)
        with open('./assets/bin/in_crc.bin', 'wb') as out:
            out.write(end.encode())



