#pragma once
#include <qobject.h>
#include <string>
#include <math.h>
#include <qmutex.h>
#include <qthread.h>
#include <iostream>
#include <memory>
#include "CSGOAI.h"
#include "NavmeshPoints.h"

class CSGORunner : public QObject
{
    Q_OBJECT

public:
    CSGORunner();
    void update();
    void set_run_navmesh_points(bool value);
    void set_add_point_key(int key_code);
    bool save_navmesh_points();
    void add_point();
    void load_files();
    void attach_to_process();
    void set_activated_behavior(const ActivatedFeatures& behavior);

signals:
    void finished();
public slots:
    void run();

private:
    QMutex mutex;
    bool is_running = true;
    bool run_navmesh_points = false;
    std::unique_ptr<CSGOAi> csgo_ai_handler = nullptr;
    std::unique_ptr<NavmeshPoints> csgo_navmesh_points_handler = nullptr;
};