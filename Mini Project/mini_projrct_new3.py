from mini_project_functions import*

#Convert The Image to YUV format
img1 = cv2.imread('C:/Users/91801/Documents/Personal Docs/MTECH/IIT Kgp Course/DVIP Lab/A mini proj/lena.bmp')
img = cv2.cvtColor(img1, cv2.COLOR_BGR2YUV)
file1=open("Encoded.txt", "w+")
dc_array_2 =[]
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

for i in range(0,  8):
    for j in range(0, 8):
        y, u, v = img[i][j]
        temp[i][j] = y
        temp1[i][j]= u
        temp2[i][j]=v

r= 2*8
c= 2*8


#To Get First BLOCK of y:
fft_temp = fft.dct(shift_img(temp))
itemp = fft.idct(fft_temp)
dc_term = itemp[0][0]
encoded = encode_dc_2(dc_term)
flag = 1
file1.write(str(encoded))

r11,c11= 0,0

print("Running for block (", r11,",", c11,")")

# extracting block of 8x8
# for only first block
for i in range(r11, r11+8):
    for j in range(c11, c11+8):
        y, u, v = img[i][j]
        temp[i-r11][j-c11] = y

# doing dct transform
# read image, shift image by -128, divide in block of 8x8
fft_temp = fft.dct(shift_img(temp))
itemp = fft.idct(fft_temp)

dc_term = itemp[0][0]
dc_array.append(dc_term)

# taking the qunatization table
qtable = get_qtable1()

# storing the quantized image in qimg
qimg = np.around(fft_temp / qtable)

# storing it in a zigzag fashion
r = zigzag(qimg)
ac_coef = symbols(r)

#CODE AC TERM
encoded_ac = encode_ac_2(ac_coef)
for i in encoded_ac:
    file1.write(str(i))
flag =  flag+1

for r1 in range(0, r, 8):
    for c1 in range(0, c, 8):
        if(r1 !=0 and c1!=0 ):
            print("Running for block (", r1,",", c1,")")
            # extracting block of 8x8
            # for only first block
            for i in range(r1, r1+8):
                for j in range(c1, c1+8):
                    y, u, v = img[i][j]
                    temp[i-r1][j-c1] = y

            # doing dct transform
            # read image, shift image by -128, divide in block of 8x8
            fft_temp = fft.dct(shift_img(temp))
            itemp = fft.idct(fft_temp)

            dc_term = itemp[0][0]
            dc_array.append(dc_term)

            # taking the qunatization table
            qtable = get_qtable1()

            # storing the quantized image in qimg
            qimg = np.around(fft_temp / qtable)

            # storing it in a zigzag fashion
            r = zigzag(qimg)
            ac_coef = symbols(r)

            if (flag > 1):
            #CODE DC TERM

                dc_array_2.append(dc_array[0])
                for i in range(1, len(dc_array)):
                    m = dc_array[i] - dc_array[i - 1]
                    dc_array_2.append(m)
                str1 = encode_dc_2(dc_array_2[len(dc_array_2) - 1])
                file1.write(str(str1))
                dc_array_2= []

            #CODE AC TERM
            encoded_ac = encode_ac_2(ac_coef)
            for i in encoded_ac:
                file1.write(str(i))
            flag =  flag+1

print("\n\nFILE CREATED WITH NAME: \tEncoded.txt \n\n")
file1.close()
print("THE ENCODED DATA:")
file1=open("Encoded.txt", "r")
for x in file1:
  print(x)
file1.close()