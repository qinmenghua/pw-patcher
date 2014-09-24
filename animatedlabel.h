#ifndef ANIMATEDLABEL_H
#define ANIMATEDLABEL_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QLabel>
#include <QList>
#include <QPixmap>

class AnimatedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AnimatedLabel(QWidget *parent);

    void initialize(const QString &imagePath,
                    const int fps = 15,
                    const int startLoopIndex = 0,
                    const int endLoopIndex = 0,
                    const int width = 192,
                    const int height = 192);

    void start();
    void stop();
    bool isPlaying();

private slots:
    void switchPixmap();

private:
    QList<QPixmap> pixmaps;
    int currentIndex;
    int startLoopIndex;
    int endLoopIndex;
    QTimer timer;
    QSize pixmapSize;
    bool _isPlaying;
};

#endif // ANIMATEDLABEL_H
