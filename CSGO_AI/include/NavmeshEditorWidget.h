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
	void on_button_load_navmesh_clicked();
	void on_button_place_corner_points_clicked();
	void on_button_save_navmesh_clicked();
};