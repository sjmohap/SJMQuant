#include <SJMHttpBundle/SJMCommandlineTextProgressBar.h>
#include <QByteArray>
#include <cstdio>

void SJMCommandlineTextProgressBar::clear()
{
	printf("\n");
	fflush(stdout);

	value = 0;
	maximum = -1;
	iteration = 0;
}

void SJMCommandlineTextProgressBar::update()
{
	++iteration;

	if (maximum > 0)
	{
		// we know the maximum
		// draw a progress bar
		const int percent = value * 100 / maximum;
		const int hashes = percent / 2;

		QByteArray progressbar(hashes, '#');
		if (percent % 2)
			progressbar += '>';

		printf("\r[%-50s] %3d%% %s     ",
			progressbar.constData(),
			percent,
			qPrintable(message));
	}
	else
	{
		// we don't know the maximum, so we can't draw a progress bar
		const int center = (iteration % 48) + 1; // 50 spaces, minus 2
		QByteArray before(qMax(center - 2, 0), ' ');
		QByteArray after(qMin(center + 2, 50), ' ');

		printf("\r[%s###%s]      %s      ",
			before.constData(), after.constData(), qPrintable(message));
	}
}

void SJMCommandlineTextProgressBar::setMessage(const QString& msg)
{
	message = msg;
}

void SJMCommandlineTextProgressBar::setStatus(qint64 val, qint64 max)
{
	value = val;
	maximum = max;
}
