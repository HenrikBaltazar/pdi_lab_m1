#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int height,width,channels,type,pixels=0;
int bMin, bMax, bAvg=0;
int gMin, gMax, gAvg=0;
int rMin, rMax, rAvg=0;

string input = "";
string output = "";
string operation = "";
int level = 0;

cv::Mat image;
cv::Mat output_image;

int getChannelMinMax(int channel, bool min) {
    cv::Vec3b pixel = output_image.at<cv::Vec3b>(0, 0);
    uchar minmax=pixel[channel];;
    for (int row=0;row<output_image.rows;++row) {
        for (int col=0;col<output_image.cols;++col) {
            pixel = output_image.at<cv::Vec3b>(row, col);
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
            cv::Vec3b pixel = output_image.at<cv::Vec3b>(row, col);
            sum+=pixel[channel];
        }
    }
    return sum/pixels;
}

void getImageInfo() {
    height = output_image.rows;
    width = output_image.cols;
    channels = output_image.channels();
    type = output_image.type();
    pixels = output_image.rows*output_image.cols;
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
    if (output_image.type() == CV_8UC3) {
        cout << "R: (Min: " << rMin << "; Max: " << rMax << "; Media: " << rAvg << ")" << endl;
        cout << "G: (Min: " << gMin << "; Max: " << gMax << "; Media: " << gAvg << ")" << endl;
        cout << "B: (Min: " << bMin << "; Max: " << bMax << "; Media: " << bAvg<< ")" << endl;
    }else {
        cout << "Min: " << bMin << "; Max: " << bMax << "; Media: " << bAvg << endl;
    }
}

void generateGrayscaleImage(float bw, float gw, float rw) {
    output_image.create(image.rows, image.cols, CV_8UC1);
    for (int row=0;row<image.rows;++row) {
        for (int col=0;col<image.cols;++col) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(row, col);
            uchar B = pixel[0];
            uchar G = pixel[1];
            uchar R = pixel[2];
            uchar gray = (R*rw+G*gw+B*bw)/3;
            output_image.at<uchar>(row, col) = gray;
        }
    }
}

void exportImage(string name) {
    getImageInfo();
    string filename;
    if (output[output.length()-1] == '/') {
        filename=output+name;
    }else {
        filename=output+"/"+name;
    }
    cv::imwrite(filename, output_image);
    cout << "<-- EXPORTED IMAGE: " << filename << " -->" << endl;
    showImageInfo();
    cout << "<--------------------------------------------------------->" << endl;
}

void generateCopyImage(uint8_t channel) {
    output_image.create(image.rows, image.cols, CV_8UC3);
    cv::Vec3b output_pixel;
    for (int row=0;row<image.rows;++row) {
        for (int col=0;col<image.cols;++col) {
            cv::Vec3b input_pixel = image.at<cv::Vec3b>(row, col);
            switch (channel) {
                case 0:
                    output_pixel[0] = input_pixel[0];
                    output_pixel[1] = 0;
                    output_pixel[2] = 0;
                    break;
                case 1:
                    output_pixel[0] = 0;
                    output_pixel[1] = input_pixel[1];
                    output_pixel[2] = 0;
                    break;
                case 2:
                    output_pixel[0] = 0;
                    output_pixel[1] = 0;
                    output_pixel[2] = input_pixel[2];
                    break;
                default:
                    output_pixel[0] = input_pixel[0];
                    output_pixel[1] = input_pixel[1];
                    output_pixel[2] = input_pixel[2];
                    break;
            }
            output_image.at<cv::Vec3b>(row, col) = output_pixel;
        }
    }
}

void generateQuantizedImage(int quant) {
    if (output_image.type() != CV_8UC1)
        generateGrayscaleImage(1,1,1);
    uchar v = 256 / quant;
    uchar k = 255 / (quant-1);
    for (int row=0;row<output_image.rows;++row) {
        for (int col=0;col<output_image.cols;++col) {
            uchar degree = output_image.at<uchar>(row, col) / v;
            output_image.at<uchar>(row,col) = degree * k;
        }
    }
}

int main( int argc, const char** argv ) {

    const string keys =
    "{help h usage ? |                             |    example: pdi_app --input=cat.png --output=/users/me/desktop/dog.png --operation=quantize --levels=8}"
    "{input          |images/input/input.png |    path for the file image to read}"
    "{output         |images/output/         |    path for the output folder}"
    "{operation      |inspect                      |    inspect: Get image info;\n		copy: Copy image;\n		channel_b: Generate channel B image\n		channel_g: Generate channel G image;\n		channel_r: Generate channel R image;\n		grayscale_average: Generate average grayscale image;\n		grayscale_weighted: Generate weighted grayscale image;\n		quantize: Generate specified levels images.}"
    "{levels         |16                           |    2, 4, 8 or 16 levels}"
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
    operation = parser.get<string>("operation");
    level = parser.get<int>("levels");

    string extension = input.substr(input.length()-3,3);
    if (extension != "png") {
        cout << "Only PNG supported" << endl;
        return 0;
    }
    image = cv::imread(input);
    if (image.empty()) {
        std::cerr << "imagem nao carregada\n";
        return 1;
    }
    output_image.create(image.rows, image.cols, image.type());

    if (operation == "inspect") {
        cout << " Show image info" << endl;
        getImageInfo();
        showImageInfo();
    }
    else if (operation == "copy") {
        cout << "Generate a copy image" << endl;
        generateCopyImage(5);
        exportImage("copy.png");
    }
    else if (operation == "channel_b") {
        cout << "Generate a copy image for channel B" << endl;
        generateCopyImage(0);
        exportImage("channel_b.png");
    }
    else if (operation == "channel_g") {
        cout << "Generate a copy image for channel G" << endl;
        generateCopyImage(1);
        exportImage("channel_g.png");
    }
    else if (operation == "channel_r") {
        cout << "Generate a copy image for channel R" << endl;
        generateCopyImage(2);
        exportImage("channel_r.png");
    }
    else if (operation == "grayscale_weighted") {
        cout << "Generate weighted gray scaled image " << endl;
        generateGrayscaleImage(0.114,0.587,0.299);
        exportImage("gray_weighted.png");
    }
    else if (operation == "grayscale_average") {
        cout << "Generate flat gray scaled image and 1: weighted gray scaled image " << endl;
        generateGrayscaleImage(1,1,1);
        exportImage("gray_average.png");
    }
    else if (operation == "quantize") {
        if (level != 2 && level != 4 && level != 8 && level != 16) {
            cout << "Invalid level" << endl;
            return 0;
        }
        cout << "Generate "<<level<<" levels quantized image" << endl;
        generateQuantizedImage(level);
        exportImage("quant_"+to_string(level)+".png");
    }
    else {
    cout << "Invalid operation" << endl;
    }

    return 0;
}