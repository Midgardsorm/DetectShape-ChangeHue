#ifndef HUECONTROLLER_H
#define HUECONTROLLER_H

#include <QColor>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QStringList>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>

using Contours = std::vector<std::vector<cv::Point>>;
using Pixel =  cv::Point3_<uint8_t>;

void mouseEvent(int a_evt, int a_x, int a_y, int a_flags, void *a_param);

// Helper function to find a cosine of angle between vectors from pt0->pt1 and pt0->pt2.
double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

class HueController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool isFileLoaded READ isFileLoaded NOTIFY fileLoaded)
  Q_PROPERTY(QStringList detectedShapes READ getDetectedShapes NOTIFY shapesDetected)
  Q_PROPERTY(int detectedTriangles MEMBER m_detectedTriangles NOTIFY shapesDetected)
  Q_PROPERTY(int detectedRectangles MEMBER m_detectedRectangles NOTIFY shapesDetected)
  Q_PROPERTY(int detectedCircles MEMBER m_detectedCircles NOTIFY shapesDetected)
  Q_PROPERTY(int detectedElipses MEMBER m_detectedElipses NOTIFY shapesDetected)
  Q_PROPERTY(int activeIndex READ getActiveItemIndex NOTIFY activeIndexChanged)

 public:
  HueController();

  Q_INVOKABLE void openFile(QString a_path);
  Q_INVOKABLE void detectShapes();
  Q_INVOKABLE void changeHue(int a_shift);
  Q_INVOKABLE void setActiveItemIndex(int a_activeIndex);
  Q_INVOKABLE void setSelectionColor(QColor a_color);
  Q_INVOKABLE void setMainImageWithCurrentHue();

  friend void mouseEvent(int a_evt, int a_x, int a_y, int a_flags, void *a_param);

  int getActiveItemIndex() const;
  QStringList getDetectedShapes() const;

 signals:
  void fileLoaded();

  void shapesDetected();

  void activeIndexChanged();

 private:
  void setWindow();
  void clearHueController();
  void selectItem(int a_x, int a_y);

  bool isFileLoaded() const;

 private:
  int m_selectedItem;
  cv::Scalar m_selectionColor;

  bool m_hasLoadedImage{};
  cv::Mat m_mainImage{};
  cv::Mat m_updatedHueImage{};
  Contours m_detectedContours{};
  QStringList m_detectedShapes{};
  int m_detectedTriangles{};
  int m_detectedRectangles{};
  int m_detectedCircles{};
  int m_detectedElipses{};
};

#endif // HUECONTROLLER_H
