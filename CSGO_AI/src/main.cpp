#pragma once
#include <QApplication>
#include <utility>
#include "UI/MainWindow.h"
#include "CSGO/CSGOAI.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWindow w;
	w.show();
	return app.exec();
}