#pragma once
#include <qevent.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qscrollarea.h>
#include <Logging.h>
#include <qlineedit.h>
#include <fstream>
#include <vector>
#include "Vec3D.h"
#include "Vec2D.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Editor
{
	struct Node
	{
		int id;
		bool corner;
		Vec3D<float> pos;
		Vec2D<int> canvas_pos;
	};

	struct Edge
	{
		int from;
		int to;
		float weight;
	};
}


class NavmeshEditor : public QScrollArea
{
	enum class State
	{
		NONE, PLACE_CORNERS, ADD_EDGES
	};

	Q_OBJECT

public:
	explicit NavmeshEditor(QWidget* parent, QLineEdit* output);
	~NavmeshEditor();

	void left_clicked(QMouseEvent* event);
	void draw_editor_map(const QPoint& mouse_pos);
	void load_image(const QString& file_name);
	void load_navmesh(const QString& file_name);
	void place_corner_points();
	void save_navmesh();
protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	void zoom(double factor);
	void output(const QString& message);
	void output_error(const QString& message);
	void output(const QString& message, Qt::GlobalColor color);
	void load_nodes(const json& navmesh_json);
	void load_edges(const json& navmesh_json);

	QLabel* displayed_map = nullptr;
	QImage image;
	double zoom_factor = 1;
	State editor_state = State::NONE;
	QLineEdit* output_line = nullptr;
	bool corner_nodes_placed = false;
	bool valid_json_loaded = false;
	std::vector<Editor::Node> nodes;
	std::vector<Editor::Edge> edges;
};