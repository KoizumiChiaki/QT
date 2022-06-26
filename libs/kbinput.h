#ifndef _QTHWK_KBINPUT_H_
#define _QTHWK_KBINPUT_H_

#include <QMainWindow>

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
				keyboardStatus.l1 = Now;
			if(ev -> key() == Qt::Key_D)
				keyboardStatus.r1 = Now;
			if(ev -> key() == Qt::Key_W)
				keyboardStatus.u1 = Now;
			if(ev -> key() == Qt::Key_S)
				keyboardStatus.d1 = Now;
			if(ev -> key() == Qt::Key_Left)
				keyboardStatus.l2 = Now;
			if(ev -> key() == Qt::Key_Right)
				keyboardStatus.r2 = Now;
			if(ev -> key() == Qt::Key_Up)
				keyboardStatus.u2 = Now;
			if(ev -> key() == Qt::Key_Down)
				keyboardStatus.d2 = Now;
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