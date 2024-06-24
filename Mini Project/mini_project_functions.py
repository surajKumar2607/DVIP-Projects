import cv2
import numpy as np
import matplotlib.pyplot as plt
from scipy import fft
import openpyxl
import math

def shift_img(img):
    h,w = img.shape
    for i in range(h):
        for j in range(w):
            img[i][j] = img[i][j] - 128
    return img

#img and qtable are both of same size and are numpy arrays
def quant_img(img,qtable):
    h,w = img.shape
    for i in range(h):
        for j in range(w):
            img[i][j] = img[i][j] / qtable[i][j]
    return img

# returns the quantization table
def get_qtable1():
    qt_obj = open('qtable.txt',"r")
    qt = []
    for line in qt_obj:
        qt.append(line.split())
    for i in range(8):
        for j in range(8):
            qt[i][j] = int(qt[i][j])
    return(np.array(qt))

def get_qtable2():
    qt_obj = open('qtable1.txt',"r")
    qt = []
    for line in qt_obj:
        qt.append(line.split())
    for i in range(8):
        for j in range(8):
            qt[i][j] = int(qt[i][j])
    return(np.array(qt))

# returns a numpy array reordered in zig zag fashion

def zigzag(input):
    h = 0
    v = 0

    vmin = 0
    hmin = 0

    vmax = input.shape[0]
    hmax = input.shape[1]

    i = 0

    output = np.zeros(( vmax * hmax))

    while ((v < vmax) and (h < hmax)):
    	
        if ((h + v) % 2) == 0:                 # going up
            
            if (v == vmin):
            	#print(1)
                output[i] = input[v, h]        # if we got to the first line

                if (h == hmax):
                    v = v + 1
                else:
                    h = h + 1                        

                i = i + 1

            elif ((h == hmax -1 ) and (v < vmax)):   # if we got to the last column
            	#print(2)
            	output[i] = input[v, h] 
            	v = v + 1
            	i = i + 1

            elif ((v > vmin) and (h < hmax -1 )):    # all other cases
            	#print(3)
            	output[i] = input[v, h] 
            	v = v - 1
            	h = h + 1
            	i = i + 1

        
        else:                                    # going down

        	if ((v == vmax -1) and (h <= hmax -1)):       # if we got to the last line
        		#print(4)
        		output[i] = input[v, h] 
        		h = h + 1
        		i = i + 1
        
        	elif (h == hmin):                  # if we got to the first column
        		#print(5)
        		output[i] = input[v, h] 

        		if (v == vmax -1):
        			h = h + 1
        		else:
        			v = v + 1

        		i = i + 1

        	elif ((v < vmax -1) and (h > hmin)):     # all other cases
        		#print(6)
        		output[i] = input[v, h] 
        		v = v + 1
        		h = h - 1
        		i = i + 1




        if ((v == vmax-1) and (h == hmax-1)):          # bottom right element
        	#print(7)        	
        	output[i] = input[v, h] 
        	break

    #print ('v:',v,', h:',h,', i:',i)
    return output

def inverse_zigzag(input, vmax, hmax):
    h = 0
    v = 0

    vmin = 0
    hmin = 0

    output = np.zeros((vmax, hmax))

    i = 0
    while ((v < vmax) and (h < hmax)): 
            #print ('v:',v,', h:',h,', i:',i)   	
            if ((h + v) % 2) == 0:                 # going up
        
                    if (v == vmin):
                            #print(1)
                            
                            output[v, h] = input[i]        # if we got to the first line

                            if (h == hmax):
                                    v = v + 1
                            else:
                                    h = h + 1                        

                            i = i + 1

                    elif ((h == hmax -1 ) and (v < vmax)):   # if we got to the last column
                            #print(2)
                            output[v, h] = input[i] 
                            v = v + 1
                            i = i + 1

                    elif ((v > vmin) and (h < hmax -1 )):    # all other cases
                            #print(3)
                            output[v, h] = input[i] 
                            v = v - 1
                            h = h + 1
                            i = i + 1

    
            else:                                    # going down

                    if ((v == vmax -1) and (h <= hmax -1)):       # if we got to the last line
                            #print(4)
                            output[v, h] = input[i] 
                            h = h + 1
                            i = i + 1
    
                    elif (h == hmin):                  # if we got to the first column
                            #print(5)
                            output[v, h] = input[i] 
                            if (v == vmax -1):
                                    h = h + 1
                            else:
                                    v = v + 1
                            i = i + 1
                                            
                    elif((v < vmax -1) and (h > hmin)):     # all other cases
                            output[v, h] = input[i] 
                            v = v + 1
                            h = h - 1
                            i = i + 1




            if ((v == vmax-1) and (h == hmax-1)):          # bottom right element
                    #print(7)        	
                    output[v, h] = input[i] 
                    break

    return output
def category(x):
    x = abs(x)  
    return math.floor(math.log2(x)+ 1)


def read_cell_value(x,y):
    dataframe = openpyxl.load_workbook("huffman_table.xlsx")
    dataframe1 = dataframe.active
    cell_obj = dataframe1.cell(row = x,column = y)
    return cell_obj.value

def int_to_bits(x):
    c = category(x)
    v = read_cell_value(c+168,3)
    return len(str(v))

# return list of tuples of intermediate symbols
def symbols(a):
    count_zero = 0
    temp =[]
    for i in range(1,len(a)):
        if a[i] != 0:
            sym1 = [count_zero,int_to_bits(a[i])]
            count_zero = 0
            temp.append([sym1,a[i]])            
        else:
            count_zero += 1
    return temp

#encoding ac from turple 3

def encode_ac_1(list1):
    temp=[]
    #compare the first element with the column 1 and col 2 and go from row 2->165
    x = len(list1)
    for i in range(x):
        row=2
        while((row<165)& (i<x)):
            if  read_cell_value(row,1) == list1[i][0][0]:
                 if read_cell_value(row,2) == list1[i][0][1]:
                     x1 = read_cell_value(row,3)
                     temp.append(x1)
                     row= 166
            row= row+1
    return temp


def onesComplement(n):
    # Find number of bits in
    # the given integer

    number_of_bits = (int)(math.floor(math.log(n) /
                                      math.log(2))) + 1;

    # XOR the given integer with poe(2,
    # number_of_bits-1 and print the result
    return ((1 << number_of_bits) - 1) ^ n;

def decimalToBinary(n):
    return bin(n).replace("0b", "")


def encode_ac_2(list1):
    temp=[]
    #compare the first element with the column 1 and col 2 and go from row 2->165
    x = len(list1)
    for i in range(x):
        row=2
        while((row<165)& (i<x)):
            if  read_cell_value(row,1) == list1[i][0][0]:
                 if read_cell_value(row,2) == list1[i][0][1]:
                     x1 = read_cell_value(row,3)
                     # Shift the x1 by n number of bits then convert the num into bin and add it
                     if (list1[i][1]< 0):
                         k = list1[i][1]
                         k=int(k* (-1))
                         d= int(decimalToBinary(onesComplement(k)))
                         x1 = int(x1)
                         k= x1 * pow(10, countTotalBits(k))
                         x1= k + d


                     elif (list1[i][1] >= 0):
                         k = list1[i][1]
                         k = int(k)
                         d = int(decimalToBinary(k))
                         x1 = int(x1)
                         k = x1 * pow(10, countTotalBits(k))
                         x1 = k + d
                     temp.append(x1)
                     row= 166
            row= row+1
    return temp


# Function to count total bits in a number

def countTotalBits(num):
    # convert number into it's binary and
    # remove first two characters 0b.
    binary = bin(num)[2:]
    return(len(binary))

#Zero Padding
def do_zero_padding_1(img, r_z, c_z):
    img = np.array(img)
    x, y = img.shape[:2]
    x1, y1 = r_z, c_z
    img2 = np.array([[[0] *3 ] * (x+x1)]*(y+y1))
    for c in range(y):
        for r in range(x):
            img2[r][c][0]= img[r][c][0]
            img2[r][c][1]= img[r][c][1]
            img2[r][c][2]=img[r][c][2]
    return img2



def encode_dc_2(num):
    if (num< 0):
        k = num
        k=int(k* (-1))
        p = countTotalBits(k)

        o_c= pow(2,(p))- 1-k
        s1 = "0"
        s2 = str(p)
        s3 = "b"
        str1 = format(o_c, s1 + s2 + s3)
        # str2 = huffman code for read_cell_value(168,2) -> read_cell_value(179,2)

        for i in range (12):
            if (read_cell_value(168+i,2)==p):
                str2 = str(read_cell_value(168+i, 3))

        return (str2+str1)
        #DC_Tuple = (p, d)
    if (num>=0):
        k= int(num)
        p = countTotalBits(k)

        s1 = "0"
        s2 = str(p)
        s3 = "b"
        str1 = format(k, s1 + s2 + s3)
        for i in range(12):
            if (read_cell_value(168 + i, 2) == p):
                str2 = str(read_cell_value(168 + i, 3))

        return (str2 + str1)

