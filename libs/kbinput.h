#ifndef _QTHWK_KBINPUT_H_
#define _QTHWK_KBINPUT_H_

#include <QMainWindow>
#include <QKeyEvent>

namespace __kbInput
{
	class keyboardStatusDef
	{
	public:
        bool l1, r1, u1, d1, toss1, shoot1, dash1;
        bool l2, r2, u2, d2, toss2, shoot2, dash2;
		keyboardStatusDef(){}
		void clear()
		{
            l1 = r1 = u1 = d1 = toss1 = shoot1 = dash1 = false;
            l2 = r2 = u2 = d2 = toss2 = shoot2 = dash2 = false;
			
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
            if(ev -> key() == Qt::Key_F)
                toss1 = Now;
            if(ev -> key() == Qt::Key_G)
                shoot1 = Now;
            if(ev -> key() == Qt::Key_H)
                dash1 = Now;

			if(ev -> key() == Qt::Key_Left)
                l2 = Now;
			if(ev -> key() == Qt::Key_Right)
                r2 = Now;
			if(ev -> key() == Qt::Key_Up)
                u2 = Now;
			if(ev -> key() == Qt::Key_Down)
                d2 = Now;
            if(ev -> key() == Qt::Key_Comma)
                toss2 = Now;
            if(ev -> key() == Qt::Key_Period)
                shoot2 = Now;
            if(ev -> key() == Qt::Key_Slash)
                dash2 = Now;
		}
	}keyboardStatus;
}
using __kbInput::keyboardStatus;

#endif // _QTHWK_KBINPUT_H_
