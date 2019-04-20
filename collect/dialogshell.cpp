#include "dialogshell.h"
#include <QtWidgets>

DialogShell::DialogShell(const QString &title, QWidget *p, Qt::WindowFlags f) : QDialog(p, f)
{
    setTitle(title);
    init();

    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::SubWindow);
    this->installEventFilter(this);
    this->setWindowState(Qt::WindowActive);
}

void DialogShell::setTitle(const QString &str)
{
    if(_titleLabel == nullptr)
    {
        _titleLabel = new QLabel(this);
        _titleLabel->setAttribute(Qt::WA_DeleteOnClose);
        _titleLabel->setFrameShape(QFrame::NoFrame);
        _titleLabel->setFrameShadow(QFrame::Plain);
        _titleLabel->setStyleSheet("background: rgb(27,27,28)");
        _titleLabel->setAlignment(Qt::AlignCenter);

        QPalette p = _titleLabel->palette();
        p.setColor(QPalette::WindowText, Qt::white);

        QFont font;
        font.setPixelSize(24);
        font.setBold(false);

        _titleLabel->setPalette(p);
        _titleLabel->setFont(font);
        _titleLabel->setFixedHeight(48);
    }
    _titleLabel->setText(str);
}

void DialogShell::init()
{
    if(vlay_ptr == nullptr)
    {
        QPushButton *p_okBtn     = new QPushButton("确定", this);
        QPushButton *p_cancelBtn = new QPushButton("取消", this);

        connect(p_okBtn,     &QPushButton::clicked, this, &QDialog::accept);
        connect(p_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

        //
        QHBoxLayout *hlay_ptr = new QHBoxLayout;
        hlay_ptr->setSpacing(4);
        hlay_ptr->setMargin(4);
        hlay_ptr->addStretch();
        hlay_ptr->addWidget(p_okBtn);
        hlay_ptr->addWidget(p_cancelBtn);

        //
        _contentW = new QWidget(this);

        //
        vlay_ptr = new QVBoxLayout(this);
        vlay_ptr->setMargin(0);
        vlay_ptr->setSpacing(0);

        vlay_ptr->addWidget(_titleLabel);
        vlay_ptr->addWidget(_contentW);
        vlay_ptr->addLayout(hlay_ptr);

        this->setLayout(vlay_ptr);
    }
}

void DialogShell::setContentWidget(QWidget *w)
{
    if(w)
    {
        w->setParent(this);

        vlay_ptr->replaceWidget(_contentW, w);
        _contentW->deleteLater();
        _contentW = nullptr;
    }
}

void DialogShell::disableMove()
{
    m_bMove = false;
}

void DialogShell::enableMove()
{
    m_bMove = true;
}

bool DialogShell::eventFilter(QObject *obj, QEvent *e)
{
    switch(e->type())
    {
    case QEvent::MouseButtonPress:
    {
        if(m_bMove)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
            if(mouseEvent->buttons() == Qt::LeftButton)
            {
                _pressed    = true;
                _pressPoint = mouseEvent->pos();
            }
        }
        return true;
    }
    case QEvent::MouseMove:
    {
        if(m_bMove && _pressed)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
            if(mouseEvent->buttons() == Qt::LeftButton)
            {
                const QPoint p = mouseEvent->pos() - _pressPoint;
                this->move(this->pos() + p);
            }
        }
        return true;
    }
    case QEvent::MouseButtonRelease:
    {
        _pressed = false;
        return true;
    }
    default: break;
    }

    return QDialog::eventFilter(obj, e);
}
