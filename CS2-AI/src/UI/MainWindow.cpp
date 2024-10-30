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

	cs2_runner_thread = new QThread();
	cs2_runner = new CS2Runner();
	cs2_runner->moveToThread(cs2_runner_thread);

	ui->editor_tab_layout->addWidget(new NavmeshEditorWidget(cs2_runner, this));

	connect(cs2_runner_thread, &QThread::started, cs2_runner, &CS2Runner::run);
	connect(cs2_runner, &CS2Runner::finished, cs2_runner_thread, &QThread::quit);
	connect(cs2_runner, &CS2Runner::finished, cs2_runner, &CS2Runner::deleteLater);
	connect(cs2_runner_thread, &QThread::finished, cs2_runner_thread, &QThread::deleteLater);
	cs2_runner_thread->start();
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

	cs2_runner->set_activated_behavior(features);
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
	cs2_runner->load_config();
	cs2_runner->load_offsets();
	cs2_runner->load_navmesh();
}

void MainWindow::on_button_reattach_clicked()
{
	cs2_runner->attach_to_process();
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
	cs2_runner->set_add_point_key(key_code);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	ModeRunning mode = ModeRunning::NONE;
	if ((SelectedTab)index == SelectedTab::AI)
		mode = ModeRunning::AI;
	else if ((SelectedTab)index == SelectedTab::POINTS)
		mode = ModeRunning::POINT_CREATOR;

	cs2_runner->set_mode(mode);
}

void MainWindow::on_button_save_points_clicked()
{
	if (this->cs2_runner->save_navmesh_points())
		Logging::log_success("File successfully saved");
	else
		Logging::log_error("Error saving file");
}

void MainWindow::on_button_add_point_clicked()
{
	cs2_runner->add_point();
}

void MainWindow::on_button_reattach_2_clicked()
{
	cs2_runner->attach_to_process();
}
