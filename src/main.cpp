#include "Header.h"

cv::VideoCapture cap(0);
cv::Mat image;
float exposure = 1.f;
bool show_windows = true;
GLuint texture;


float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}
//gets the proper exposure time (OpenCV has different behavior for exposure based on platform)
float getExposure(float exposure_time) {
#ifdef _WIN64
	return clip(round((-log2(10000.0 / exposure_time))), -13, 0);
#endif
#ifdef __unix__
	return clip(exposure_time, 0, 1000);
#endif

}

void BindCVMat2GLTexture(cv::Mat& image, GLuint& imageTexture)
{

	if (image.empty()) {
		std::cout << "image empty" << std::endl;
	}
	else {
		glBindTexture(GL_TEXTURE_2D, imageTexture); // Allocate GPU memory for handle (Texture ID)

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

		glTexImage2D(GL_TEXTURE_2D,         // Type of texture
			0,                   // Pyramid level (for mip-mapping) - 0 is the top level
			GL_RGB,              // Internal colour format to convert to
			image.cols,          // Image width  i.e. 640 for Kinect in standard mode
			image.rows,          // Image height i.e. 480 for Kinect in standard mode
			0,                   // Border width in pixels (can either be 1 or 0)
			GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			GL_UNSIGNED_BYTE,    // Image data type
			image.ptr());        // The actual image data itself
	}
}

GLFWwindow* init() {

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Calibruh", nullptr, nullptr);
	//glfwSetWindowCloseCallback(window, [](GLFWwindow* window) { glfwSetWindowShouldClose(window, GL_FALSE); });
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	gl3wInit();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	return window;
}

int main(int argc, char* argv[])
{
	if (!glfwInit()) {
		return -1;
	}
	GLFWwindow* window = init();
	// Open the default camera
	if (!cap.isOpened()) {
		std::cerr << "Failed to open the webcam." << std::endl;
		return -1;
	}
e;
	while (!glfwWindowShouldClose(window)) {
		//initialize new frames
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		cap >> image; // Capture a frame to image
		if (image.empty()) {
			std::cerr << "Failed to capture an image from the webcam." << std::endl;
			return -1;
		}
		//bind opencv mat to texture and show in widget
		BindCVMat2GLTexture(image, texture);
		ImGui::Begin("Camera Stream", &show_windows, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture)), ImVec2(image.cols, image.rows));
		ImGui::End();

		//modify exposure widget
		ImGui::Begin("Camera Controls", &show_windows, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
		if (ImGui::SliderFloat("Exposure (ms)", &exposure, 1, 1000))
			cap.set(cv::CAP_PROP_EXPOSURE, getExposure(exposure));
		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glfwTerminate();
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	image.release();
	cap.release(); // Don't forget to release the VideoCapture object when done

	return 0;
}