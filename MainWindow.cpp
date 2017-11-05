#include "MainWindow.h"
#include "MyAudioInput.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QThread>
#include <QTime>
#include <memory>


struct MainWindow::Private {
	MyAudioInput audio_in;
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m(new Private)
{
	ui->setupUi(this);
	m->audio_in.bind(this);
	connect(&m->audio_in, SIGNAL(updatePeak(int)), this, SLOT(onUpdatePeak(int)));

	m->audio_in.start();
}

MainWindow::~MainWindow()
{
	m->audio_in.requestInterruption();
	m->audio_in.wait();
	delete m;
	delete ui;
}



void MainWindow::onUpdatePeak(int v)
{
	v = v * 100 / 32768;
	if (v > 100) {
		v = 100;
	} else if (v < 0) {
		v = 0;
	}
	ui->progressBar->setRange(0, 100);
	ui->progressBar->setValue(v);
}

