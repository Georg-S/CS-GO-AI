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
#include "CSGORunner.h"
#include "QTBoxLogger.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void init_csgo_ai();
    bool all_three_checkboxes_checked();
    void update_behavior_executed();
    void attach_to_process();
    void load_files();

    Ui::MainWindow* ui;
    QThread* csgo_runner_thread = nullptr;
    CSGORunner* csgo_runner = nullptr;
    std::shared_ptr<CSGOAi> csgo_ai_handler = nullptr;
    std::shared_ptr<QTBoxLogger> logger = nullptr;

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
    void on_button_reattach_2_clicked();
};
