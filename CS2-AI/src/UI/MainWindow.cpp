#include "UI/MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);

	m_box_logger = std::make_unique<QTBoxLogger>(
		std::initializer_list<QTextEdit*>({ m_ui->textEdit_output, m_ui->textEdit_point_output }));
	Logging::set_logger(m_box_logger.get());

	m_log_updater = new QTimer(this);
	connect(m_log_updater, &QTimer::timeout, this, &MainWindow::update_logger);
	m_log_updater->start();

	m_cs2_runner_thread = new QThread();
	m_cs2_runner = new CS2Runner();
	m_cs2_runner->moveToThread(m_cs2_runner_thread);

	m_ui->editor_tab_layout->addWidget(new NavmeshEditorWidget(m_cs2_runner, this));

	connect(m_cs2_runner_thread, &QThread::started, m_cs2_runner, &CS2Runner::run);
	connect(m_cs2_runner, &CS2Runner::finished, m_cs2_runner_thread, &QThread::quit);
	connect(m_cs2_runner, &CS2Runner::finished, m_cs2_runner, &CS2Runner::deleteLater);
	connect(m_cs2_runner_thread, &QThread::finished, m_cs2_runner_thread, &QThread::deleteLater);
	m_cs2_runner_thread->start();
}

MainWindow::~MainWindow()
{
	delete m_log_updater;
	delete m_ui;
}

void MainWindow::update_behavior_executed()
{
	ActivatedFeatures features;
	features.triggerBot = m_ui->checkBox_triggerbot->isChecked();
	features.aimbot = m_ui->checkBox_aimbot->isChecked();
	features.movement = m_ui->checkBox_movement->isChecked();

	m_cs2_runner->set_activated_behavior(features);
}

void MainWindow::update_logger()
{
	m_box_logger->update();
}

bool MainWindow::all_behavior_checkboxes_checked()
{
	return all_checked({ m_ui->checkBox_aimbot, m_ui->checkBox_triggerbot, m_ui->checkBox_movement });
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
	if (all_behavior_checkboxes_checked() && m_ui->checkBox_ai->isChecked())
	{
		set_enabled(false, { m_ui->checkBox_aimbot, m_ui->checkBox_movement, m_ui->checkBox_triggerbot });
		return;
	}

	if (m_ui->checkBox_ai->isChecked())
	{
		set_checked(true, { m_ui->checkBox_aimbot, m_ui->checkBox_movement, m_ui->checkBox_triggerbot });
		set_enabled(false, { m_ui->checkBox_aimbot, m_ui->checkBox_movement, m_ui->checkBox_triggerbot });
	}
	else
	{
		set_checked(false, { m_ui->checkBox_aimbot, m_ui->checkBox_movement, m_ui->checkBox_triggerbot });
		set_enabled(true, { m_ui->checkBox_aimbot, m_ui->checkBox_movement, m_ui->checkBox_triggerbot });
	}
}

void MainWindow::on_checkBox_aimbot_stateChanged()
{
	if (all_behavior_checkboxes_checked())
		m_ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_checkBox_movement_stateChanged()
{
	if (all_behavior_checkboxes_checked())
		m_ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_checkBox_triggerbot_stateChanged()
{
	if (all_behavior_checkboxes_checked())
		m_ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_button_reload_files_clicked()
{
	m_cs2_runner->load_config();
	m_cs2_runner->load_offsets();
	m_cs2_runner->load_navmesh();
}

void MainWindow::on_button_reattach_clicked()
{
	m_cs2_runner->attach_to_process();
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
	m_cs2_runner->set_add_point_key(key_code);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	ModeRunning mode = ModeRunning::NONE;
	if ((SelectedTab)index == SelectedTab::AI)
		mode = ModeRunning::AI;
	else if ((SelectedTab)index == SelectedTab::POINTS)
		mode = ModeRunning::POINT_CREATOR;

	m_cs2_runner->set_mode(mode);
}

void MainWindow::on_button_save_points_clicked()
{
	if (m_cs2_runner->save_navmesh_points())
		Logging::log_success("File successfully saved");
	else
		Logging::log_error("Error saving file");
}

void MainWindow::on_button_add_point_clicked()
{
	m_cs2_runner->add_point();
}

void MainWindow::on_button_reattach_2_clicked()
{
	m_cs2_runner->attach_to_process();
}
