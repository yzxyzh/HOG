//
//  hog.cpp
//  HOG
//
//  Created by Yanzixu on 14-9-22.
//  Copyright (c) 2014年 Yanzixu. All rights reserved.
//

#include "hog.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Lazarus {
    namespace HOG {
        
        inline void NormVec(vector<double>& inOutVec)
        {
            double L2_norm=0;
            for (int i=0; i<inOutVec.size(); i++) {
                L2_norm+=inOutVec.size();
            }
            L2_norm+=0.0001;
            for (int i=0; i<inOutVec.size(); i++) {
                inOutVec[i]/=L2_norm;
            }
            
            
            
        }
        
        
         void ComputeHOG(Mat* inSrc, int patch_width, vector<double>& outVec)
        {
            int nRows=inSrc->rows;
            int nCols=inSrc->cols;
            
            Mat in64f;
            
            if(CV_64FC1 == inSrc->type())
                inSrc->copyTo(in64f);
            else
                inSrc->convertTo(in64f, CV_64FC1, 1.0/255.0);
            
            Mat kernelY=(cv::Mat_<double>(3,3)<< 0,1.0,0,
                         0,0,0,
                         0,-1.0,0);
            Mat kernelX=(cv::Mat_<double>(3,3)<< 0,0,0,
                         1.0,0,-1.0,
                         0,0,0);
            
            Mat GradX,GradY;
            
            cv::filter2D(in64f,GradX,-1,kernelX);
            cv::filter2D(in64f,GradY,-1,kernelY);
            
            
            int x_size=nRows/patch_width;
            int y_size=nCols/patch_width;
            
            typedef  vector<double> HOG;
            typedef  vector<vector<HOG>> HoGBlockType;
            
            HoGBlockType newImg(x_size,vector<HOG>(y_size));
            
            
            for (int x=0; x<nRows-patch_width; x+=patch_width) {
                for (int y=0; y<nCols-patch_width; y+=patch_width) {
                    int i=x/patch_width;
                    int j=y/patch_width;
                    newImg[i][j].resize(9,0.0);
                    for (int p=0; p<patch_width; p++) {
                        double* gradXPtr=&GradX.ptr<double>(x+p)[y];
                        double* gradYPtr=&GradY.ptr<double>(x+p)[y];
                        for (int q=0; q<patch_width; q++) {
                            double gx_val=*gradXPtr++;
                            double gy_val=*gradYPtr++;
                            double currIntensity=sqrt(gx_val*gx_val+gy_val*gy_val+1e-7);
                            //把三四象限的边长反射到一二象限
                            if(gy_val<0)
                            {
                                gy_val=-gy_val;
                                gx_val=-gx_val;
                            }
                            double currAngle=asin(gy_val/currIntensity);
                            if(gx_val<0) currAngle=CV_PI-currAngle;
                            
                            int belonging=(currAngle*179.0/CV_PI)/20;
                            newImg[i][j][belonging]+=currIntensity;
                        }
                    }
                    
                }
            }
            
            outVec.clear();
            for (int x=0; x<nRows-2*patch_width; x+=patch_width) {
                for (int y=0; y<nCols-2*patch_width; y+=patch_width) {
                    int i=x/patch_width;
                    int j=y/patch_width;
                    HOG currentBlock;
                    currentBlock.insert(currentBlock.end(), newImg[i][j].begin(),newImg[i][j].end());
                    currentBlock.insert(currentBlock.end(), newImg[i][j+1].begin(),newImg[i][j+1].end());
                    currentBlock.insert(currentBlock.end(), newImg[i+1][j].begin(),newImg[i+1][j].end());
                    currentBlock.insert(currentBlock.end(), newImg[i+1][j+1].begin(),newImg[i+1][j+1].end());
                    
                    NormVec(currentBlock);
                    
                    outVec.insert(outVec.end(), currentBlock.begin(),currentBlock.end());
                    
                }
            }
            
            
            
#ifdef SHOW_HOG
     
            Mat show(nRows,nCols,CV_8UC1);
            show.setTo(Scalar(0.0));
            for (int x=0; x<nRows-patch_width; x+=patch_width) {
                for (int y=0; y<nCols-patch_width; y+=patch_width) {
                    int i=x/patch_width;
                    int j=y/patch_width;
                    
                    int maxIndex=-1;
                    double maxVal=-1;
                    for (int p=0; p<9; p++) {
                        if(newImg[i][j][p]>maxVal)
                        {
                            maxVal=newImg[i][j][p];
                            maxIndex=p;
                        }
                    }
                    
                    double angle=maxIndex*20*CV_PI/180.0;
                    Point2i radial(cos(angle)*patch_width/2.0,sin(angle)*patch_width/2.0);
                    Point2i central(y+patch_width/2,x+patch_width/2);
                    cv::line(show,central-radial,central+radial,Scalar(255));
                }
            }
            
            imshow("RESULT",show);
            waitKey(0);
#endif
            
            
            
            
            
        }
        
        
    }
}