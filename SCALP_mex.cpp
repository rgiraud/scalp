#include<mex.h>
#include<matrix.h>
#include"./include/SCALP.h"


void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
    unsigned char* R,* G,* B;
    int SuperpixelNum;
    int nRows;int nCols;
    unsigned short* label;
    unsigned short* output;
    unsigned char* img;
    
    //Default parameters
    float ratio = 0.075;  //Can be given in input
    float * kernel;
    int bres_color = 1;
    int pw = 3;
    float gamma = 50;
    
    float * contour;
    int bres_contour = 0;
    
    if(nrhs==3||nrhs==4)
    {
        if(mxGetNumberOfDimensions(prhs[0])!=3)
            mexErrMsgTxt("The input image must be in CIERGB form");
        if(mxGetClassID(prhs[0])!=mxUINT8_CLASS)
            mexErrMsgTxt("The input image must be in CIERGB form");
        nRows=mxGetM(prhs[0]);
        nCols=mxGetN(prhs[0])/3;
        
        img=(unsigned char*)mxGetPr(prhs[0]);
        SuperpixelNum= (int) mxGetScalar(prhs[1]);
        
        if(nrhs>=3)
            ratio= (float) mxGetScalar(prhs[2]);
        else
            ratio=0.075;
        
        
        if (nrhs == 4) {
            bres_contour = 1;
            contour = (float*) mxGetPr(prhs[3]);
            for (int i=0; i<nRows*nCols; i++)
                contour[i] = 1 + gamma*(1-expf(-contour[i]*contour[i]));
            
        }
    }
    else if(nrhs>5)
        mexErrMsgTxt("Too many inputs!");
    
        printf("%d\n", img[0]);
    
    
    int pixel=nRows*nCols;
    R=new unsigned char[pixel];
    G=new unsigned char[pixel];
    B=new unsigned char[pixel];
    label=new unsigned short[pixel];
    
    for(int i=0;i<pixel;i++)
    {
        R[i]=img[i];
        G[i]=img[i+pixel];
        B[i]=img[i+pixel+pixel];
    }
    
    
    //SCALP
    SCALP(R,G,B,nCols,nRows,SuperpixelNum,ratio,label,kernel,bres_color,bres_contour,contour,pw);
    
    
    //Output label map
    plhs[0]=mxCreateNumericMatrix(nRows,nCols,mxUINT16_CLASS,mxREAL);
    output=(unsigned short*)mxGetPr(plhs[0]);
    for(int i=0;i<pixel;i++)
        output[i]=label[i];
    
    delete [] R;
    delete [] G;
    delete [] B;
    delete [] label;
}
