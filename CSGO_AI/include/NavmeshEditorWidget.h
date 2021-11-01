#pragma once
#include <memory>
#include <qfiledialog.h>
#include <iostream>
#include <qwidget.h>
#include "ui_NavmeshEditorWidget.h"
#include "NavmeshEditor.h"

class NavmeshEditorWidget : public QWidget 
{
	Q_OBJECT;
public:
	explicit NavmeshEditorWidget(QWidget* parent = nullptr);
	~NavmeshEditorWidget();

private:
	Ui::NavmeshEditorWidget* ui;
	std::unique_ptr<NavmeshEditor> navmesh_editor;

private slots:
	void on_button_load_map_clicked();
};