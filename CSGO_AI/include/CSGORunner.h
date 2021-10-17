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
    CSGORunner(std::shared_ptr<CSGOAi> csgo_ai_handler);
    void set_run_navmesh_points(bool value);
    void set_add_point_key(int key_code);
    bool save_navmesh_points();
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