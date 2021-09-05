#pragma once
#include <iostream>
#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <qdebug.h>
#include <string>
#include <math.h>
#include <qmutex.h>
#include <qthread.h>
#include <memory>
#include "CSGOAI.h"
#include "ui_MainWindow.h"
#include "NavmeshPoints.h"

class CSGORunner : public QObject
{
    Q_OBJECT

public:
    CSGORunner(std::shared_ptr<CSGOAi> csgo_ai_handler);
    void set_run_navmesh_points(bool value);
    void set_add_point_key(int key_code);
    void save_navmesh_points();
    void add_point();
    Vec3D<float> get_latest_point();

signals:
    void finished();
    void new_point();
public slots:
    void run();

private:
    QMutex mutex;
    bool is_running = true;
    bool run_navmesh_points = false;
    std::shared_ptr<CSGOAi> csgo_ai_handler = nullptr;
    std::shared_ptr<NavmeshPoints> csgo_navmesh_points_handler = nullptr;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void output_error(const QString& string);
    void output_success(const QString& string);
    void init_csgo_ai();
    bool all_three_checkboxes_checked();
    void update_behavior_executed();
    void attach_to_process();
    void load_files();

    Ui::MainWindow* ui;
    QThread* csgo_runner_thread = nullptr;
    CSGORunner* csgoRunner = nullptr;
    std::shared_ptr<CSGOAi> csgo_ai_handler = nullptr;

signals:
    void stopped();
public slots:
    void print_newest_point();
private slots:
    void on_checkBox_ai_stateChanged();
    void on_checkBox_aimbot_stateChanged();
    void on_checkBox_movement_stateChanged();
    void on_checkBox_triggerbot_stateChanged();
    void on_button_reload_files_clicked();
    void on_button_reattach_clicked();
    void on_lineEdit_keycode_textChanged(const QString& str);
    void on_tabWidget_currentChanged(int index);
    void on_button_save_points_clicked();
    void on_button_add_point_clicked();
};
