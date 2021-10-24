#pragma once
#include <iostream>
#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <qdebug.h>
#include <string>
#include <math.h>
#include <qmutex.h>
#include <qthread.h>
#include <qfiledialog.h>
#include <qpainter.h>
#include <memory>
#include <qevent.h>
#include "NavmeshEditor.h"
#include "CSGOAI.h"
#include "ui_MainWindow.h"
#include "NavmeshPoints.h"
#include "CSGORunner.h"
#include "Logging.h"
#include "QTBoxLogger.h"

enum class SelectedTab
{
    AI = 0, POINTS = 1, EDITOR = 2
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    bool is_navmesh_editor_tab_selected() const;
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    bool all_behavior_checkboxes_checked();
    void update_behavior_executed();
    bool all_checked(std::initializer_list<QCheckBox*> checkboxes);
    void set_checked(bool value, std::initializer_list<QCheckBox*> checkboxes);
    void set_enabled(bool value, std::initializer_list<QCheckBox*> checkboxes);

    Ui::MainWindow* ui;
    QThread* csgo_runner_thread = nullptr;
    CSGORunner* csgo_runner = nullptr;
    std::unique_ptr<NavmeshEditor> navmesh_editor = nullptr;
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
    void on_button_load_navmesh_clicked();
};
