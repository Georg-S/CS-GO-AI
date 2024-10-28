#pragma once
#include <iostream>
#include <QtWidgets/QMainWindow>
#include <string>
#include <math.h>
#include <qthread.h>
#include <memory>
#include <qtimer.h>
#include "CS2/CS2AI.h"
#include "ui_MainWindow.h"
#include "CS2/NavmeshPoints.h"
#include "CS2Runner.h"
#include "Utility/Logging.h"
#include "QTBoxLogger.h"
#include "NavmeshEditorWidget.h"

enum class SelectedTab
{
    AI = 0, POINTS = 1, EDITOR = 2
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    bool all_behavior_checkboxes_checked();
    void update_behavior_executed();
    void update_logger();
    bool all_checked(std::initializer_list<QCheckBox*> checkboxes);
    void set_checked(bool value, std::initializer_list<QCheckBox*> checkboxes);
    void set_enabled(bool value, std::initializer_list<QCheckBox*> checkboxes);

    Ui::MainWindow* ui = nullptr;
    QThread* csgo_runner_thread = nullptr;
    CS2Runner* csgo_runner = nullptr;
    QTimer* log_updater = nullptr;
    std::unique_ptr<QTBoxLogger> box_logger = nullptr;
signals:
    void stopped();
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
