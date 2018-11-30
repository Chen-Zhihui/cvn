#ifndef FocusWidget_H
#define FocusWidget_H
#include <QWidget>
#include <QEvent>
#include <QPainter>

template <class BaseWidget>
class FocusWidget : public BaseWidget {
public:
    FocusWidget(QWidget *parent = nullptr)  : BaseWidget(parent){
        BaseWidget::setFocusPolicy(Qt::ClickFocus);
    }

    void sltChange(QWidget* old, QWidget* current){
        bool oldFocurs = false;
        while(old){
            auto oldWdg = dynamic_cast<FocusWidget*>(old);
            if(oldWdg && oldWdg == this){
                oldFocurs = true;
                break;
            }
            old = old->parentWidget();
        }
        bool curFocurs = false;

        while(current){
            auto curWdg = dynamic_cast<FocusWidget*>(current);
            if(curWdg && curWdg == this){
                curFocurs = true;
                break;
            }
            current = current->parentWidget();
        }

        if(oldFocurs && !curFocurs){
            _focursOn = false;
            update();
            changeShortCut();
        }  else if(!oldFocurs && curFocurs){
            _focursOn = true;
            update();
            changeShortCut();
        }
    }

    virtual void paintEvent(QPaintEvent* event){
        BaseWidget::paintEvent(event);
        if(_focursOn){
            QPainter painter(this);
            painter.setPen(QPen(QColor(244, 121,0), 2,Qt::SolidLine ));
            QRect rect = this->rect();
            painter.drawRect(rect);
        }
    }

    virtual void changeShortCut(){}

    bool isClickOnSelf(){return _clickOnSelf;}

protected:
    bool _focursOn = false;
    bool _clickOnSelf = false;
};

#endif // FocusWidget_H
