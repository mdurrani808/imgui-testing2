#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
namespace userConfig {
	inline constexpr bool writeBoard = true;
	inline const std::string boardFile = "ChArUcoBoard";
	inline constexpr bool logDetectedCorners = false;
	inline const std::string cornersLog = "corners.vnl";
	inline constexpr int resXDPM = 9843;
	inline constexpr int resYDPM = 9843;
}

namespace imageConfig {
	inline constexpr int image_width = 1280;
	inline constexpr int image_height = 720;
}

namespace calibConfig {
	inline constexpr int board_x = 9;
	inline constexpr int board_y = 6;
	inline constexpr int square_len = 280;
	inline constexpr int marker_len = 182; // mm?
	inline constexpr int dictionary = 0;
	inline constexpr int guidanceWhite = -50; //java code uses bytes...
	inline constexpr int guidanceBlack = 1;
	inline const cv::Scalar progressInsertCameraGrey = Scalar(170.);
	inline const cv::Scalar progressInsertCameraGuidanceGrey = Scalar(105.);
}

namespace convergenceConfig {
	inline constexpr int pt_min_parkers = 1;
	inline constexpr bool tryRefineMarkers = true;
	inline constexpr int cornerRefinementMaxIterations = 2000;
	//inline const int cornerRefinementMethod = cv::CORNER_R this doesnt work and idk why
	inline constexpr bool checkAllOrders = true;
	inline constexpr float errorCorrectionRate = 3.0f;
	inline constexpr float minRepDistance = 10.0f;
	inline constexpr double mean_flow_max = 3.;
	inline constexpr double pose_close_to_tgt_min = 0.85;
	inline constexpr double MAX_OVERLAP = .9;
	inline constexpr double minCorners = 6;
	inline constexpr double var_terminate = 0.1;
	inline cv::TermCriteria calibrateCameraCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, DBL_EPSILON);
	inline cv::TermCriteria solvePnPRefineVVSCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 20, FLT_EPSILON);
	inline constexpr double solvePnPRefineVVSLambda = 1.;
	inline cv::TermCriteria undistortPointsIterCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 20, FLT_EPSILON); // default cv::TermCriteria(cv::TermCriteria::COUNT, 5, 0.01)

}