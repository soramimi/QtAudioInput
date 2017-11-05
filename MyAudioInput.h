#ifndef MYAUDIOINPUT_H
#define MYAUDIOINPUT_H

#include <QThread>
#include "MainWindow.h"

class MyAudioInput : public QThread {
	Q_OBJECT
private:
	MainWindow *mainwindow = nullptr;
public:
	void bind(MainWindow *mw);

protected:
	void run();
signals:
	void updatePeak(int v);
};


#endif // MYAUDIOINPUT_H
