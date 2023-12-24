#define NOMINMAX
#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <thread>  
#include <mutex>    


// opencv dependencies 
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// openGL
#include <glad/glad.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>