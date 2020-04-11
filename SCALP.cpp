#include"./include/SCALP.h"
#include "./include/CImg.h"
#include <iostream>
#include <string>
// #include "./utils_sp/GR.h"
// #include "./utils_sp/ASA.h"


using namespace cimg_library;


int main(int argc, char* argv[])
{
    if (argc < 3) {
        printf("Not enough inputs!\nUsage : ./SCALP -i img_name [-outm output_map_name] [-outb output_border_name]  [-k superpixel_nbr] [-m compactness] [-c contour]\n");
        return -1;
    }
    
    //Inputs
    string img_name = string(cimg_option("-i","","Input image file"));
    int SuperpixelNum = cimg_option("-k",450,"Number of desired superpixels");
    float ratio = cimg_option("-m", 0.075, "Compactness value");
    string contour_input = string(cimg_option("-c","","Input contour image file"));
    string gt_input = string(cimg_option("-gt","","Input ground truth image file"));
    
    //Outputs
    string output_map_name = string(cimg_option("-outm","labelmap.png","Output Labeled image file"));
    string output_border_name = string(cimg_option("-outb","borders.png","Output borders of superPixels with original image as background"));
    
    
    //Default parameters
    float * kernel = NULL;
    int bres_color = 1;
    int pw = 3;
    float gamma = 50;
    
    
    //Image loading
    cout << img_name.c_str() << "\n";
    CImg<int> img_in(img_name.c_str());
    CImg<int> img = img_in;
    int nCols = img.width();
    int nRows = img.height();
    
    
    //Contour loading
    float * contour = NULL;
    int bres_contour = 0;
    
    if(contour_input!="") {
        
        bres_contour = 1;
        
        CImg<int> contour_img(contour_input.c_str());
        
        if ( (contour_img.width() != nCols) || (contour_img.height() != nRows) ) {
            printf("Contour dimensions do not match image size!\n");
            return -1;
        }
        
        contour = (float *) malloc(nCols*nRows*sizeof(float));
        for (int j=0; j<nCols; j++) {
            for (int i=0; i<nRows; i++)
                contour[i+j*nRows] = 1 + gamma*(1-expf(-contour_img(j,i)*contour_img(j,i)/(255*255)));
        }
        
    }
    
    unsigned char* R,* G,* B;
    unsigned short* label;
    int pixel=nRows*nCols;
    R=new unsigned char[pixel];
    G=new unsigned char[pixel];
    B=new unsigned char[pixel];
    label=new unsigned short[pixel];
    
    for (int j=0; j<nCols; j++) {
        for (int i=0; i<nRows; i++)
        {
            R[i+j*nRows] = img(j,i,0,0);
            G[i+j*nRows] = img(j,i,0,1);
            B[i+j*nRows] = img(j,i,0,2);
        }
    }
    
    //SCALP
    SCALP(R,G,B,nCols,nRows,SuperpixelNum,ratio,label,kernel,bres_color,bres_contour,contour,pw);
    
    
    // SAVE OUTPUTS
    
    // Output Label image (main output for many applications)
    int max_sp = 0;
    CImg<int> output(nCols,nRows);
    for (int i=0; i<nCols; i++){
        for (int j=0; j<nRows; j++) {
            output(i,j) = (int) label[j+i*nRows];
            if (output(i,j) > max_sp)
                max_sp = output(i,j);
        }
    }
    
    char str[100];
    strcpy(str, "res/");
    strcat(str, output_map_name.c_str());
    output.save(str);
    
    
    // Output borders of SuperPixels with original image as background
    // Draw borders in a slice by slice fashion for 3d images
    CImg<> output_border = img;
    int v4x[]={-1,0,1,0};
    int v4y[]={0,-1,0,1};
    
    cimg_forZ(img,z) {
        cimg_forXY(output_border,x,y) {
            int lab1=output(x,y,z);
            for(int k=0;k<4;k++)
                if(output_border.containsXYZC(x+v4x[k],y+v4y[k],z))
                    if(lab1 != output(x+v4x[k],y+v4y[k],z))
                        cimg_forC(output_border,c)
                        output_border(x,y,z,c)=0;
        }
    }
    
    char str2[100];
    strcpy(str2, "res/");
    strcat(str2, output_border_name.c_str());
    output_border.save(str2);
    
    
    delete [] R;
    delete [] G;
    delete [] B;
    delete [] label;
    
    if(contour_input!="")
        free(contour);
    
    
    
    ////////////
    //Metric evaluation
    ////////////
    
    //Reprocessing
    max_sp = max_sp +1;
    int *spx_sizes = (int *)calloc(max_sp, sizeof(int));
    for (int i = 0; i < nCols*nRows; i++)
        spx_sizes[output[i]]++;
    
    
    int **spx = (int **) malloc(max_sp * sizeof(int *));
    
    
    for (int i = 0; i < max_sp; i++) {
        spx[i] = (int *) malloc(spx_sizes[i] * sizeof(int));
        spx_sizes[i] = 0;
    }
    
    for (int i = 0; i < nCols*nRows; i++)
        spx[ output[i] ][ spx_sizes[output[i]] ++ ] = i;
    
    
    
    
//     ////////////////////////
//     // If downloaded display and evaluation superpixel toolbox
//     // at remigiraud.fr/research/#superpixels
//     //Ground Truth loading
//     int * gt = NULL;
//     if(gt_input!="") {
//
//         CImg<int> gt_img(gt_input.c_str());
//
//         if ( (gt_img.width() != nCols) || (gt_img.height() != nRows) ) {
//             printf("Ground truth dimensions do not match image size!\n");
//             return -1;
//         }
//
//         gt = (int *) malloc(nCols*nRows*sizeof(int));
//         for (int i=0; i<nCols*nRows; i++)
//             gt[i] = gt_img[i];
//
//         double asa = asa_metric(spx, spx_sizes, max_sp, nCols, nRows, gt);
//
//         cout << "ASA = " << asa << "\n";
//
//     }
//     
//     ////////////////////////
//     // If downloaded Global Regularity (GR) superpixel metric at remigiraud.fr/research/gr.php
//     // add #include "utils_sp/GR.h"
//     
//     double gr = gr_metric(spx, spx_sizes, max_sp, nCols, nRows);
//     
//     cout << "GR = " << gr << "\n";
    
    
    
    
    free(spx);
    free(spx_sizes);
    
    
    
    return 0;
}
