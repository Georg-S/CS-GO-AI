#include "UI/MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	box_logger = std::make_unique<QTBoxLogger>(
		std::initializer_list<QTextEdit*>({ ui->textEdit_output, ui->textEdit_point_output }));
	Logging::set_logger(box_logger.get());

	log_updater = new QTimer(this);
	connect(log_updater, &QTimer::timeout, this, &MainWindow::update_logger);
	log_updater->start();

	csgo_runner_thread = new QThread();
	csgo_runner = new CSGORunner();
	csgo_runner->moveToThread(csgo_runner_thread);

	ui->editor_tab_layout->addWidget(new NavmeshEditorWidget(csgo_runner, this));

	connect(csgo_runner_thread, &QThread::started, csgo_runner, &CSGORunner::run);
	connect(csgo_runner, &CSGORunner::finished, csgo_runner_thread, &QThread::quit);
	connect(csgo_runner, &CSGORunner::finished, csgo_runner, &CSGORunner::deleteLater);
	connect(csgo_runner_thread, &QThread::finished, csgo_runner_thread, &QThread::deleteLater);
	csgo_runner_thread->start();
}

MainWindow::~MainWindow()
{
	delete log_updater;
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

void MainWindow::update_logger()
{
	box_logger->update();
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
	csgo_runner->load_config();
	csgo_runner->load_offsets();
	csgo_runner->load_navmesh();
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
	ModeRunning mode = ModeRunning::NONE;
	if ((SelectedTab)index == SelectedTab::AI)
		mode = ModeRunning::AI;
	else if ((SelectedTab)index == SelectedTab::POINTS)
		mode = ModeRunning::POINT_CREATOR;

	csgo_runner->set_mode(mode);
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
