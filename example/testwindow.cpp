#include "testwindow.h"

TestWindow::TestWindow()
{
	hexEdit = new QHexEdit2(this);
	QFont font("Noto Mono", 10);
	hexEdit->setFont(font);
	this->resize(800, 600);
	setCentralWidget(hexEdit);

	hexEdit->setAddressAreaColor(QColor(46, 46, 46));
	hexEdit->setAddressAreaFontColor(QColor(132, 132, 132));
	hexEdit->setBreakpointFieldColor(QColor(18, 18, 17));
	hexEdit->setHexFieldColor(QColor(48, 48, 48));
	hexEdit->setAddressOffset(0);
	hexEdit->setAddressWidth(4);
	hexEdit->setBytesPerLine(16);
	hexEdit->setAsciiArea(true);

	file.setFileName("inzynierka.txt");
	hexEdit->setData(file);
}

TestWindow::~TestWindow()
{
}
