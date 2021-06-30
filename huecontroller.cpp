#include "huecontroller.h"

constexpr int NO_SELECTED_ITEM = -1;
constexpr int FIRST_ELEMENT = 0;
constexpr int SELECTION_WIDTH = 2;
constexpr int INSIDE_POLYGON = 1;

void mouseEvent(int a_evt, int a_x, int a_y, int a_flags, void *a_param)
{
  if (a_evt == cv::EVENT_LBUTTONDOWN) {
    HueController *controller = static_cast<HueController*>(a_param);
    controller->selectItem(a_x, a_y);
  }
}

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
  double dx1 = pt1.x - pt0.x;
  double dy1 = pt1.y - pt0.y;
  double dx2 = pt2.x - pt0.x;
  double dy2 = pt2.y - pt0.y;
  return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

HueController::HueController()
  : m_selectedItem{ NO_SELECTED_ITEM }
  , m_selectionColor{cv::Scalar(255,0,0)}
{
}

void HueController::openFile(QString a_path) {
  QString filePath = a_path.replace("file:///", "");
  m_mainImage = cv::imread(filePath.toStdString());
  if (m_mainImage.empty()) {
    qDebug() << "Wrong file path: " <<  filePath;
  } else {
    clearHueController();
    m_hasLoadedImage = true;
    setWindow();
    emit shapesDetected();
    emit fileLoaded();
  }
}

void HueController::detectShapes() {
  clearHueController();
  cv::Mat gray;
  cv::cvtColor(m_mainImage, gray, cv::COLOR_BGR2GRAY);

  // Use Canny instead of threshold to catch squares with gradient shading.
  cv::Mat bw;
  cv::Canny(gray, bw, 0, 50, 5);

  Contours contours{};
  // Find contours.
  cv::findContours(bw.clone(), contours, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_NONE);

  std::vector<cv::Point> approx;
  cv::Mat dst = m_mainImage.clone();

  int triangles{};
  int rectangles{};
  int circles{};
  int elipses{};

  for (std::size_t i = 0; i < contours.size(); i++) {
    // Approximate contour with accuracy proportional to the contour perimeter.
    cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

    // Skip small or non-convex objects.
    if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx)) {
      continue;
    }

    // Skip non-convex objects.
    if (!cv::isContourConvex(approx)) {
      continue;
    }

    if (approx.size() == 3) {
      m_detectedContours.push_back(std::move(contours[i]));
      m_detectedShapes.emplace_back("Triangle_" + QString::number(triangles++));
    } else if (approx.size() == 4) {

      // Number of vertices of polygonal curve.
      int vtc = approx.size();

      // Get the cosines of all corners.
      std::vector<double> cos;
      for (int j = 2; j < vtc + 1; j++) {
        cos.push_back(angle(approx[j % vtc], approx[j - 2], approx[j - 1]));
      }

      // Sort ascending the cosine values.
      std::sort(cos.begin(), cos.end());

      // Get the lowest and the highest cosine.
      double mincos = cos.front();
      double maxcos = cos.back();

      // Use the degrees obtained above and the number of vertices to determine
      // the shape of the contour.
      if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3) {
        m_detectedContours.push_back(std::move(contours[i]));
        m_detectedShapes.emplace_back("Rectangle_" + QString::number(rectangles++));
      }
    } else {
      // Detect and label circles.
      double area = cv::contourArea(contours[i]);
      cv::Rect r = cv::boundingRect(contours[i]);
      int radius = r.width / 2;

      if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
          std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2) {
        m_detectedContours.push_back(std::move(contours[i]));
        m_detectedShapes.emplace_back("Circle_" + QString::number(circles++));
      } else {
        // Now we know that figure has at least 8 vertices, is a convex shape and
        // is not a circle, so we can assume that it is closly to elipse. To acquire
        // definitive detection further calculation will be needed.
        m_detectedContours.push_back(std::move(contours[i]));
        m_detectedShapes.emplace_back("Elipse_" + QString::number(elipses++));
      }
    }
  }

  m_detectedTriangles = triangles;
  m_detectedRectangles = rectangles;
  m_detectedCircles = circles;
  m_detectedElipses = elipses;

  emit shapesDetected();
  if (m_detectedShapes.size()) {
    setActiveItemIndex(FIRST_ELEMENT);
  }
}

void HueController::selectItem(int a_x, int a_y) {
  cv::Mat img = m_mainImage.clone();
  for (int i = 0; i < m_detectedContours.size(); ++i) {
    double isInsidePolygon = cv::pointPolygonTest(m_detectedContours[i], cv::Point(a_x, a_y), false);
    if (isInsidePolygon == INSIDE_POLYGON) {
      m_selectedItem = i;
    }
  }
  cv::drawContours(img, m_detectedContours, m_selectedItem, m_selectionColor, SELECTION_WIDTH, cv::LINE_AA);
  cv::imshow("Content", img);

  emit activeIndexChanged();
}

void HueController::clearHueController() {
  m_detectedContours.clear();
  m_detectedShapes.clear();
  m_selectedItem = NO_SELECTED_ITEM;
  m_detectedTriangles = 0;
  m_detectedRectangles = 0;
  m_detectedCircles = 0;
  m_detectedElipses = 0;
}

QStringList HueController::getDetectedShapes() const {
  return m_detectedShapes;
}

bool HueController::isFileLoaded() const {
  return m_hasLoadedImage;
}

int HueController::getActiveItemIndex() const {
  return m_selectedItem;
}

void HueController::setActiveItemIndex(int a_activeIndex) {
  m_selectedItem = a_activeIndex;
  cv::Mat img = m_mainImage.clone();
  cv::drawContours(img, m_detectedContours, m_selectedItem, m_selectionColor, SELECTION_WIDTH, cv::LINE_AA);
  cv::imshow("Content", img);

  emit activeIndexChanged();
}

void HueController::changeHue(int a_shift) {
  // Change hue of entire image.
  cv::Mat tempImage = m_mainImage.clone();
  cv::cvtColor(tempImage, tempImage, cv::COLOR_BGR2HSV);
  tempImage.forEach<Pixel>([&](Pixel &p, const int position[]) { p.x = (p.x + a_shift) % 180; });
  cv::cvtColor(tempImage, tempImage, cv::COLOR_HSV2BGR);

  // Take from image with chage hue only the part with our contour.
  cv::Mat foreground{};
  Contours selectedContours{ m_detectedContours[m_selectedItem]};
  cv::Mat mask = cv::Mat::zeros(m_mainImage.size(), m_mainImage.type());
  cv::drawContours(mask, selectedContours, -1, cv::Scalar(255, 255, 255), cv::FILLED);
  cv::bitwise_and(tempImage, mask, foreground);

  // Take rest from orignal image.
  cv::Mat background{};
  cv::bitwise_not(mask, mask);
  cv::bitwise_and(m_mainImage.clone(), mask, background);

  // Combine foreground with background.
  cv::Mat finalImage{};
  cv::bitwise_or(foreground, background, finalImage);

  m_updatedHueImage = finalImage.clone();
  cv::drawContours(finalImage, m_detectedContours, m_selectedItem, m_selectionColor, SELECTION_WIDTH, cv::LINE_AA);
  cv::imshow("Content", finalImage);
}

void HueController::setWindow(){
  cv::namedWindow("Content", cv::WINDOW_NORMAL);
  cv::setMouseCallback("Content", mouseEvent, this);
  cv::imshow("Content", m_mainImage);
}

void HueController::setSelectionColor(QColor a_color) {
  m_selectionColor = cv::Scalar(a_color.blue(), a_color.green(), a_color.red());
  if (isFileLoaded()) {
    cv::Mat img = m_mainImage.clone();
    cv::drawContours(img, m_detectedContours, m_selectedItem, m_selectionColor, SELECTION_WIDTH, cv::LINE_AA);
    cv::imshow("Content", img);
  }
}

void HueController::setMainImageWithCurrentHue() {
  m_mainImage = m_updatedHueImage;
}
