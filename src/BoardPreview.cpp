#include "BoardPreview.h"

BoardPreview::BoardPreview(Mat img) {
	img.copyTo(this->m_img);

	flip(this->m_img, this->m_img, 0);
	/*
	at this point the img is flipped upside down.This is needed because the Imgproc.warpPerspective
	flips the img upside down likely because it's acting like it's projecting through a camera
	aperture/lens which flips the scene upside down. So after the Imgproc.warpPerspective the img is
	again upside right.
	*/ 

	/*
	Modifies the old code to use thresholding rather than the for loop

	light to much to see low exposure camera images behind it so reduce almost to nothing. Can't set to 0 - messes up other places that check 0 or not 0
	set black pixels to gray; non-black pixels stay the same
	*/
	uchar Cfg_guidanceBlack = 1; // Assign your guidance black value
	uchar Cfg_guidanceWhite = -50;
	threshold(img, this->m_img, 1, Cfg_guidanceWhite, THRESH_BINARY);
	this->m_img.setTo(Cfg_guidanceBlack, this->m_img == 0);
	cvtColor(this->m_img, this->m_img, COLOR_GRAY2BGR);
	
	/* Original code
	* 
			// set blue and red channels to black (0) so gray/white becomes a shade of green (green channel was not changed)
		byte[] img3ChannelBuff = new byte[this.img.rows()*this.img.cols()*this.img.channels()]; // temp buffers for efficient access
		// process one row at a time for efficiency
		this.img.get(0, 0, img3ChannelBuff); // get the row, all channels
		for (int index = 0; index < img3ChannelBuff.length; index += 3) // process each triplet (channels) of the row
		{
			img3ChannelBuff[index] = 0; // B
			img3ChannelBuff[index + 2] = 0; // R
		}
		this.img.put(0, 0, img3ChannelBuff);

	*
	*/
	Mat imgBlue, imgRed, imgGreen;
	split(img, std::vector<Mat>{imgBlue, imgGreen, imgRed});
	threshold(imgBlue, imgBlue, 255, 0, THRESH_TOZERO_INV);
	threshold(imgRed, imgRed, 255, 0, THRESH_TOZERO_INV);
	merge(std::vector<Mat>{imgBlue, imgGreen, imgRed}, img);
	this->shadow = Mat::ones(this->m_img.rows, this->m_img.cols, CV_8UC1);
}
/*

* Computes and stores distortion maps based on provided camera parameters.
*
* Parameters:
*     K: The camera matrix.
*     cdist: Distortion coefficients.
*     sz: Size of the input images.
*/
void BoardPreview::create_maps(Mat K, Mat cdist, Size sz) {
	this->sz = sz;
	Mat scale = Mat::zeros(3, 3, K.type());

	// Calculating the scaling factors for the camera matrix
	scale.row(0).col(0) = static_cast<float>(this->SIZE->width) / sz.width;
	scale.row(1).col(1) = static_cast<float>(this->SIZE->height) / sz.height;
	scale.row(2).col(2) = 1.;

	gemm(scale, K, 1., Mat(), 0., this->Knew);

	sz = Size(this->SIZE->width, this->SIZE->height);

	Mat map1, map2;
	Mat R = Mat::eye(3, 3, CV_32F); // Identity matrix
	initUndistortRectifyMap(K, cdist, R, Knew, sz, CV_32FC1, map1, map2);

	// Store the distortion maps
	this->maps = map1; 
}

/**

 * Projects an input image onto a surface considering the specified camera parameters,
 * rotation, translation, and size.
 *
 * img The input image to be projected.
 * sz The size of the output image.
 * K The camera matrix.
 * rvec The rotation vector.
 * t The translation vector.
 * flags The interpolation method.
 * return The projected image on the surface.
 */
Mat project_img(Mat img, Size sz, Mat K, Mat rvec, Mat t, int flags) {
	Mat R = Mat();
	Rodrigues(rvec, R);
	Mat transform = Mat::zeros(3, 3, CV_64FC1);

	// Assigning values to the transform matrix
	transform.at<double>(0, 0) = R.at<double>(0, 0);
	transform.at<double>(0, 1) = R.at<double>(0, 1);
	transform.at<double>(0, 2) = R.at<double>(0, 2);
	transform.at<double>(1, 0) = R.at<double>(1, 0);
	transform.at<double>(1, 1) = R.at<double>(1, 1);
	transform.at<double>(1, 2) = R.at<double>(1, 2);
	transform.at<double>(2, 0) = t.at<double>(0, 0);
	transform.at<double>(2, 1) = t.at<double>(0, 1);
	transform.at<double>(2, 2) = t.at<double>(0, 2);
	transpose(transform, transform);
	Mat H = Mat();
	gemm(K, transform, 1., Mat(), 0., H);
	H /= H.at<double>(2, 2); //is this the same as Core.divide(H, new Scalar(H.get(2, 2)[0]), H);
	Mat imgProjected = Mat();
	warpPerspective(img, imgProjected, H, sz);
	flip(imgProjected, imgProjected, 0);
	drawFrameAxes(imgProjected, K, Mat(), R, t, 300.f);
	flip(imgProjected, imgProjected, 0);

	transform.release();
	R.release();
	H.release();
	return imgProjected;
}

/**
 *
 * Projects an image onto a surface using the specified rotation and translation matrices.
 *
 * r The rotation matrix.
 * t The translation matrix.
 * useShadow Determines whether to use the shadow or original image.
 * inter The interpolation method.
 * return The projected image.
 */
Mat BoardPreview::project(Mat r, Mat t, bool useShadow, int inter) {
	Mat img = Mat();
	Size actualSize = *(this->SIZE); 
	img = project_img(useShadow ? this->shadow : this->m_img, actualSize, this->Knew, r, t, INTER_LINEAR);
	remap(img, img, this->maps, Mat(), inter);
	resize(img, img, this->sz, 0, 0, inter);
	return img;
}
