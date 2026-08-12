#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int main() {
    cv::Mat image = cv::imread("../../entrada.png");
    if (image.empty()) {
        std::cerr << "imagem nao carregada\n";
        return 1;
    }

    cout << image.cols << " x " << image.rows << endl;
    cv::Mat gray_image(image.rows, image.cols, CV_8UC1);


    for (int row=0;row<image.rows;++row) {
        for (int col=0;col<image.cols;++col) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(row, col);
            uchar B = pixel[0];
            uchar G = pixel[1];
            uchar R = pixel[2];

            uchar gray = (R+G+B)/3;
            //cout << "("<<(int)R<<"+"<<(int)G<<"+"<<(int)B<<")/3 = "<<(int)gray<<endl;
            if (gray > 0 && gray<=85) gray = 85;
            else if (gray > 85 && gray<=172) gray = 172;
            else gray = 255;
            gray_image.at<uchar>(row, col) = gray;

        }
    }

    cv::imwrite("../../saida.png", gray_image);
    return 0;
}