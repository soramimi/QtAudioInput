#include "MyAudioInput.h"

#include <QAudioInput>
#include <stdint.h>

void MyAudioInput::bind(MainWindow *mw)
{
	mainwindow = mw;
}

void MyAudioInput::run()
{
	QAudioFormat format;
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setChannelCount(1);
	format.setCodec("audio/pcm");
	format.setSampleRate(8000);
	format.setSampleSize(16);
	format.setSampleType(QAudioFormat::SignedInt);
    QAudioInput source(format);
    QIODevice *device = source.start();

	while (1) {
		if (isInterruptionRequested()) break;

		int peak = INT_MIN;

		while (1) {
            int n = source.bytesReady();
			n /= 2;
			if (n < 400) break;
			for (int i = 0; i < n; i++) {
				int16_t v;
				device->read((char *)&v, 2);
				if (v < 0) {
					v = -v;
				}
				if (peak < v) {
					peak = v;
				}
			}
		}

		if (peak != INT_MIN) {
			emit updatePeak(peak);
		}

		msleep(10);
	}

}
