#include<opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include<complex>



using namespace std;
using namespace cv;


//Creating Complex vector, 2D for FFT and IFFT
vector<vector<complex<double>>> FFTt;
vector<vector<complex<double>>> FFTf;
vector<vector<complex<double>>> IFFTt; //for inverse FFT Row
vector<vector<complex<double>>> IFFTf; //for inverse FFT Col



#define pi 3.14159265
Mat image = imread("C:/Users/91801/Documents/Personal Docs/MTECH/IIT Kgp Course/VDIP Lab/Assignment 3/Coding/lake.jpg");


//ifft utility function
void fun_ifft_butterfly(vector<complex<double>>& g, string dir)
{
    int N = g.size();    
    vector<complex<double>> v;
    double Rdp[2000], Idp[2000] = { 0 };
    double TRdp[2000], TIdp[2000];
    int i, twid = 1, pow;
    double rw, iw; 
    
    //initializing the real and imaginary value of w
    //Initialize the real part to the samples
    for (i = 0; i < N; i++)
    {
        Rdp[i] = real(g[i]);
        Idp[i] = imag(g[i]);
    }

    i = N;
    while ((i / 2) != 0)
    {
        i = i / 2;
        int c1 = 0, c2 = 0, state = 1;

        //update the temporary arrays at every stage of fft
        for (int k = 0; k < N; k++)
        {
            TRdp[k] = Rdp[k];
            TIdp[k] = Idp[k];
        }

        for (int k = 0; k < N; k++)
        {
            if (state)
            {
                Rdp[k] = TRdp[k] + TRdp[k + i];
                Idp[k] = TIdp[k] + TIdp[k + i];
                c1++;
                pow = 0;
                if (c1 == i)
                {
                    state = 0;
                    c1 = 0;
                }
            }
            else {

                rw = cos(2 * pi * (-pow) / N);
                iw = sin(2 * pi * (-pow) / N);

                Rdp[k] = (TRdp[k - i] - TRdp[k]) * rw + (TIdp[k] - TIdp[k - i]) * iw;
                Idp[k] = (TRdp[k - i] - TRdp[k]) * iw + (TIdp[k - i] - TIdp[k]) * rw;

                c2++;
                pow = pow + twid;
                if (c2 == i) {
                    state = 1;
                    c2 = 0;
                }
            }
        }
        twid = twid * 2;
     }

    //bit reversal display
    for (i = 0; i < N; i++)
    {
        int B = i, BR = 0, b2;

        for (int j = 0; j <= log2(N) - 1; j++)
        {
            b2 = B / 2;
            BR = 2 * BR + B - (2 * b2);
            B = b2;

        }
        //cout<< BR<<"  "<<Rdp[BR] << "+ j" << Idp[BR]<<endl;
        complex<double> c(Rdp[BR] / N, Idp[BR] / N);
        v.push_back(c);


    }

    if (dir == "row")
        IFFTt.push_back(v);
    else IFFTf.push_back(v);

}


// fft utility function 
void fun_fft_butterfly(vector<complex<double>>& g, string dir)
{

    int N = g.size();
    vector<complex<double>> v;
    double Rdp[2000], Idp[2000] = { 0 }, TRdp[2000], TIdp[2000];
    int i, twid = 1, pow;
    double rw, iw; //initializing the real and imaginary value of w
    //Initialize the real part to the samples
    for (i = 0; i < N; i++)
    {
        Rdp[i] = real(g[i]);
        Idp[i] = imag(g[i]);
    }
    i = N;
    while ((i / 2) != 0)
    {
        i = i / 2;
        int c1 = 0, c2 = 0, state = 1;

        //update the temporary arrays at every stage of fft

        for (int k = 0; k < N; k++)
        {
            TRdp[k] = Rdp[k];
            TIdp[k] = Idp[k];
        }


        for (int k = 0; k < N; k++)
        {


            if (state)
            {
                Rdp[k] = TRdp[k] + TRdp[k + i];
                Idp[k] = TIdp[k] + TIdp[k + i];
                c1++;
                pow = 0;
                if (c1 == i)
                {
                    state = 0;
                    c1 = 0;
                }

            }
            else {

                rw = cos(2 * pi * pow / N);
                iw = sin(2 * pi * pow / N);

                Rdp[k] = (TRdp[k - i] - TRdp[k]) * rw + (TIdp[k] - TIdp[k - i]) * iw;
                Idp[k] = (TRdp[k - i] - TRdp[k]) * iw + (TIdp[k - i] - TIdp[k]) * rw;

                c2++;
                pow = pow + twid;
                if (c2 == i) {
                    state = 1;
                    c2 = 0;
                }

            }

        }
        twid = twid * 2;


    }
    //bit reversal display
    vector<int> fft;
    for (i = 0; i < N; i++)
    {
        int B = i, BR = 0, b2;

        for (int j = 0; j <= log2(N) - 1; j++)
        {
            b2 = B / 2;
            BR = 2 * BR + B - (2 * b2);
            B = b2;

        }
        //cout<< BR<<"  "<<Rdp[BR] << "+ j" << Idp[BR]<<endl;
        complex<double> c(Rdp[BR], Idp[BR]);
        v.push_back(c);


    }

    if (dir == "row")
        FFTt.push_back(v);
    else FFTf.push_back(v);

}


//utility function to view the fft of matrix
void disfft()
{

    for (int i = 0; i < FFTf.size(); i++)
    {
        for (int j = 0; j < FFTf[i].size(); j++)
            cout << real(FFTf[i][j]) << "+ j" << imag(FFTf[i][j]) << endl;
        cout << "/////" << endl;
    }
    FFTt.clear();
}


//**************************************************************************//

void Multiply_minus_one_power()
{
    int n, column, t;
    n = image.size[0];
    column = image.size[1];

    for (int j = 0; j < n; j++)
    {

        for (int i = 0; i < column; i++)
        {
            t = image.at<Vec3b>(j, i)[0];
            t = t * pow(-1, i + j);
            image.at<Vec3b>(j, i)[0] = t;
            image.at<Vec3b>(j, i)[1] = t;
            image.at<Vec3b>(j, i)[2] = t;
        }

    }
}

//*****************************************************//

void complex_conjugate(vector<vector<complex<double>>>& g)
{
    for (int i = 0; i < g.size(); i++)
    {
        for (int j = 0; j < g[0].size(); j++)
            g[i][j] = conj(g[i][j]);
    }
}

//********************************************************//

void Multipy_real_ifft(vector<vector<complex<double>>>& g)
{
    int t;
    for (int i = 0; i < g.size(); i++)
    {
        for (int j = 0; j < g[0].size(); j++)
        {
            t = pow(-1, i + j) * real(g[i][j]);
            complex<double> c(t, imag(g[i][j]));
            g[i][j] = c;
        }
    }
    
}

//**************************MAIN********************************************************************************************************//
int main()
{
    vector<complex<double>> inp;
    double sum = 0;
    int n, pad, t, column, operation;
    string row = "row", col = "col";

    cout << "Enter 0:\nfor Visualization of Magnitude and phase spectrum of that image\n\n"
        << "Enter 1:\n 1.for Multiply the image by -1 power x+y"
        << "\n 2. Compute the FFT"
        << "\n 3. Compute the Complex Conjugate of resultant FFT."
        << "\n 4. Compute Inverse FFT(iFFT) of the Complex Conjugate."
        << "\n 5. Multiply the real part of the result by -1 x +y\n\n";

    cin >> operation;
   imshow("image", image);
    waitKey(0);


    if (operation)
        Multiply_minus_one_power();


//******************************************************//

    //Row fft
    
    //Padding
    n = image.size[0]; //row
    column = image.size[1]; //column

    if (ceil(log2(n)) == floor(log2(n)))
        pad = n;   
    else 
        pad = pow(2, int(log2(n) + 1));

    for (int j = 0; j < pad; j++)
    {
        for (int i = 0; i < column; i++)
        {
            if (i < n)
            {
                t = image.at<Vec3b>(j, i)[0];                
                complex<double> c(t, 0);
                inp.push_back(c);
            }
            else {
                complex<double> c(0, 0); //pad all for more than n
                inp.push_back(c);
            }
        }

        fun_fft_butterfly(inp, row);
        inp.clear();
    }

    // column fft
    for (int i = 0; i < FFTt[0].size(); i++)
    {
        for (int j = 0; j < FFTt.size(); j++)
        {
            inp.push_back(FFTt[j][i]);
        }
        fun_fft_butterfly(inp, col);
        inp.clear();
    }
    
    
 //************************************************************//
       
    //transposed FFTf
    //normalize FFT
    int factor = 255 / log10f(abs(FFTf[0][0]));
    cout << "\n\n\n" << factor << "\n\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < column; j++)
        {
            image.at<Vec3b>(i, j)[0] = int(log10f(abs(FFTf[j][i])) * factor);
            image.at<Vec3b>(i, j)[1] = int(log10f(abs(FFTf[j][i])) * factor);
            image.at<Vec3b>(i, j)[2] = int(log10f(abs(FFTf[j][i])) * factor);
        }
    }
    //show image FFT;
    imshow("fft_mag_plot", image);
    imwrite("fft_mag_plot_lake.jpg", image);
    waitKey(0);

 //********************************************************************//

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < column; j++)
        {
            image.at<Vec3b>(i, j)[0] = int(arg(FFTf[j][i]));
            image.at<Vec3b>(i, j)[1] = int(arg(FFTf[j][i]));
            image.at<Vec3b>(i, j)[2] = int(arg(FFTf[j][i]));
        }
    }
    //show image FFT; 
    imshow("fft_phase_plot", image);
    imwrite("fft_phase_plot_lake.jpg", image);
    waitKey(0);
//******************************************************//

    if (operation)
        complex_conjugate(FFTf);    
    inp.clear();
//*******************************************************//

    //ROW ifft on FFTf
    for (int j = 0; j < pad; j++)
    {
        for (int i = 0; i < column; i++)
        {
            if (i < n)
            {
                inp.push_back(FFTf[i][j]);
            }
            else {
                complex<double> c(0, 0); // after n, do 0 padding
                inp.push_back(c);
            }
        }
        fun_ifft_butterfly(inp, row);
        inp.clear();
    }
    //Column of ifft
    for (int i = 0; i < IFFTt[0].size(); i++)
    {
        for (int j = 0; j < IFFTt.size(); j++)
        {
            inp.push_back(IFFTt[j][i]);
        }
        fun_ifft_butterfly(inp, col);
        inp.clear();
    }
//*****************************************************************//

    //show image IFFT;
    if (operation)
        Multipy_real_ifft(IFFTf);

//****************************************************************//

    //magnitude plot
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < column; j++)
        {
            image.at<Vec3b>(i, j)[0] = int(abs(IFFTf[j][i]));
            image.at<Vec3b>(i, j)[1] = int(abs(IFFTf[j][i]));
            image.at<Vec3b>(i, j)[2] = int(abs(IFFTf[j][i]));
        }
    }

    imshow("ifft_mag", image);
    imwrite("ifft_mag_plot_lake.jpg", image);
    waitKey(0);
    //phase plot
//*********************************************************************//

    return 0;
}
