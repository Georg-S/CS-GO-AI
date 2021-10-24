#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	Logging::set_logger(std::make_unique<QTBoxLogger>(ui->textEdit_output, ui->textEdit_point_output));
	navmesh_editor = std::make_unique<NavmeshEditor>(this);
	this->ui->editor_tab_layout->addWidget(navmesh_editor.get());


	csgo_runner_thread = new QThread();
	csgo_runner = new CSGORunner();
	csgo_runner->moveToThread(csgo_runner_thread);

	connect(csgo_runner_thread, &QThread::started, csgo_runner, &CSGORunner::run);
	connect(csgo_runner, &CSGORunner::finished, csgo_runner_thread, &QThread::quit);
	connect(csgo_runner, &CSGORunner::finished, csgo_runner, &CSGORunner::deleteLater);
	connect(csgo_runner_thread, &QThread::finished, csgo_runner_thread, &QThread::deleteLater);
	csgo_runner_thread->start();
}

bool MainWindow::is_navmesh_editor_tab_selected() const
{
	return (SelectedTab)ui->tabWidget->currentIndex() == SelectedTab::EDITOR;
}

MainWindow::~MainWindow()
{
	delete ui;
}



void MainWindow::update_behavior_executed()
{
	ActivatedFeatures features;
	features.triggerBot = ui->checkBox_triggerbot->isChecked();
	features.aimbot = ui->checkBox_aimbot->isChecked();
	features.movement = ui->checkBox_movement->isChecked();

	csgo_runner->set_activated_behavior(features);
}

bool MainWindow::all_behavior_checkboxes_checked()
{
	return all_checked({ ui->checkBox_aimbot, ui->checkBox_triggerbot, ui->checkBox_movement });
}

bool MainWindow::all_checked(std::initializer_list<QCheckBox*> checkboxes)
{
	if (checkboxes.size() == 0)
		return false;

	bool all_checked = true;
	for (QCheckBox* box : checkboxes)
		all_checked = all_checked && box->isChecked();

	return all_checked;
}

void MainWindow::set_checked(bool value, std::initializer_list<QCheckBox*> checkboxes)
{
	for (QCheckBox* elem : checkboxes)
		elem->setChecked(value);
}

void MainWindow::set_enabled(bool value, std::initializer_list<QCheckBox*> checkboxes)
{
	for (QCheckBox* box : checkboxes)
		box->setEnabled(value);
}

void MainWindow::on_checkBox_ai_stateChanged()
{
	if (all_behavior_checkboxes_checked() && ui->checkBox_ai->isChecked())
	{
		set_enabled(false, { ui->checkBox_aimbot, ui->checkBox_movement, ui->checkBox_triggerbot });
		return;
	}

	if (ui->checkBox_ai->isChecked())
	{
		set_checked(true, { ui->checkBox_aimbot, ui->checkBox_movement, ui->checkBox_triggerbot });
		set_enabled(false, { ui->checkBox_aimbot, ui->checkBox_movement, ui->checkBox_triggerbot });
	}
	else
	{
		set_checked(false, { ui->checkBox_aimbot, ui->checkBox_movement, ui->checkBox_triggerbot });
		set_enabled(true, { ui->checkBox_aimbot, ui->checkBox_movement, ui->checkBox_triggerbot });
	}
}

void MainWindow::on_checkBox_aimbot_stateChanged()
{
	if (all_behavior_checkboxes_checked())
		ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_checkBox_movement_stateChanged()
{
	if (all_behavior_checkboxes_checked())
		ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_checkBox_triggerbot_stateChanged()
{
	if (all_behavior_checkboxes_checked())
		ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_button_reload_files_clicked()
{
	csgo_runner->load_files();
}

void MainWindow::on_button_reattach_clicked()
{
	csgo_runner->attach_to_process();
}

void MainWindow::on_lineEdit_keycode_textChanged(const QString& str)
{
	if (str.size() == 0)
		return;

	auto get_key_code_from_char = [](char c)
	{
		HKL currentKBL = GetKeyboardLayout(0);
		return VkKeyScanExA(c, currentKBL);
	};

	std::string buf = str.toStdString();
	int key_code = get_key_code_from_char(buf.at(0));
	csgo_runner->set_add_point_key(key_code);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	csgo_runner->set_run_navmesh_points((SelectedTab)index == SelectedTab::POINTS);
}

void MainWindow::on_button_save_points_clicked()
{
	if (this->csgo_runner->save_navmesh_points())
		Logging::log_success("File successfully saved");
	else
		Logging::log_error("Error saving file");
}

void MainWindow::on_button_add_point_clicked()
{
	csgo_runner->add_point();
}

void MainWindow::on_button_reattach_2_clicked()
{
	csgo_runner->attach_to_process();
}

void MainWindow::on_button_load_navmesh_clicked()
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
	if (file_name.isEmpty())
		return;

	navmesh_editor->load_file(file_name);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{

}
