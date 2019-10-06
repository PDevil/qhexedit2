#pragma once

#include "qhexedit2.h"

#include <QMainWindow>

class TestWindow : public QMainWindow
{
	Q_OBJECT

public:
	TestWindow();
	~TestWindow();

private:
	QHexEdit2* hexEdit;
	QFile file;
};
