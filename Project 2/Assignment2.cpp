#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>


using namespace std;

struct BMP
{
    // header
    char signature[2];
    int fileSize, reserved, dataOffset;

    // info header
    int infoHeaderSize, width, height;
    short int planes, bitsPerPixel;
    int compression, imageSize, xPixelsPerM, yPixelsPerM, colorsUsed, importantColors;

    // color table
    vector<vector<uint8_t>> colorTable;

    // image
    vector<vector<uint8_t>> image, tempimage;
    vector<vector<vector<uint8_t>>> image24Bit, tempimage24Bit;

    int tempheight, tempwidth, tempfilesize;
    // loading image from filename





    
    void ReadImage(string filename)
    {
        ifstream img;
        img.open(filename, ifstream::binary);

        // reading header meta data
        img.read(signature, 2);
        if (signature[0] != 'B')
        {
            cout << "format declined" << endl;
            return;
        }

        img.read(reinterpret_cast<char *>(&fileSize), 4);  //loads 4 size from file(img) to variable FileSize
        img.read(reinterpret_cast<char *>(&reserved), 4);
        img.read(reinterpret_cast<char *>(&dataOffset), 4);

        // reading info header data
        img.read(reinterpret_cast<char *>(&infoHeaderSize), 4);
        img.read(reinterpret_cast<char *>(&width), 4);
        img.read(reinterpret_cast<char *>(&height), 4);
        img.read(reinterpret_cast<char *>(&planes), 2);
        img.read(reinterpret_cast<char *>(&bitsPerPixel), 2);
        img.read(reinterpret_cast<char *>(&compression), 4);
        img.read(reinterpret_cast<char *>(&imageSize), 4);
        img.read(reinterpret_cast<char *>(&xPixelsPerM), 4);
        img.read(reinterpret_cast<char *>(&yPixelsPerM), 4);
        img.read(reinterpret_cast<char *>(&colorsUsed), 4);
        img.read(reinterpret_cast<char *>(&importantColors), 4);
        
       // cout<<"Type of " <<typeid(signature).name()<<"\n\n";

        // reading color table
        colorTable.resize(colorsUsed, vector<uint8_t>(4));
        for (int i = 0; i < colorsUsed; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                img.read(reinterpret_cast<char *>(&colorTable[i][j]), 1);
            }
        }

        // reading image data
        if (bitsPerPixel == 8)
        {
            image.resize(height, vector<uint8_t>(width));
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    img.read(reinterpret_cast<char *>(&image[i][j]), bitsPerPixel / 8);
                }
            }
        }
        else if (bitsPerPixel == 24)
        {
            image24Bit.resize(height, vector<vector<uint8_t>>(width, vector<uint8_t>(3)));
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        img.read(reinterpret_cast<char *>(&image24Bit[i][j][k]), 1);
                    }
                }
            }
        }

        // closing file
        img.close();
    }





    // show all the data in headers
    void ShowHeaderInfo()
    {
        // headers
        cout << "signature - " << signature << endl;
        cout << "fileSize - " << fileSize << endl;
        cout << "reserved - " << reserved << endl;
        cout << "dataoffset - " << dataOffset << endl;

        // info headers
        cout << "infoHeaderSize - " << infoHeaderSize << endl;
        cout << "width - " << width << endl;
        cout << "height - " << height << endl;
        cout << "planes - " << planes << endl;
        cout << "bitsPerPixel - " << bitsPerPixel << endl;
        cout << "compression - " << compression << endl;
        cout << "imageSize - " << imageSize << endl;
        cout << "xPixelsPerM - " << xPixelsPerM << endl;
        cout << "yPixelsPerM - " << yPixelsPerM << endl;
        cout << "colorsUsed - " << colorsUsed << endl;
        cout << "importantColors - " << importantColors << endl;
    }




    // display the color table / palette
    void showColorTable()
    {
        for (int i = 0; i < colorsUsed; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                cout << colorTable[i][j] << " ";
            }
            cout << endl;
        }
    }




    // show image data
    void showImageData()
    {
        if (bitsPerPixel == 8)
        {

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    cout << (int)image[i][j] << " ";
                }
                cout << endl;
            }
        }
        else if (bitsPerPixel == 24)
        {

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        cout << (int)image24Bit[i][j][k] << "-";
                    }
                    cout << " ";
                }
                cout << endl;
            }
        }
    }





    

    void FlipDiagonal()
    {
        if (bitsPerPixel == 8)
        {
            tempimage = image;
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j <= i; j++)
                {
                    tempimage[i][j] = image[j][i];
                    tempimage[j][i] = image[i][j];
                }
            }
            tempheight = width;
            tempwidth = height;
            tempfilesize = fileSize;
            writetofile("FlipDiagonal_gray.bmp");
        }
        else if (bitsPerPixel == 24)
        {
            tempimage24Bit = image24Bit;
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j <= i; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        tempimage24Bit[i][j][k] = image24Bit[j][i][k];
                        tempimage24Bit[j][i][k] = image24Bit[i][j][k];
                    }
                }
            }
            tempheight = height;
            tempwidth = width;
            tempfilesize = fileSize;
            writetofile("FlipDiagonalcolor.bmp");
        }
    }





    void Rotate90()
    {
        if (bitsPerPixel == 8)
        {
            tempimage = image;
            for (int j = width - 1; j >= 0; j--)
            {
                for (int i = 0; i < height; i++)
                {
                    tempimage[i][width - 1 - j] = image[j][i];
                }
            }
            tempheight = width;
            tempwidth = height;
            tempfilesize = fileSize;
            writetofile("Rotate90_gray.bmp");
        }
        else if (bitsPerPixel == 24)
        {
            tempimage24Bit = image24Bit;
            for (int j = width - 1; j >= 0; j--)
            {
                for (int i = 0; i < height; i++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        tempimage24Bit[i][width - 1 - j][k] = image24Bit[j][i][k];
                    }
                }
            }
            tempheight = width;
            tempwidth = height;
            tempfilesize = fileSize;
            writetofile("Rotate90_color.bmp");
        }
    }




    void transpose()
    {
        int temph = height;
        height = width;
        width = temph;

        if (bitsPerPixel == 24)
        {

            vector<vector<vector<uint8_t>>> temp(height, vector<vector<uint8_t>>(width, vector<uint8_t>(3)));
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    temp[j][i] = tempimage24Bit[i][j];
                }
            }
            tempimage24Bit = temp;
            writetofile("Rotate45_color.bmp");
        }
        else if (bitsPerPixel == 8)
        {
            vector<vector<uint8_t>> temp(height, vector<uint8_t>(width, 0));

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    temp[j][i] = tempimage[i][j];
                }
            }
            tempimage = temp;
            writetofile("Rotate45_gray.bmp");
        }
    }







    void Rotate45()
    {
        if (bitsPerPixel == 8)
        {
            vector<vector<uint8_t>> temp(height, vector<uint8_t>(width, 0));

            float scale = 1;
            for (int i = 0; i < height; i++)
            {
                float x2 = i - height / 2.0;
                for (int j = 0; j < width; j++)
                {
                    float y2 = j - width / 2.0;
                    int y = width / 2.0 + (x2 - y2) * scale;
                    int x = height / 2.0 + (x2 + y2) * scale;
                    if (x < height && x >= 0 && y < width && y >= 0)
                    {
                        temp[i][j] = image[x][y];
                    }
                }
            }
            tempimage = temp;
        }
        else if (bitsPerPixel == 24)
        {
            vector<vector<vector<uint8_t>>> temp(height, vector<vector<uint8_t>>(width, vector<uint8_t>(3)));

            float scale = 1;
            for (int i = 0; i < height; i++)
            {
                float x2 = i - height / 2.0;
                for (int j = 0; j < width; j++)
                {
                    float y2 = j - width / 2.0;
                    int y = width / 2.0 + (x2 - y2) * scale;
                    int x = height / 2.0 + (x2 + y2) * scale;
                    if (x < height && x >= 0 && y < width && y >= 0)
                    {
                        temp[i][j] = image24Bit[x][y];
                    }
                }
            }
            tempimage24Bit = temp;
        }
        transpose();
    }



    //scaling
    void scaling()
    {

        if (bitsPerPixel == 8)
        {
            vector<vector<uint8_t>> temp2(height * 2, vector<uint8_t>(width * 2, 0));
            tempimage.resize(2 * height, vector<uint8_t>(2 * width));
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    temp2[2 * i][2 * j] = image[i][j];
                    temp2[2 * i][2 * j + 1] = image[i][j];
                    temp2[2 * i + 1][2 * j] = image[i][j];
                    temp2[2 * i + 1][2 * j + 1] = image[i][j];
                }
            }
            tempimage = temp2;
            tempheight = 2 * height;
            tempwidth = 2 * width;
            tempfilesize = 4 * fileSize;
            writetofile("scaling_gray.bmp");
        }
        else if (bitsPerPixel == 24)
        {
            vector<vector<vector<uint8_t>>> temp3(height * 2, vector<vector<uint8_t>>(width * 2, vector<uint8_t>(3, 0)));
            tempimage24Bit = temp3;

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        tempimage24Bit[2 * i][2 * j][k] = image24Bit[i][j][k];
                        tempimage24Bit[2 * i][2 * j + 1][k] = image24Bit[i][j][k];
                        tempimage24Bit[2 * i + 1][2 * j][k] = image24Bit[i][j][k];
                        tempimage24Bit[2 * i + 1][2 * j + 1][k] = image24Bit[i][j][k];
                    }
                }
            }
            tempheight = 2 * height;
            tempwidth = 2 * width;
            tempfilesize = 4 * fileSize;
            writetofile("scaling_color.bmp");
        }
    }

 //scaling
    void scaling_small()
    {

        if (bitsPerPixel == 8)
        {
            vector<vector<uint8_t>> temp2(height * 0.5, vector<uint8_t>(width * 0.5, 0));
            tempimage.resize(0.5 * height, vector<uint8_t>(0.5 * width));
            for (int i = 0; i < height*0.5; i++)
            {
                for (int j = 0; j < width*0.5; j++)
                {
                    temp2[ i][ j] = image[i*2][j*2];
                   
                }
            }
            tempimage = temp2;
            tempheight = 0.5 * height;
            tempwidth = 0.5 * width;
            tempfilesize = 0.25 * fileSize;
            writetofile("scaling_gray_small.bmp");
        }
        else if (bitsPerPixel == 24)
        {
            vector<vector<vector<uint8_t>>> temp3(height * 0.5, vector<vector<uint8_t>>(width * 0.5, vector<uint8_t>(3, 0)));
            tempimage24Bit = temp3;

            for (int i = 0; i < width*0.5; i++)
            {
                for (int j = 0; j < height*0.5; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        tempimage24Bit[ i][j][k] = image24Bit[i*2][j*2][k];
                      
                    }
                }
            }
            tempheight = 0.5 * height;
            tempwidth = 0.5 * width;
            tempfilesize = 0.25 * fileSize;
            writetofile("scaling_color_small.bmp");
        }
    }

void Interpolation()
    {		
    		float x=2.2;
    		float f= 1/x;
    		int a1, b1, c, d ;
    		float pixel1, pixel2;
    		int pixel3;

        if (bitsPerPixel == 8)
        {
            vector<vector<uint8_t>> temp2(height * x, vector<uint8_t>(width * x, 0));
            tempimage.resize(x * height, vector<uint8_t>(x * width));
            for (int i = 0; i < height*floor(x); i++)
            {
                for (int j = 0; j < width*floor(x); j++)
                {	 i=0; j=0;
					if ((i==0)||(j==0))
                	{
                	a1 = ceil(i/x);
                	b1 = ceil(j/x);
                	c = a1 - ceil(i/x);
                	d= b1 - ceil(j/x);
                	
                	pixel1 = c*((int)image[a1][b1])+(1-c)*((int)image[a1][b1]);
                	pixel2 = c*((int)image[a1][b1])+(1-c)*((int)image[a1][b1]);
                	pixel3 = d*pixel2 + (1-d)*pixel1;                	
                	
                            	
                    temp2[ i][ j] =((uint8_t)pixel3);				
							
					}
					
					else {
					
                	a1 = ceil(i/x);
                	b1 = ceil(j/x);
                	c = a1 - ceil(i/x);
                	d= b1 - ceil(j/x);
                	
                	pixel1 = c*((int)image[a1][b1])+(1-c)*((int)image[a1][b1-1]);
                	pixel2 = c*((int)image[a1-1][b1-1])+(1-c)*((int)image[a1-1][b1]);
                	pixel3 = d*pixel2 + (1-d)*pixel1;             	
                	
                    temp2[ i][ j] = (uint8_t)pixel3;
               	}
                }
            }
            tempimage = temp2;
            tempheight = floor(x) * height;
            tempwidth = floor(x) * width;
            tempfilesize = floor(x)*floor(x)* fileSize;
            writetofile("Biliniar_image_2.bmp");
        }
        else if (bitsPerPixel == 24)
        {
            vector<vector<vector<uint8_t>>> temp3(height * x, vector<vector<uint8_t>>(width * x, vector<uint8_t>(3, 0)));
            tempimage24Bit = temp3;
            
              for (int i = 0; i < (width*floor(x)); i++)
            {
                for (int j = 0; j < (height*floor(x)); j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                    
                    	if ((i==0)||(j==0))
                    	
                    	{
                    	a1 = ceil(i/x); 
                		b1 = ceil(j/x);
                		c = a1 - ceil(i/x);
                		d= b1 - ceil(j/x);
                	
                		pixel1 = c*(int)image24Bit[a1][b1][k]+(1-c)*(int)image24Bit[a1][b1][k];
                		pixel2 = c*(int)image24Bit[a1][b1][k]+(1-c)*(int)image24Bit[a1][b1][k];
                		pixel3 = (d*pixel2 + (1-d)*pixel1);  
									              	
                	
                    	tempimage24Bit[ i][ j][k] = (uint8_t)pixel3;
                    		
						}
						
						else {
						
					
						
                        a1 = ceil(i/x);
                		b1 = ceil(j/x);
                		c = a1 - ceil(i/x);
                		d= b1 - ceil(j/x);
                	
                		pixel1 = c*(int)image24Bit[a1][b1][k]+(1-c)*(int)image24Bit[a1][b1-1][k];
                		pixel2 = c*(int)image24Bit[a1-1][b1-1][k]+(1-c)*(int)image24Bit[a1-1][b1][k];
                		pixel3 = (d*pixel2 + (1-d)*pixel1);  
						           	
                	
                    	tempimage24Bit[ i][ j][k] = (uint8_t)pixel3;                  	
								              	
                	
                    
                    }
                      
                    }
                }
            }
            tempheight = ( floor(x)* height);
            tempwidth = ( floor(x) * width);
            tempfilesize = floor(x) * floor(x)* fileSize;
            writetofile("Biliniar_image_1.bmp");
        }
        
        
    }

	



    // write to another bitmap image
    void writetofile(string filename)
    {
        ofstream img;
        img.open(filename, ofstream::binary);

        // writing headers
        img.write(signature, 2);
        img.write(reinterpret_cast<char *>(&tempfilesize), 4);
        img.write(reinterpret_cast<char *>(&reserved), 4);
        img.write(reinterpret_cast<char *>(&dataOffset), 4);

        // writing info headers
        img.write(reinterpret_cast<char *>(&infoHeaderSize), 4);
        img.write(reinterpret_cast<char *>(&tempwidth), 4);
        img.write(reinterpret_cast<char *>(&tempheight), 4);
        img.write(reinterpret_cast<char *>(&planes), 2);
        img.write(reinterpret_cast<char *>(&bitsPerPixel), 2);
        img.write(reinterpret_cast<char *>(&compression), 4);
        img.write(reinterpret_cast<char *>(&imageSize), 4);
        img.write(reinterpret_cast<char *>(&xPixelsPerM), 4);
        img.write(reinterpret_cast<char *>(&yPixelsPerM), 4);
        img.write(reinterpret_cast<char *>(&colorsUsed), 4);
        img.write(reinterpret_cast<char *>(&importantColors), 4);

        // writing color table
        for (int i = 0; i < colorsUsed; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                img.write(reinterpret_cast<char *>(&colorTable[i][j]), 1);
            }
        }

        // writing image data
        if (bitsPerPixel == 8)
        {
            // pixelarray.resize(width, vector<unsigned char>(height));
            for (int i = 0; i < tempwidth; i++)
            {
                for (int j = 0; j < tempheight; j++)
                {
                    img.write(reinterpret_cast<char *>(&tempimage[i][j]), bitsPerPixel / 8);
                }
            }
        }
        else if (bitsPerPixel == 24)
        {
            //image24Bit.resize(width, vector<vector<unsigned char>>(height, vector<unsigned char>(3)));
            for (int i = 0; i < tempwidth; i++)
            {
                for (int j = 0; j < tempheight; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        img.write(reinterpret_cast<char *>(&tempimage24Bit[i][j][k]), 1);
                    }
                }
            }
        }

        // success message
        cout << "File writing operation successful" << endl;

        // closing file
        img.close();
    }
};




// main function


int main()
{

    while (1)
    {
        string img;
        cout << "Enter the file name with extension (enter 'exit', if you don't have any) : ";
        cin >> img;
        if (img == "exit")
        {
            break;
        }
        else
        {

            BMP image;
            image.ReadImage(img);
            image.ShowHeaderInfo();
          	image.FlipDiagonal();
          	image.Rotate90();
          	image.Rotate45();
		  	image.scaling();
            image.scaling_small();
            image.Interpolation();
            
        }
    }

    return 0;
}
