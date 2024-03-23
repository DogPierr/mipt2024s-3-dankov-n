#include "junction.h"

#include <filesystem>
#include <opencv2/opencv.hpp>

JunctionMat::JunctionMat(int radius_border, int angle_border)
    : cv::Mat(), radius_border_(radius_border), angle_border_(angle_border) {
  FindBorderPoints();
}

JunctionMat::JunctionMat(const cv::Mat& mat, int radius_border,
                         int angle_border)
    : cv::Mat(mat), radius_border_(radius_border), angle_border_(angle_border) {
  FindBorderPoints();
}

JunctionMat::JunctionMat(const std::string& filename, int radius_border,
                         int angle_border)
    : cv::Mat(cv::imread(filename)),
      radius_border_(radius_border),
      angle_border_(angle_border) {
  FindBorderPoints();
}

std::vector<cv::Point> JunctionMat::FindJunctionPoints() {
  std::vector<cv::Point> result;
  for (size_t i = 0; i < borderPoints_.size(); ++i) {
    cv::Point p1 =
        borderPoints_[i - radius_border_ >= 0
                          ? i - radius_border_
                          : borderPoints_.size() + i - radius_border_];
    cv::Point p2 = borderPoints_[i];
    cv::Point p3 =
        borderPoints_[i + radius_border_ < borderPoints_.size()
                          ? i + radius_border_
                          : i + radius_border_ - borderPoints_.size()];

    double angle = std::acos((p2 - p1).dot(p3 - p2) /
                             (cv::norm(p2 - p1) * cv::norm(p3 - p2))) *
                   180 / M_PI;

    if (IsJunction(angle, result)) {
      result.push_back(p2);
      cv::arrowedLine(*this, p1, p2, cv::Scalar(0, 255, 0), 2);
      cv::arrowedLine(*this, p2, p3, cv::Scalar(0, 255, 255), 2);
    }
    cv::arrowedLine(*this, borderPoints_[i],
             borderPoints_[i > 0 ? i - 1 : borderPoints_.size() - 1],
             cv::Scalar(0, 0, 255), 2);
  }

  for (const cv::Point& point : result) {
    cv::circle(*this, point, 5, cv::Scalar(255, 0, 0), 2);
  }
  return result;
}

void JunctionMat::Draw() {
  cv::namedWindow("Image Edges", cv::WINDOW_AUTOSIZE);
  cv::imshow("Image Edges", *this);
  cv::waitKey(0);
}

void JunctionMat::FindBorderPoints() {
  cv::Mat edges;
  cv::Canny(*this, edges, 100, 200);

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
  borderPoints_ = contours[0];
}

void JunctionMat::Save(const std::string& filename) {
  cv::imwrite(filename, *this);
}

bool JunctionMat::IsJunction(double angle,
                             const std::vector<cv::Point>& result) {
  return angle > angle_border_ && angle < 180 &&
         (result.size() <= 1 || cv::norm(result[result.size() - 1] -
                                         result[result.size() - 2]) > 0.99);
}

void processImagesInFolder(const std::string& folderPath,
                           const std::string& outputFolderPath,
                           const std::string& combinedImagePath) {
  std::vector<JunctionMat> processedImages;
  for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
    if (entry.is_regular_file() && (entry.path().extension() == ".jpg" ||
                                    entry.path().extension() == ".png")) {
      JunctionMat junctionMat(entry.path().string(), 15, 100);

      if (junctionMat.empty()) {
        std::cerr << "Error loading image: " << entry.path().string()
                  << std::endl;
        continue;
      }

      junctionMat.FindJunctionPoints();
      junctionMat.Save(outputFolderPath + entry.path().filename().string());
      processedImages.push_back(junctionMat);
    }
  }
  if (!processedImages.empty()) {
    int numRows = 4;
    int numCols = (processedImages.size() + numRows - 1) / numRows;

    int cellWidth = processedImages[0].cols;
    int cellHeight = processedImages[0].rows;

    cv::Mat combinedImage(cellHeight * numRows, cellWidth * numCols,
                          processedImages[0].type(), cv::Scalar(255, 255, 255));

    for (int i = 0; i < processedImages.size(); ++i) {
      int row = i / numCols;
      int col = i % numCols;

      cv::Rect roi(col * cellWidth, row * cellHeight, cellWidth, cellHeight);
      cv::Mat targetROI = combinedImage(roi);
      cv::resize(processedImages[i], processedImages[i], targetROI.size());
      processedImages[i].copyTo(targetROI);
    }

    std::string outputFileName = combinedImagePath + "combined_result.png";
    cv::imwrite(outputFileName, combinedImage);
  }
}

int main() {
  std::string folderPath = "../prj.cw/images/test/";
  std::string outputFolderPath = "../prj.cw/images/result/";
  std::string combinedImagePath = "../prj.cw/images/resultcombo/";

  processImagesInFolder(folderPath, outputFolderPath, combinedImagePath);

  return 0;
}