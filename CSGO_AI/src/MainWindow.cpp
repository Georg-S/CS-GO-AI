#include "MainWindow.h"


CSGORunner::CSGORunner(std::shared_ptr<CSGOAi> csgo_ai_handler) : QObject(nullptr), csgo_ai_handler(csgo_ai_handler)
{
	this->csgo_navmesh_points_handler = std::make_shared<NavmeshPoints>(csgo_ai_handler->get_game_info_handler());
}

void CSGORunner::run()
{
	while (is_running)
	{
		if (run_navmesh_points)
		{
			mutex.lock();
			bool new_point_added = csgo_navmesh_points_handler->update();
			mutex.unlock();
			if (new_point_added)
				emit new_point();
		}
		else
		{
			csgo_ai_handler->update();
		}
	}

	deleteLater();
}

void CSGORunner::set_run_navmesh_points(bool value)
{
	this->run_navmesh_points = value;
}

void CSGORunner::set_add_point_key(int key_code)
{
	csgo_navmesh_points_handler->set_add_point_button(key_code);
}

bool CSGORunner::save_navmesh_points()
{
	mutex.lock();
	bool success = this->csgo_navmesh_points_handler->save_to_file();
	mutex.unlock();

	return success;
}

void CSGORunner::add_point()
{
	mutex.lock();
	csgo_navmesh_points_handler->add_point();
	mutex.unlock();
	emit new_point(); // emit outside of mutex, since we call it from the same thread 
}

Vec3D<float> CSGORunner::get_latest_point()
{
	mutex.lock();
	auto result = csgo_navmesh_points_handler->get_latest_point();
	mutex.unlock();

	return result;
}




MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	csgo_ai_handler = std::make_shared<CSGOAi>();
	init_csgo_ai();

	csgo_runner_thread = new QThread();
	csgo_runner = new CSGORunner(csgo_ai_handler);
	csgo_runner->moveToThread(csgo_runner_thread);

	connect(csgo_runner, &CSGORunner::new_point, this, &MainWindow::print_newest_point);
	connect(csgo_runner_thread, &QThread::started, csgo_runner, &CSGORunner::run);
	connect(csgo_runner, &CSGORunner::finished, csgo_runner_thread, &QThread::quit);
	connect(csgo_runner, &CSGORunner::finished, csgo_runner, &CSGORunner::deleteLater);
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
	csgo_runner->set_add_point_key(key_code);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	constexpr int ai_tab = 0;
	constexpr int point_tab = 1;

	csgo_runner->set_run_navmesh_points(index == point_tab);
}

void MainWindow::on_button_save_points_clicked()
{
	if (this->csgo_runner->save_navmesh_points())
		output_success("File successfully saved");
	else
		output_error("Error saving file");
}

void MainWindow::on_button_add_point_clicked()
{
	csgo_runner->add_point();
}

void MainWindow::print_newest_point()
{
	Vec3D<float> buf = csgo_runner->get_latest_point();

	ui->textEdit_point_output->setTextColor(Qt::black);
	ui->textEdit_point_output->append("Point added: X: " + QString::number(buf.x) + " Y: " + QString::number(buf.y) + " Z: " + QString::number(buf.z));
}
