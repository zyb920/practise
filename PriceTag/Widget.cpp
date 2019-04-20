#include "Widget.h"
#include "ui_Widget.h"
#include <QPainter>
#include <QSvgWidget>
#include <QSvgGenerator>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>

#define Author "Author:赵彦博"
#define Email  "Email:408815041@qq.com"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(Author Email);

    QRegExp rx("\\d{1,3}");
    QValidator *validator = new QRegExpValidator(rx, this);

    ui->widthEdit->setValidator(validator);
    ui->heighEdit->setValidator(validator);

    QRegExp rx2("(\\d{1,4})(\\.\\d{1,2}){1}");
    QValidator *validator_2 = new QRegExpValidator(rx2, this);
    ui->priceEdit->setValidator(validator_2);

    svgW = new QSvgWidget;
    svgW->setObjectName("svgW");
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setMargin(0);
    hlay->addWidget(svgW,0,Qt::AlignCenter);
    ui->widget->setLayout(hlay);

    svgContent = "";

    maxHeight = 11;
    minHeight = 6;

    wMaxMap.insert("d",7);
    wMaxMap.insert(".",3);
    wMaxMap.insert("y",12);

    wMinMap.insert("d",3);
    wMinMap.insert(".",2);
    wMinMap.insert("y",6);

    on_pushButton_clicked();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    const int minW = 100;
    const int minH = 50;

    const int  imgW = ui->widthEdit->text().trimmed().toInt();
    const int  imgH = ui->heighEdit->text().trimmed().toInt();
    QString p = ui->priceEdit->text().trimmed();

    if(imgW < minW || imgW > ui->widget->width()) {
        QMessageBox::warning(nullptr, "Warning", QString("\n\n\t宽度适合范围[%1,%2]\t\t\n\n").arg(minW)
                             .arg(ui->widget->width()),
                             QMessageBox::Ok);
        return;
    }

    if(imgH < minH || imgH > ui->widget->height()) {
        QMessageBox::warning(nullptr, "Warning", QString("\n\n\t高度适合范围[%1,%2]\t\t\n\n").arg(minH)
                             .arg(ui->widget->height()),
                             QMessageBox::Ok);
        return;
    }

    QStringList list = p.split(".",QString::SkipEmptyParts);
    if(p.indexOf(".") > 0) {
        if(list.count() != 2) {
            QMessageBox::warning(nullptr, "Warning", QString("\n\n\t价格值不正确\t\t\n\n"),
                                 QMessageBox::Ok);
            return;
        }
    }


    QString str1(""),str2("");
    str1 = list.at(0);
    int w1 = str1.length() * wMaxMap["d"];
    if(p.toDouble() < 300) {
        str1.append(".");
        w1 += wMaxMap["."];
        if(list.count() == 1)
            str2 = "00";
        else
            str2 = list.at(1);
    }
    int w2 = str2.length() * wMinMap["d"];
    if(!str2.isEmpty()) w2 += 1;
    const int w3 = wMaxMap["y"];
    const int w = w1 + w2 + w3;
    const QString path = "./mySvg.svg";
    //生存SVG图像
    if(svgContent != p)
    {
        svgContent = p;
        QSvgGenerator generator;
        generator.setFileName(path);
        //generator.metric(QPaintDevice::PdmDevicePixelRatio);
        generator.setTitle(tr(Author));
        generator.setDescription(tr(Email));
        generator.setSize(QSize(w, maxHeight));
        generator.setViewBox(QRect(0, 0, w, maxHeight));

        QFont font;
        font.setFamily("Arial");

        QPainter painter;
        painter.begin(&generator);

        //画前部分
        font.setPixelSize(12);
        painter.setFont(font);
        painter.setPen(QColor(Qt::red));
        painter.fillRect(QRect(0, 0, w1+w2+w3, maxHeight), Qt::white);
        QRectF rf(0,-2.6,w1, maxHeight);
        painter.drawText(rf, str1);

        //画后部分
        if(!str2.isEmpty()) {
            font.setPixelSize(6);
            painter.setFont(font);
            painter.setPen(QColor(Qt::black));
            //painter.fillRect(QRectF(w1-0.6, 0, w2, maxHeight), Qt::white);
            QRectF rf2(w1-0.5,0,w2, minHeight);
            painter.drawText(rf2, str2);
        }

        //画元
        font.setPixelSize(12);
        painter.setFont(font);
        painter.setPen(QColor(Qt::blue));
        //painter.fillRect(QRectF(w1+w2-0.6, 0, w3, maxHeight), Qt::white);
        QRectF rf3(w1+w2-0.5,-2.6,w3, maxHeight);
        painter.drawText(rf3, "元");

        painter.end();
    }

    svgW->setFixedSize(imgW,imgH);
    svgW->load(path);
    svgW->update();
}

void Widget::on_saveAsBtn_clicked()
{
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(
                "PNG File (*.png);;"
                "JPG File (*.jpg);;"
                "BMP File (*.bmp)");
    dialog.setDirectory("./");
    if(dialog.exec() == QFileDialog::Accepted) {
        QString fileName = dialog.selectedFiles().at(0);
        const QString filter = dialog.selectedNameFilter().at(0);
        const QString tmpFilter = fileName.right(4).toLower();
        if(tmpFilter != ".png" && tmpFilter != ".jpg" && tmpFilter != ".bmp") {
            fileName.append(filter.mid(11,4));
        }
        if(svgW->grab().save(fileName, qPrintable(fileName.right(3).toUpper()))) {
            QMessageBox::information(nullptr, "info", QString("\n\n\t图片保存成功!\t\t\n\n"),
                                 QMessageBox::Ok);
        } else {
            QMessageBox::warning(nullptr, "Warning", QString("\n\n\t图片保存失败!\t\t\n\n"),
                                 QMessageBox::Ok);
        }
    }
}
