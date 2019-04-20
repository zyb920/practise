#ifndef DIALOGSHELL_H
#define DIALOGSHELL_H

#include <QDialog>
class QLabel;
class QVBoxLayout;

class DialogShell : public QDialog
{
    Q_OBJECT
public:
    explicit DialogShell(const QString &title, QWidget *p = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void setContentWidget(QWidget *w); //设置中心界面

    void disableMove();
    void enableMove();

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    void setTitle(const QString &str);//设置显示标题
    void init();

private:
    QLabel  *_titleLabel = nullptr;//标题
    QWidget *_contentW = nullptr;  //

    bool _pressed = false; //是否按下
    QPoint _pressPoint;    //按下的坐标
    bool m_bMove = true;

    QVBoxLayout *vlay_ptr = nullptr;
};

#endif // DIALOGSHELL_H
