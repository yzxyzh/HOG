//
//  hog.h
//  HOG
//
//  Created by Yanzixu on 14-9-22.
//  Copyright (c) 2014年 Yanzixu. All rights reserved.
//

#ifndef __HOG__hog__
#define __HOG__hog__

#include <iostream>
#include <opencv2/core/core.hpp>
#include <vector>
using namespace std;
using namespace cv;

namespace Lazarus {
    namespace HOG {
        
        
        void ComputeHOG(Mat* inSrc, int patch_width, vector<double>& outVec);
        
        
    }
}



#endif /* defined(__HOG__hog__) */
