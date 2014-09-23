#include "animatedlabel.h"
#include <QImage>
#include <QtMath>

AnimatedLabel::AnimatedLabel(QWidget *parent) :
    QLabel(parent),
    currentIndex(0),
    isPlaying(false)
{

}

void AnimatedLabel::initialize(const QString &imagePath, const int fps, const int startLoopIndex, const int endLoopIndex, const int width, const int height)
{
    pixmapSize.setWidth(width);
    pixmapSize.setHeight(height);
    this->startLoopIndex = startLoopIndex;
    this->endLoopIndex = endLoopIndex;

    QImage img;
    img.load(imagePath);
    int count = (img.height() * img.width()) / (width * height);

    for (int i = 1; i <= count; i++)
    {
        // TODO
        int x = ((i - 1) - (qFloor(i / 6) * 5)) * pixmapSize.height();
        int y = qFloor((i - 1) / 5) * pixmapSize.width();
        QImage sprite = img.copy(x, y, pixmapSize.width(), pixmapSize.height());
        pixmaps.push_back(QPixmap::fromImage(sprite));
    }

    connect(&timer, &QTimer::timeout, this, &AnimatedLabel::switchPixmap);
    timer.start(1000 / fps);
    switchPixmap();
}

void AnimatedLabel::start()
{

}

void AnimatedLabel::stop()
{

}

void AnimatedLabel::switchPixmap()
{
    if (currentIndex >= pixmaps.length())
        currentIndex = 0;

    setPixmap(pixmaps.at(currentIndex++));
}
