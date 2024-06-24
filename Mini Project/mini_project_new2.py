##Group 13 Mini Project

from mini_project_functions import*

#Convert The Image to YUV format
img1 = cv2.imread('C:/Users/91801/Documents/Personal Docs/MTECH/IIT Kgp Course/DVIP Lab/A mini proj/lena.bmp')
img = cv2.cvtColor(img1, cv2.COLOR_BGR2YUV)
file1=open("Encoded.txt", "w+")
dc_array_2 =[]
dc_array_21 =[]
dc_array_22 =[]
r, c = img.shape[:2]

#Zero Padding
if ((r%8 != 0) | (c%8 !=0)):
    r_z = 8- r%8
    c_z = 8- c%8
    img2 = do_zero_padding_1(img, r_z, c_z)

print("IMAGE row = ",r)
print("IMAGE column = ", c)

# storing the block in the temp
temp = np.zeros((8, 8))
temp1 = np.zeros((8, 8))
temp2 = np.zeros((8, 8))
dc_array = []
dc_array1 = []
dc_array2 = []

for i in range(0,  8):
    for j in range(0, 8):
        y, u, v = img[i][j]
        temp[i][j] = y
        temp1[i][j]= u
        temp2[i][j]=v

#To Get First DC TERM of y:
fft_temp = fft.dct(shift_img(temp))
itemp = fft.idct(fft_temp)
dc_term = itemp[0][0]
encoded = encode_dc_2(dc_term)
encoded = str(encoded)

#To Get First DC TERM of u:
fft_temp1 = fft.dct(shift_img(temp1))
itemp1 = fft.idct(fft_temp1)
dc_term1 = itemp1[0][0]
encoded1 = encode_dc_2(dc_term1)
encoded1 = str(encoded1)

#To Get First DC TERM of v:
fft_temp2 = fft.dct(shift_img(temp2))
itemp2 = fft.idct(fft_temp2)
dc_term2 = itemp2[0][0]
encoded2 = encode_dc_2(dc_term2)
encoded2 = str(encoded2)

flag = 1

r= 2*8
c= 2*8

for r1 in range(0, r, 8):
    for c1 in range(0, c, 8):

        print("Running for block (", r1,",", c1,")")

        # extracting block of 8x8
        # for only first block
        for i in range(r1, r1+8):
            for j in range(c1, c1+8):
                y, u, v = img[i][j]
                temp[i-r1][j-c1] = y
                temp1[i-r1][j-c1] = u
                temp2[i - r1][j - c1] = v

        # doing dct transform
        # read image, shift image by -128, divide in block of 8x8
        fft_temp = fft.dct(shift_img(temp))
        itemp = fft.idct(fft_temp)

        fft_temp1 = fft.dct(shift_img(temp1))
        itemp1 = fft.idct(fft_temp1)

        fft_temp2 = fft.dct(shift_img(temp2))
        itemp2 = fft.idct(fft_temp2)


        dc_term = itemp[0][0]
        dc_array.append(dc_term)

        dc_term1 = itemp1[0][0]
        dc_array1.append(dc_term1)

        dc_term2 = itemp2[0][0]
        dc_array2.append(dc_term2)

        # taking the qunatization table
        qtable = get_qtable1()
        qtable1 = get_qtable2()
        ##########################################################

        # storing the quantized image in qimg and storing it in a zigzag fashion
        qimg = np.around(fft_temp / qtable)
        ry = zigzag(qimg)
        ac_coef = symbols(ry)

        qimg1 = np.around(fft_temp1 / qtable1)
        ru = zigzag(qimg1)
        ac_coef1 = symbols(ru)

        qimg2 = np.around(fft_temp2 / qtable1)
        rv = zigzag(qimg2)
        ac_coef2 = symbols(rv)

        if (flag > 1):
        #CODE DC TERM
            dc_array_2.append(dc_array[0])
            for i in range(1, len(dc_array)):
                m = dc_array[i] - dc_array[i - 1]
                dc_array_2.append(m)
            str1 = encode_dc_2(dc_array_2[len(dc_array_2) - 1])
            encoded = str1
            dc_array_2= []

            dc_array_21.append(dc_array1[0])
            for i in range(1, len(dc_array1)):
                m1 = dc_array1[i] - dc_array1[i - 1]
                dc_array_21.append(m1)
            str11 = encode_dc_2(dc_array_21[len(dc_array_21) - 1])
            encoded1 = str11
            dc_array_21 = []

            dc_array_22.append(dc_array2[0])
            for i in range(1, len(dc_array2)):
                m2 = dc_array2[i] - dc_array2[i - 1]
                dc_array_22.append(m2)
            str12 = encode_dc_2(dc_array_22[len(dc_array_22) - 1])
            encoded2 = str12
            dc_array_22 = []

        #CODE AC TERM
        encoded_ac = (encode_ac_2(ac_coef))
        file1.write(encoded)
        for i in encoded_ac:
            file1.write(str(i))


        encoded_ac1 = (encode_ac_2(ac_coef1))
        file1.write(encoded1)
        for i in encoded_ac1:
            file1.write(str(i))


        encoded_ac2 = (encode_ac_2(ac_coef2))
        file1.write(encoded2)
        for i in encoded_ac2:
            file1.write(str(i))
        flag = flag + 1


file1.close()

print("THE ENCODED DATA:")
file1=open("Encoded.txt", "r")
for x in file1:
    m= x
file1.close()

new_x =[]

print(m)

word = ''
count = 0
for i in range(len(m)):
    if (count == 8):
        if (word == '11111111'):
            new_x.append("1111111100000000")
            count = 0
            word = ''
        else:
            new_x.append(word)
            count = 0
            word = ''
    word = word + m[i]
    count= count+1
new_x.append(word)
## To See The Bit Stuffing ##

print(new_x)
print(len(m))
print(len(new_x))


file1=open("Encoded2.txt", "w")
for j in new_x:
    file1.write(j)
file1.close()

print("\n\nFILE CREATED WITH NAME: \tEncoded2.txt \n\n")
file1=open("Encoded2.txt", "r")
for j in file1:
    code = j
file1.close()

print(code)
