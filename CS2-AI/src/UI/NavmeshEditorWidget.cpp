#include "UI/NavmeshEditorWidget.h"

NavmeshEditorWidget::NavmeshEditorWidget(CS2Runner* cs2_runner, QWidget* parent) : QWidget(parent), m_ui(new Ui::NavmeshEditorWidget)
{
	m_ui->setupUi(this);
	m_navmesh_editor = std::make_unique<NavmeshEditor>(this, m_ui->lineEdit_navmesh_output);
	m_ui->base_layout->addWidget(m_navmesh_editor.get());
	m_cs2_runner = cs2_runner;
}

NavmeshEditorWidget::~NavmeshEditorWidget()
{
	delete m_ui;
}

void NavmeshEditorWidget::on_button_load_navmesh_clicked()
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath() + "/Navmesh/json");
	if (file_name.isEmpty())
		return;

	m_navmesh_editor->load_navmesh(file_name);
}

void NavmeshEditorWidget::on_button_place_corner_points_clicked()
{
	m_navmesh_editor->place_corner_points();
}

void NavmeshEditorWidget::on_button_save_navmesh_clicked()
{
	m_navmesh_editor->save_navmesh();
}

void NavmeshEditorWidget::on_button_create_node_from_game_pos_clicked()
{
	auto res = m_cs2_runner->get_current_position();
	if (res.first)
		m_navmesh_editor->add_node(res.second);
}

void NavmeshEditorWidget::on_button_load_map_clicked() 
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath() + "/Navmesh/map_images");
	if (file_name.isEmpty())
		return;

	m_navmesh_editor->load_image(file_name);
}
