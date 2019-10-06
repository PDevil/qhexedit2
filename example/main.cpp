#include <QApplication>
#include <QIcon>

#include "mainwindow.h"
#include <testwindow.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qhexedit);
    QApplication app(argc, argv);
    app.setApplicationName("QHexEdit");
    app.setOrganizationName("QHexEdit");
    app.setWindowIcon(QIcon(":images/qhexedit.ico"));

    // Identify locale and load translation if available
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("qhexedit_") + locale);
    app.installTranslator(&translator);

#if 1
	MainWindow *mainWin = new MainWindow;
	mainWin->show();

	TestWindow* testWnd = new TestWindow();
	testWnd->show();
#endif

    return app.exec();
}
