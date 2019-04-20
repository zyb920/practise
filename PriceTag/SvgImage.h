#ifndef SVGIMAGE_H
#define SVGIMAGE_H

#include <QSvgWidget>

class QSvgGenerator;

class SvgImage : public QSvgWidget
{
    Q_OBJECT
public:
    explicit SvgImage(QWidget *parent = 0);
    void openSvg(QString &file);
};

#endif // SVGIMAGE_H
