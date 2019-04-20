#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
class QSvgWidget;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_saveAsBtn_clicked();

private:
    Ui::Widget *ui;
    QSvgWidget *svgW;

    QString svgContent;
    int maxHeight;
    int minHeight;
    QMap<QString, int> wMaxMap; //宽度map
    QMap<QString, int> wMinMap; //宽度map

};

#endif // WIDGET_H
