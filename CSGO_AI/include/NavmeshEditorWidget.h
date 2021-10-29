#pragma once
#include <qwidget.h>
#include "ui_NavmeshEditorWidget.h"

class NavmeshEditorWidget : public QWidget 
{
	Q_OBJECT;
public:
	explicit NavmeshEditorWidget(QWidget* parent = nullptr);
	~NavmeshEditorWidget();


private:
	Ui::NavmeshEditorWidget* ui;
};