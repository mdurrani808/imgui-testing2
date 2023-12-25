#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <vector>
#include <zlib.h>

int argmax(const std::vector<double>& vec) {
    return std::max_element(vec.begin(), vec.end()) - vec.begin();
}

int argmin(const std::vector<double>& vec) {
    return std::min_element(vec.begin(), vec.end()) - vec.begin();
}

bool isAllTrue(const std::vector<bool>& vec) {
    return std::all_of(vec.begin(), vec.end(), [](bool v) { return v; });
}
/*
std::string brief(cv::Mat mat) {
    return brief(mat, 4, 4, 12, 12);
}*/

std::string brief(cv::Mat mat, int firstRows, int lastRows, int firstRowData, int lastRowData) {
    const int acknowledgeRowsSkipped = 2000; // skip count to print an icon
    const int acknowledgeRowDataSkipped = 2000; // skip count to print an icon

    std::ostringstream sb;
    sb << mat;
    sb << "\n";

    int printCountRow = 0;
    int printCountColChan = 0;
    bool skippedRow = false;

    for (int row = 0; row < mat.rows; row++) {
        if (row >= firstRows && row < mat.rows - lastRows) {
            skippedRow = true;
            if (printCountRow % acknowledgeRowsSkipped == 0) {
                printCountRow = 0;
                sb << ".r.";
            }
            printCountRow++;
            continue;
        }
        if (skippedRow) {
            sb << "\n";
            skippedRow = false;
        }

        cv::Mat rowMat = mat.row(row);
        printCountColChan = 0;
        for (int colChan = 0; colChan < mat.cols * mat.channels(); colChan++) {
            if (colChan >= firstRowData && colChan < mat.cols * mat.channels() - lastRowData) {
                if (printCountColChan % acknowledgeRowDataSkipped == 0) {
                    printCountColChan = 0;
                    sb << ".c. ";
                }
                printCountColChan++;
                continue;
            }
            sb << rowMat.at<float>(colChan); // Assuming the Mat is of type CV_32F. Change as needed.
            sb << " ";
        }
        sb << "\n";
    }
    return sb.str();
}

void intToByteArray(int toBeConvertedInt, std::vector<unsigned char>& dst, int offset) {
    dst[offset + 3] = static_cast<unsigned char>(toBeConvertedInt & 0xff); // least significant byte

    toBeConvertedInt >>= 8;
    dst[offset + 2] = static_cast<unsigned char>(toBeConvertedInt & 0xff);

    toBeConvertedInt >>= 8;
    dst[offset + 1] = static_cast<unsigned char>(toBeConvertedInt & 0xff);

    toBeConvertedInt >>= 8;
    dst[offset + 0] = static_cast<unsigned char>(toBeConvertedInt); // most significant byte
}
/*
std::vector<unsigned char> compress(const std::vector<unsigned char>& bytesToCompress) {
    uLongf compressedSize = compressBound(bytesToCompress.size());
    std::vector<unsigned char> compressedBytes(compressedSize);

    if (compress2(compressedBytes.data(), &compressedSize, bytesToCompress.data(), bytesToCompress.size(), Z_BEST_COMPRESSION) != Z_OK) {
        // Handle compression error
    }

    compressedBytes.resize(compressedSize);
    return compressedBytes;
}*/



