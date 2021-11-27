#pragma once
#include <qobject.h>
#include <string>
#include <math.h>
#include <qmutex.h>
#include <qthread.h>
#include <iostream>
#include <memory>
#include "CSGO/CSGOAI.h"
#include "CSGO/NavmeshPoints.h"

enum class ModeRunning
{
    NONE = 0, AI, POINT_CREATOR
};

class CSGORunner : public QObject
{
    Q_OBJECT

public:
    CSGORunner();
    void update();
    void set_mode(ModeRunning mode);
    void set_add_point_key(int key_code);
    bool save_navmesh_points();
    void add_point();
    void load_config();
    void load_offsets();
    void load_navmesh();
    void attach_to_process();
    void set_activated_behavior(const ActivatedFeatures& behavior);

signals:
    void finished();
public slots:
    void run();

private:
    QMutex mutex;
    ModeRunning mode = ModeRunning::NONE;
    bool is_running = true;
    std::unique_ptr<CSGOAi> csgo_ai_handler = nullptr;
    std::unique_ptr<NavmeshPoints> csgo_navmesh_points_handler = nullptr;
};