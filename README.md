这个程序Implement了2D的HOG。block size为4（4个cell组成一组），cell的大小可以调整

编译方法：

clang++ -I/usr/local/include -L/usr/local/lib xxx.cpp xxx.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc
