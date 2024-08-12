#include <algorithm>
#include "opencv2/opencv.hpp"
#include "Timer.h"

using namespace std;
using namespace cv;

typedef struct _Input 
{
    Mat im_bgr;
    Mat im_gray;
    
} Input;

typedef struct _Output 
{
    Mat im_edge;
    
} Output;

std::string itos_formatted(int ii, int n_digit)
{
    std::stringstream ss;
    ss << std::setw(n_digit) << std::setfill('0') << ii;
    return ss.str();
}

bool is_only_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
	        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}


//------------ Check if the string is camera index --------------  
//	cout << is_this_camera_index("04");
//	=> true
bool is_this_camera_index(const std::string& strin)
{
	return 2 >= strin.size() && is_only_number(strin); 	
}

bool proc_common(Output& output, const Input& input)
{
    Canny(input.im_gray, output.im_edge, 100.0, 300);
    return true;
}

bool proc_img(const std::string& strin)
{
    Input input;
    Output output;
    input.im_gray = cv::imread(strin, CV_LOAD_IMAGE_GRAYSCALE);
    proc_common(output, input);
    //imwrite("im_edge.bmp", output.im_edge);
    imshow("im_egge", output.im_edge); waitKey();    
    return true;
}

bool proc_cam(int idxCam)
{
    Input input;
    Output output;
	//Mat im_bgr;
    Timer timer;    timer.Start();
	//비디오 캡쳐 초기화
	VideoCapture cap(idxCam);
	if (!cap.isOpened()) {
		cerr << "에러 - 카메라를 열 수 없습니다.\n";	return false;
	}
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    //cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	while(1)
	{
		// 카메라로부터 캡쳐한 영상을 frame에 저장합니다.
		cap.read(input.im_bgr);
		if(input.im_bgr.empty()) {
			cerr << "빈 영상이 캡쳐되었습니다.\n";	return false;
		}
		// 영상을 화면에 보여줍니다.
        //Mat im_gray, im_edge;
        cvtColor(input.im_bgr, input.im_gray, CV_BGR2GRAY);
        proc_common(output, input);
		imshow("im_edge", output.im_edge);
        // ESC 키를 입력하면 루프가 종료됩니다. 
		char ch = waitKey(1);
		if (27 == ch) break;
        cout << "FPS : " << timer.updateFPS() << endl;
	}
    return true;
}

//void main()
int main(int argc, char **argv)
{
	if(argc < 2) 
    {
        cout << "Usage : " << argv[0] << " [cameraID or path to image]" << endl;	return 0;
    }     
	return is_this_camera_index(argv[1]) ? proc_cam(atoi(argv[1])) : proc_img(argv[1]);
}
