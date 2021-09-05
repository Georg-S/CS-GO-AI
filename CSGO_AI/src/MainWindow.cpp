#include "MainWindow.h"


CSGORunner::CSGORunner(std::shared_ptr<CSGOAi> csgo_ai_handler) : QObject(nullptr), csgo_ai_handler(csgo_ai_handler)
{
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	csgo_ai_handler = std::make_shared<CSGOAi>();
	init_csgo_ai();

	csgo_runner_thread = new QThread();
	csgoRunner = new CSGORunner(csgo_ai_handler);
	csgoRunner->moveToThread(csgo_runner_thread);

	connect(csgo_runner_thread, &QThread::started, csgoRunner, &CSGORunner::run);
	connect(csgoRunner, &CSGORunner::finished, csgo_runner_thread, &QThread::quit);
	connect(csgoRunner, &CSGORunner::finished, csgoRunner, &CSGORunner::deleteLater);
	connect(csgo_runner_thread, &QThread::finished, csgo_runner_thread, &QThread::deleteLater);
	csgo_runner_thread->start();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::output_error(const QString& string)
{
	ui->textEdit_output->setTextColor(Qt::red);
	ui->textEdit_output->append(string);

	ui->textEdit_point_output->setTextColor(Qt::red);
	ui->textEdit_point_output->append(string);
}

void MainWindow::output_success(const QString& string)
{
	ui->textEdit_output->setTextColor(Qt::green);
	ui->textEdit_output->append(string);

	ui->textEdit_point_output->setTextColor(Qt::green);
	ui->textEdit_point_output->append(string);
}

void MainWindow::init_csgo_ai()
{
	load_files();
	attach_to_process();
}

bool MainWindow::all_three_checkboxes_checked()
{
	return ui->checkBox_aimbot->isChecked() && ui->checkBox_triggerbot->isChecked() && ui->checkBox_movement->isChecked();
}

void MainWindow::update_behavior_executed()
{
	ActivatedFeatures features;
	features.triggerBot = ui->checkBox_triggerbot->isChecked();
	features.aimbot = ui->checkBox_aimbot->isChecked();
	features.movement = ui->checkBox_movement->isChecked();

	csgo_ai_handler->set_activated_behavior(features);
}

void MainWindow::attach_to_process()
{
	if (!csgo_ai_handler->attach_to_csgo_process())
		output_error("Error getting dll address / Error attaching to CS-GO process");
	else
		output_success("Attached to the CSGO process");
}

void MainWindow::load_files()
{
	if (!csgo_ai_handler->load_config("config.json"))
		output_error("Config couldn't be read, make sure you have a valid config");
	if (!csgo_ai_handler->load_offsets("offsets.json"))
		output_error("Offsets couldn't be read, make sure you have a valid offsets file");
	if (!csgo_ai_handler->load_navmesh("nav_mesh.json"))
		output_error("Error loading / parsing Navmesh, make sure you have a valid nav - mesh file");
}

void MainWindow::on_checkBox_ai_stateChanged()
{
	if (all_three_checkboxes_checked() && ui->checkBox_ai->isChecked())
	{
		ui->checkBox_aimbot->setEnabled(false);
		ui->checkBox_movement->setEnabled(false);
		ui->checkBox_triggerbot->setEnabled(false);
		return;
	}

	if (ui->checkBox_ai->isChecked())
	{
		ui->checkBox_aimbot->setChecked(true);
		ui->checkBox_aimbot->setEnabled(false);
		ui->checkBox_movement->setChecked(true);
		ui->checkBox_movement->setEnabled(false);
		ui->checkBox_triggerbot->setChecked(true);
		ui->checkBox_triggerbot->setEnabled(false);
	}
	else
	{
		ui->checkBox_aimbot->setChecked(false);
		ui->checkBox_aimbot->setEnabled(true);
		ui->checkBox_movement->setChecked(false);
		ui->checkBox_movement->setEnabled(true);
		ui->checkBox_triggerbot->setChecked(false);
		ui->checkBox_triggerbot->setEnabled(true);
	}
}

void MainWindow::on_checkBox_aimbot_stateChanged()
{
	if (all_three_checkboxes_checked())
		ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_checkBox_movement_stateChanged()
{
	if (all_three_checkboxes_checked())
		ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_checkBox_triggerbot_stateChanged()
{
	if (all_three_checkboxes_checked())
		ui->checkBox_ai->setChecked(true);

	update_behavior_executed();
}

void MainWindow::on_button_reload_files_clicked()
{
	load_files();
}

void MainWindow::on_button_reattach_clicked()
{
	attach_to_process();
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
}

void MainWindow::on_tabWidget_currentChanged(int index)
{


	output_success(QString(index));
}

void CSGORunner::run()
{
	while (is_running) 
	{
		csgo_ai_handler->update();
	}

	deleteLater();
}
