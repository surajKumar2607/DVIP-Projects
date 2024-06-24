from struct import unpack

marker_mapping = {
    0xffd8: "Start of Image",
    0xffe0: "Application Default Header",
    0xffdb: "Quantization Table",
    0xffc0: "Start of Frame",
    0xffc4: "Define Huffman Table",
    0xffda: "Start of Scan",
    0xffd9: "End of Image"
}


class HuffmanTable:
    def __init__(self):
        self.root = []
        self.elements = []

    def BitsFromLengths(self, root, element, pos):
        if isinstance(root, list):
            if pos == 0:
                if len(root) < 2:
                    root.append(element)
                    return True
                return False
            for i in [0, 1]:
                if len(root) == i:
                    root.append([])
                if self.BitsFromLengths(root[i], element, pos - 1) == True:
                    return True
        return False

    def GetHuffmanBits(self, lengths, elements):
        self.elements = elements
        ii = 0
        for i in range(len(lengths)):
            for j in range(lengths[i]):
                self.BitsFromLengths(self.root, elements[ii], i)
                ii += 1

    def Find(self, st):
        r = self.root
        while isinstance(r, list):
            r = r[st.GetBit()]
        return r

    def GetCode(self, st):
        while (True):
            res = self.Find(st)
            if res == 0:
                return 0
            elif (res != -1):
                return res

class JPEG:
    def __init__(self, image_file):
        with open(image_file, 'rb') as f:
            self.img_data = f.read()
    def decodeHuffman(self, data):
        offset = 0
        header, = unpack("B", data[offset:offset + 1])
        offset += 1

        # Extract the 16 bytes containing length data
        lengths = unpack("BBBBBBBBBBBBBBBB", data[offset:offset + 16])
        offset += 16

        # Extract the elements after the initial 16 bytes
        elements = []
        for i in lengths:
            elements += (unpack("B" * i, data[offset:offset + i]))
            offset += i

        print("Header: ", header)
        print("lengths: ", lengths)
        print("Elements: ", len(elements))
        data = data[offset:]

        hf = HuffmanTable()
        hf.GetHuffmanBits(lengths, elements)
        data = data[offset:]

    def decode(self):
        data = self.img_data

        while (True):
            marker, = unpack(">H", data[0:2])
            print(marker_mapping.get(marker))
            if marker == 0xffd8: #"Start of Image",
                data = data[2:] #Shift the data to right
            elif marker == 0xffd9: #"End of Image",
                return
            elif marker == 0xffda:  #"Start of Scan",
                data = data[-2:] #Since EOF (end of file) is at last 2 bytes

           # else:
           #     lent, = unpack(">H", data[2:4]) #Go to next 2 byte of marker and get the length
            #    data = data[(2 + lent):] #move the data by that length
           # if len(data) == 0:
              #  break

            else:
                len_chunk, = unpack(">H", data[2:4])#Go to next 2 byte of marker and get the length
                len_chunk += 2
                chunk = data[4:len_chunk] #move the data by that length and create a chunk for huffman
                if marker == 0xffc4:
                    self.decodeHuffman(chunk)
                data = data[len_chunk:]
            if len(data) == 0:
                break

class Stream:
    def __init__(self, data):
        self.data= data
        self.pos = 0

    def GetBit(self):
        b = self.data[self.pos >> 3]
        s = 7-(self.pos & 0x7)
        self.pos+=1
        return (b >> s) & 1

    def GetBitN(self, l):
        val = 0
        for i in range(l):
            val = val*2 + self.GetBit()
        return val

if __name__ == "__main__":
    img = JPEG('C:/Users/91801/Documents/Personal Docs/MTECH/IIT Kgp Course/DVIP Lab/mini proj/lena.jpg')
    img.img_data
    img.decode()


# OUTPUT:
# Start of Image
# Application Default Header
# Quantization Table
# Quantization Table
# Start of Frame
# Huffman Table
# Huffman Table
# Huffman Table
# Huffman Table
# Start of Scan
# End of Image