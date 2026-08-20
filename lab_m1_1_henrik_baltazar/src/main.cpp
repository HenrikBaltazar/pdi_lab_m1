#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int height,width,channels,type,pixels=0;
int bMin, bMax, bAvg=0;
int gMin, gMax, gAvg=0;
int rMin, rMax, rAvg=0;

string input = "";
string output = "";
int operation = 0;

cv::Mat image;

int getChannelMinMax(int channel, bool min) {
    cv::Vec3b pixel = image.at<cv::Vec3b>(0, 0);
    uchar minmax=pixel[channel];;
    for (int row=0;row<image.rows;++row) {
        for (int col=0;col<image.cols;++col) {
            pixel = image.at<cv::Vec3b>(row, col);
            if (min) {
                if (pixel[channel]<minmax) minmax=pixel[channel];
            }else {
                if (pixel[channel]>minmax) minmax=pixel[channel];
            }
        }
    }
    return minmax;
}

int getChannelAvg(int channel) {
    int sum = 0;
    for (int row=0;row<image.rows;++row) {
        for (int col=0;col<image.cols;++col) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(row, col);
            sum+=pixel[channel];
        }
    }
    return sum/pixels;
}

void getImageInfo() {
    height = image.rows;
    width = image.cols;
    channels = image.channels();
    type = image.type();
    pixels = image.rows*image.cols;
    bMin = getChannelMinMax(0, true);
    bMax = getChannelMinMax(0, false);
    bAvg = getChannelAvg(0);
    gMin = getChannelMinMax(1, true);
    gMax = getChannelMinMax(1, false);
    gAvg = getChannelAvg(1);
    rMin = getChannelMinMax(2, true);
    rMax = getChannelMinMax(2, false);
    rAvg = getChannelAvg(2);
}

void showImageInfo() {
    cout << "Largura: " << width << endl;
    cout << "Altura: " << height << endl;
    cout << "Numero de canais: " << channels << endl;
    cout << "Tipo da imagem: " << type << endl;
    cout << "Quantidade de pixels: " << pixels << endl;
    if (operation != 2) {
        cout << "R: (Min: " << rMin << "; Max: " << rMax << "; Media: " << rAvg << ")" << endl;
        cout << "G: (Min: " << gMin << "; Max: " << gMax << "; Media: " << gAvg << ")" << endl;
        cout << "B: (Min: " << bMin << "; Max: " << bMax << "; Media: " << bAvg<< ")" << endl;
    }else {
        cout << "Min: " << bMin << "; Max: " << bMax << "; Media: " << bAvg << endl;
    }
}

void grayscale() {
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
    getImageInfo();
    showImageInfo();
    cv::imwrite("../../saida.png", gray_image);
}

int main( int argc, const char** argv ) {

    const string keys =
    "{help h usage ? |         | usage: pdi_app --input=cat.png --output=/users/me/desktop/dog.png --operation=1  }"
    "{input          |../../entrada.png| image to read                                                                    }"
    "{output         |../../saida.png| output folder                                                                    }"
    "{operation      |1        | 1:Get image info, 2:Output grayscale image, 3:Output reverse image               }"
    ;
    cv::CommandLineParser parser(argc, argv,keys);
    parser.about("Henrik Baltazar - Lab M1 parte 1");
    if (parser.has("help")){
        parser.printMessage();
        return 0;
    }
    if (!parser.check()){
        parser.printErrors();
        return 0;
    }
    input = parser.get<string>("input");
    output = parser.get<string>("output");
    operation = parser.get<int>("operation");

    image = cv::imread(input);
    if (image.empty()) {
        std::cerr << "imagem nao carregada\n";
        return 1;
    }

    getImageInfo();

    switch (operation) {
        case 1:
            showImageInfo();
            break;
        case 2:
            grayscale();
            break;
        case 3:
            showImageInfo();
            break;
        default: break;;
    }


    return 0;
}