#ifndef _QTHWK_KBINPUT_H_
#define _QTHWK_KBINPUT_H_

#include <QMainWindow>
#include <QKeyEvent>

namespace __kbInput
{
	class keyboardStatusDef
	{
	public:
		bool l1, r1, u1, d1;
		bool l2, r2, u2, d2;
		keyboardStatusDef(){}
		void clear()
		{
			l1 = r1 = u1 = d1 = false;
			l2 = r2 = u2 = d2 = false;
			
		}
		void update(QKeyEvent *ev, bool Now)
		{
			if(ev -> key() == Qt::Key_A)
                l1 = Now;
			if(ev -> key() == Qt::Key_D)
                r1 = Now;
			if(ev -> key() == Qt::Key_W)
                u1 = Now;
			if(ev -> key() == Qt::Key_S)
                d1 = Now;
			if(ev -> key() == Qt::Key_Left)
                l2 = Now;
			if(ev -> key() == Qt::Key_Right)
                r2 = Now;
			if(ev -> key() == Qt::Key_Up)
                u2 = Now;
			if(ev -> key() == Qt::Key_Down)
                d2 = Now;
		}
	}keyboardStatus;
}
using __kbInput::keyboardStatus;
void QWidget::keyPressEvent(QKeyEvent *ev)
{
	keyboardStatus.update(ev, true);
}
void QWidget::keyReleaseEvent(QKeyEvent *ev)
{
	keyboardStatus.update(ev, false);
}


#endif
