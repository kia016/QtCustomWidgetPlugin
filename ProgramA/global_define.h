#ifndef GLOBAL_DEFINE_H
#define GLOBAL_DEFINE_H

#include <QString>
#include <QStringList>
#include <QImage>

#include "opencv2/opencv.hpp"
//#include <mil.h>
//#include <Camera/camerapaint.h>
#define BUFFERSIZE_MAX 16

enum AUTHORIZATION {
    NONE = -1,
    G4 = 0,
    ADMIN = 1,
    OPERATOR = 2
};

class AuthorClass {
public:
    static AUTHORIZATION StringToAuthor(QString author) {
        QStringList options = {"G4", "ADMIN", "OPERATOR"};
        switch (options.indexOf(author)) {
        case 0:
            return AUTHORIZATION::G4;
            break;
        case 1:
            return AUTHORIZATION::ADMIN;
            break;
        case 2:
            return AUTHORIZATION::OPERATOR;
            break;
        default:
            return AUTHORIZATION::NONE;
            break;
        }
    }

    static QString AuthorToString(AUTHORIZATION author) {
        switch (author) {
        case AUTHORIZATION::G4:
            return "G4";
            break;
        case AUTHORIZATION::ADMIN:
            return "ADMIN";
            break;
        case AUTHORIZATION::OPERATOR:
            return "OPERATOR";
            break;
        default:
            return "NONE";
            break;
        }
    }
};

typedef struct {
    QString id;
    QString password;
    AUTHORIZATION author;
} UserInfo;

typedef struct {
    QString name_;
    int count_;
    int width_;
    int height_;
    int channel_;
    int id_;
} HookDataStruct;

class CustomFunction {
public:
    static cv::Mat QImageToCvMat(const QImage &image) {
        switch (image.format()) {
          case QImage::Format_ARGB32:
          case QImage::Format_ARGB32_Premultiplied: {
              cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), static_cast<size_t>(image.bytesPerLine()));
              return mat.clone();
          }
          case QImage::Format_RGB32:
          case QImage::Format_RGB888: {
              QImage swapped = image;
              if (image.format() == QImage::Format_RGB32) {
                  swapped = swapped.convertToFormat(QImage::Format_RGB888);
              }
              swapped = swapped.rgbSwapped();
              cv::Mat mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), static_cast<size_t>(swapped.bytesPerLine()));
              return mat.clone();
          }
          case QImage::Format_Grayscale8:
          case QImage::Format_Indexed8: {
              cv::Mat mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), static_cast<size_t>(image.bytesPerLine()));
              return mat.clone();
          }
        }
        return cv::Mat();
    }
    static QImage CvMatToQImage(const cv::Mat &mat) {
        if (mat.type() == CV_8UC1) {
                QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
                image.setColorCount(256);
                for (int i = 0; i < 256; i++) {
                    image.setColor(i, qRgb(i, i, i));
                }
                uchar *pSrc = mat.data;
                for (int row = 0; row < mat.rows; row++) {
                    uchar *pDest = image.scanLine(row);
                    memcpy(pDest, pSrc, mat.cols);
                    pSrc += mat.step;
                }
                return image;
            } else if (mat.type() == CV_8UC3) {
                const uchar *pSrc = (const uchar*)mat.data;
                QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
                return image.rgbSwapped();
            } else if (mat.type() == CV_8UC4) {
                const uchar *pSrc = (const uchar*)mat.data;
                QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
                return image.copy();
            } else {
    //            qDebug() << "Error: Mat could not be converted to QImage.";
                return QImage();
            }
    }
};
#endif // GLOBAL_DEFINE_H
