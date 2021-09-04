#pragma once
#include <QApplication>
#include <utility>
#include "MainWindow.h"
#include "CSGOAI.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWindow w;
	w.show();
	return app.exec();
}