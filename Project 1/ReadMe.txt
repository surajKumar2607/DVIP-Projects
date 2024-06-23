READ ME:

Done by : Suraj Kumar
Roll no. 22EC65R14

Working with the code:

1. put the name of the output new file name in the main program with the channel filter code
2. output will be the image of the corn filtered with the channel (red, blue or green)
3. Function READBMP(file_in_bmp) is used to read the file name in .bmp format and display the header data 
4. Function WRITEBMP(output_file_name, Image_data_header, Image_data_pixel) is used to write the new file, with name output_file_name
	with data of header as Image_data_header and pixel data as Image_data_pixel
5. Function MANUPULATION(file_name, ch, file_output_name) is used to filter the color channel (red, blue or green) based on ch,
	takes the input file name as file_name and generate output file as file_output_name
