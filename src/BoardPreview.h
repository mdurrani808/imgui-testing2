#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
class BoardPreview {
private:
	Size *SIZE = new Size(640., 480.);
	Size sz;
	Mat m_img;
	Mat shadow;
	Mat maps;
	Mat Knew;
	static Mat project_image(Mat img, Size sz, Mat K, Mat rvec, Mat t);
public:
	BoardPreview(Mat img);
	void create_maps(Mat K, Mat cdist, Size sz);
	Mat project(Mat r, Mat t, bool useShadow, int inter);
};