#include "NavmeshEditor.h"


NavmeshEditor::NavmeshEditor(QWidget* parent, QLineEdit* output_line) : QScrollArea(parent), output_line(output_line)
{
	displayed_map = new QLabel;
	displayed_map->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	displayed_map->setScaledContents(true);
	setWidget(displayed_map);
	this->viewport()->setMouseTracking(true);
}

void NavmeshEditor::left_clicked(QMouseEvent* event)
{
	if (image.isNull())
		return;

	auto mapped_pos = displayed_map->mapFromGlobal(QCursor::pos());
	auto scaled_cursor_pos = mapped_pos / zoom_factor;

	if (editor_state == State::PLACE_CORNER_1) 
	{
		corner_node_1->canvas_pos = Vec2D<int>(scaled_cursor_pos.x(), scaled_cursor_pos.y());
		corner_node_1->render = true;
		editor_state = State::PLACE_CORNER_2;
		output("Place second node: " + QString::fromStdString(corner_node_2->pos.to_string()));
	}
	else if (editor_state == State::PLACE_CORNER_2) 
	{
		corner_node_2->canvas_pos = Vec2D<int>(scaled_cursor_pos.x(), scaled_cursor_pos.y());
		corner_node_2->render = true;
		editor_state = State::ADD_EDGES;
		output_success("Corner nodes set: edges can be added");
		adjust_all_nodes();
	}
	render_editor();
}

NavmeshEditor::~NavmeshEditor()
{
	if (displayed_map)
		delete displayed_map;
}

void NavmeshEditor::render_editor()
{
	if (image.isNull())
		return;

	QPixmap map = QPixmap::fromImage(image);
	render_edges(map);
	render_nodes(map);
	displayed_map->setPixmap(map);
	displayed_map->resize(zoom_factor * map.size());
}

void NavmeshEditor::render_edges(QPixmap& map) 
{
	QPainter painter(&map);

	painter.setBrush(Qt::black);
	for (const auto& edge : edges)
	{
	}
}

void NavmeshEditor::render_nodes(QPixmap& map)
{
	QPainter painter(&map);

	painter.setBrush(Qt::red);
	for (const auto& node : nodes) 
	{
		if(node->render)
			painter.drawEllipse(QPoint(node->canvas_pos.x, node->canvas_pos.y), NODE_SIZE, NODE_SIZE);
	}
}

void NavmeshEditor::load_image(const QString& file_name)
{
	QImage img = QImage(file_name);
	if (img.isNull())
	{
		output_error("Could not load map image");
		return;
	}

	image = img;
	render_editor();
}

void NavmeshEditor::load_navmesh(const QString& file_name)
{
	auto std_file_name = file_name.toStdString();

	try
	{
		std::ifstream ifs(std_file_name);

		auto navmesh_json = json::parse(ifs);
		ifs.close();

		load_nodes(navmesh_json);
		load_edges(navmesh_json);
	}
	catch (std::exception const& e)
	{
		output_error("Could not load Navmesh: " + QString(e.what()));
	}
}

void NavmeshEditor::place_corner_points()
{
	set_corner_nodes();

	if (!corner_node_1 || !corner_node_2) 
	{
		output_error("Couldn't find two corner points, please load a valid navmesh");
		return;
	}

	editor_state = State::PLACE_CORNER_1;
	output("Place first node: " + QString::fromStdString(corner_node_1->pos.to_string()));
}

void NavmeshEditor::save_navmesh()
{
}

void NavmeshEditor::wheelEvent(QWheelEvent* event)
{
	QPoint movement = event->angleDelta();

	if (image.isNull() || (movement.x() != 0))
		return;

	bool up = (movement.y() > 0);
	if (up)
		zoom(1.1);
	else
		zoom(0.9); // Maybe adjust

	render_editor();
}

void NavmeshEditor::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		left_clicked(event);
	else if (event->button() == Qt::RightButton)
		;
}

void NavmeshEditor::zoom(double factor)
{
	constexpr double max_factor = 5;
	constexpr double min_factor = 0.2;

	zoom_factor *= factor;
	zoom_factor = std::min(zoom_factor, max_factor);
	zoom_factor = std::max(zoom_factor, min_factor);

	render_editor();
}

void NavmeshEditor::output(const QString& message)
{
	output(message, Qt::black);
}

void NavmeshEditor::output_error(const QString& message)
{
	output(message, Qt::red);
}

void NavmeshEditor::output(const QString& message, Qt::GlobalColor color)
{
	QPalette palette = QPalette();
	palette.setColor(QPalette::Text, color);

	output_line->setPalette(palette);
	output_line->setText(message);
}

void NavmeshEditor::load_nodes(const json& navmesh_json)
{
	if (!navmesh_json.contains("nodes"))
		return;

	nodes.clear();
	valid_json_loaded = false;
	int corner_count = 0;
	for (const auto& json_node : navmesh_json["nodes"])
	{
		Vec3D<float> pos;
		int id = json_node["id"];
		pos.x = json_node["x"];
		pos.y = json_node["y"];
		pos.z = json_node["z"];
		bool corner = json_node["corner"];

		if (corner)
			corner_count++;

		nodes.push_back(std::make_unique<Editor::Node>(id, corner, pos));
	}

	if (corner_count >= 2)
		valid_json_loaded = true;
	else
		output_error("Invalid Navmesh: No corner nodes defined");
}

void NavmeshEditor::load_edges(const json& navmesh_json)
{
	if (!navmesh_json.contains("edges"))
		return;

	for (const auto& json_edge : navmesh_json["edges"])
	{
		int from_id = json_edge["from"];
		int to_id = json_edge["to"];
		float weight = json_edge["distance"];

		edges.push_back(std::make_unique<Editor::Edge>(from_id, to_id, weight));
	}
}

void NavmeshEditor::output_success(const QString& message)
{
	output(message, Qt::green);
}

void NavmeshEditor::set_corner_nodes()
{
	corner_node_1 = nullptr;
	corner_node_2 = nullptr;

	for (const auto& node : nodes)
	{
		if (!node->corner)
			continue;

		if (!corner_node_1)
		{
			corner_node_1 = node.get();
		}
		else if (!corner_node_2)
		{
			corner_node_2 = node.get();
			return;
		}
	}
}

void NavmeshEditor::adjust_all_nodes() 
{
	auto cs_go_diff_vec = corner_node_2->pos - corner_node_1->pos;
	auto canvas_diff_vec = corner_node_2->canvas_pos - corner_node_1->canvas_pos;

	int x_go_coord_diff = cs_go_diff_vec.x;
	int y_go_coord_diff = cs_go_diff_vec.y;

	if (x_go_coord_diff == 0 || y_go_coord_diff == 0)
		return;

	int x_canvas_coord_diff = canvas_diff_vec.x;
	int y_canvas_coord_diff = canvas_diff_vec.y;

	float x_ratio = (float)x_canvas_coord_diff / x_go_coord_diff;
	float y_ratio = (float)y_canvas_coord_diff / y_go_coord_diff;


	for (auto&& node : nodes) 
	{
		if (node->id == corner_node_1->id || node->id == corner_node_2->id)
			continue;

		auto go_diff_vec = node->pos - corner_node_1->pos;
		Vec2D<int> canvas_pos = corner_node_1->canvas_pos;
		
		canvas_pos.x += go_diff_vec.x * x_ratio;
		canvas_pos.y += go_diff_vec.y * y_ratio;

		node->canvas_pos = canvas_pos;
		node->render = true;
	}
}
