#include "NavmeshEditorWidget.h"

NavmeshEditorWidget::NavmeshEditorWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NavmeshEditorWidget)
{
	ui->setupUi(this);
	navmesh_editor = std::make_unique<NavmeshEditor>(this);
	ui->base_layout->addWidget(navmesh_editor.get());
}

NavmeshEditorWidget::~NavmeshEditorWidget()
{
	delete ui;
}

void NavmeshEditorWidget::on_button_load_map_clicked() 
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
	if (file_name.isEmpty())
		return;

	navmesh_editor->load_file(file_name);
}
