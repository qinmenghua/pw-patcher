#include "animatedlabel.h"
#include <QImage>
#include <QtMath>

AnimatedLabel::AnimatedLabel(QWidget *parent) :
    QLabel(parent),
    currentIndex(0),
    _isPlaying(false)
{
    // hide when not playing
    setVisible(false);
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

    for (int i = 0; i < count; i++)
    {
        int x = (i % 5) * pixmapSize.height();
        int y = qFloor(i / 5) * pixmapSize.width();
        QImage sprite = img.copy(x, y, pixmapSize.width(), pixmapSize.height());
        pixmaps.push_back(QPixmap::fromImage(sprite));
    }

    connect(&timer, &QTimer::timeout, this, &AnimatedLabel::switchPixmap);
    timer.setInterval(1000 / fps);
}

void AnimatedLabel::start()
{
    currentIndex = 0;
    _isPlaying = true;

    setVisible(true);
    timer.start();
}

void AnimatedLabel::stop()
{
    _isPlaying = false;
}

bool AnimatedLabel::isPlaying()
{
    return _isPlaying;
}

void AnimatedLabel::switchPixmap()
{
    if (_isPlaying)
    {
        if (endLoopIndex > 0 && currentIndex > endLoopIndex)
            currentIndex = startLoopIndex;
        else if (currentIndex >= pixmaps.length())
            currentIndex = 0;
    }
    else if (currentIndex >= pixmaps.length())
    {
        setVisible(false);
        timer.stop();
        return;
    }

    setPixmap(pixmaps.at(currentIndex++));
}
