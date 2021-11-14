#include "UI/NavmeshEditorWidget.h"

NavmeshEditorWidget::NavmeshEditorWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NavmeshEditorWidget)
{
	ui->setupUi(this);
	navmesh_editor = std::make_unique<NavmeshEditor>(this, ui->lineEdit_navmesh_output);
	ui->base_layout->addWidget(navmesh_editor.get());
}

NavmeshEditorWidget::~NavmeshEditorWidget()
{
	delete ui;
}

void NavmeshEditorWidget::on_button_load_navmesh_clicked()
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath() + "/Navmesh/json");
	if (file_name.isEmpty())
		return;

	navmesh_editor->load_navmesh(file_name);
}

void NavmeshEditorWidget::on_button_place_corner_points_clicked()
{
	navmesh_editor->place_corner_points();
}

void NavmeshEditorWidget::on_button_save_navmesh_clicked()
{
	navmesh_editor->save_navmesh();
}

void NavmeshEditorWidget::on_button_load_map_clicked() 
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath() + "/Navmesh/map_images");
	if (file_name.isEmpty())
		return;

	navmesh_editor->load_image(file_name);
}
