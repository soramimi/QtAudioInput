#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QAudioInput>
#include <QDebug>
#include <QThread>
#include <QTime>
#include <memory>

struct MainWindow::Private {
    std::shared_ptr<QAudioInput> source;
    QIODevice *device;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m(new Private)
{
    ui->setupUi(this);

    QAudioFormat format;
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setSampleRate(8000);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    m->source = std::shared_ptr<QAudioInput>(new QAudioInput(format));
    m->device = m->source->start();

    connect(m->device, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

MainWindow::~MainWindow()
{
    delete m;
    delete ui;
}

void MainWindow::updatePeak(int v)
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

void MainWindow::onReadyRead()
{
    int peak = 0;

    int n = m->source->bytesReady();
    n /= 2;
    if (n >= 80) {
        for (int i = 0; i < n; i++) {
            int16_t v;
            m->device->read((char *)&v, 2);
            if (v < 0) {
                v = -v;
            }
            if (peak < v) {
                peak = v;
            }
        }

        updatePeak(peak);
    }
}
