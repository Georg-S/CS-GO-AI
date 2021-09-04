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

class CSGORunner : public QObject
{
    Q_OBJECT

public:
    CSGORunner(std::shared_ptr<CSGOAi> csgo_ai_handler);

public slots:
    /// This is the method which runs in the thread.
    void run();
signals:
    /// A child process exited normally.
    void exited(int ospid, int code);
    void finished();

private:
    QMutex mutex;
    bool isRunning = true;
    std::shared_ptr<CSGOAi> csgo_ai_handler = nullptr;
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

private slots:
    void on_checkBox_ai_stateChanged();
    void on_checkBox_aimbot_stateChanged();
    void on_checkBox_movement_stateChanged();
    void on_checkBox_triggerbot_stateChanged();
    void on_button_reload_files_clicked();
    void on_button_reattach_clicked();

signals:
    void stopped();
};
