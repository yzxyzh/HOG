//
//  main.cpp
//  HOG
//
//  Created by Yanzixu on 14-9-22.
//  Copyright (c) 2014年 Yanzixu. All rights reserved.
//

#include <iostream>
#include "hog.h"
#include <opencv2/highgui/highgui.hpp>
int main(int argc, const char * argv[])
{

    Mat img=imread("/Users/yanzixu/Downloads/tMap.bmp",CV_LOAD_IMAGE_GRAYSCALE);
    
    vector<double> out;
    Lazarus::HOG::ComputeHOG(&img, 16, out);
    
    
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

