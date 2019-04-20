#include "SvgImage.h"
#include <QPainter>
#include <QSvgGenerator>

SvgImage::SvgImage(QWidget *parent) : QSvgWidget(parent)
{
    this->load(QString("D:/xx.svg"));
}
