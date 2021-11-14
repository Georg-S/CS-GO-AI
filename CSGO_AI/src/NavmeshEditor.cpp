#include "NavmeshEditor.h"


NavmeshEditor::NavmeshEditor(QWidget* parent, QLineEdit* output_line) : QScrollArea(parent), output_line(output_line)
{
	displayed_map = new QLabel;
	displayed_map->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	displayed_map->setScaledContents(true);
	setWidget(displayed_map);
}

void NavmeshEditor::left_clicked(QMouseEvent* event)
{
	if (image.isNull())
		return;

	auto mapped_pos = displayed_map->mapFromGlobal(QCursor::pos());
	auto scaled_cursor_pos = mapped_pos / zoom_factor;
	auto cursor_pos_on_canvas = Vec2D<int>(scaled_cursor_pos.x(), scaled_cursor_pos.y());

	if (editor_state == State::PLACE_CORNER_1) 
	{
		corner_node_1->canvas_pos = cursor_pos_on_canvas;
		corner_node_1->render = true;
		editor_state = State::PLACE_CORNER_2;
		output("Place second node: " + QString::fromStdString(corner_node_2->pos.to_string()));
	}
	else if (editor_state == State::PLACE_CORNER_2) 
	{
		corner_node_2->canvas_pos = cursor_pos_on_canvas;
		corner_node_2->render = true;
		editor_state = State::ADD_EDGES;
		output_success("Corner nodes set: edges can be added");
		adjust_all_nodes();
	}
	else if (editor_state == State::ADD_EDGES) 
	{
		if (!selected_node_1) 
		{
			selected_node_1 = get_clicked_node(nodes, cursor_pos_on_canvas);
			if (selected_node_1)
				output("Selected Node: " + std::to_string(selected_node_1->id));
		}
		else if(!selected_node_2) 
		{
			selected_node_2 = get_clicked_node(nodes, cursor_pos_on_canvas);
		}

		if (selected_node_1 && selected_node_2) 
		{
			// Right now only create bidirectional edges
			add_edge(selected_node_1, selected_node_2);
			add_edge(selected_node_2, selected_node_1);
			output("Edge created between ID: " + std::to_string(selected_node_1->id) + " and: " + std::to_string(selected_node_2->id));
			selected_node_1 = nullptr;
			selected_node_2 = nullptr;
		}
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
	if (!corner_nodes_placed)
		return;

	QPainter painter(&map);

	painter.setPen(QPen(Qt::black, 4));
	for (const auto& edge : edges)
	{
		QPoint from = QPoint(edge->from->canvas_pos.x, edge->from->canvas_pos.y);
		QPoint to = QPoint(edge->to->canvas_pos.x, edge->to->canvas_pos.y);
		painter.drawLine(from, to);
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
		map_name = navmesh_json["map_name"];
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

bool NavmeshEditor::save_navmesh()
{
	try
	{
		std::ofstream my_file;
		std::string file_name = "Navmesh/json/";
		if (map_name == "")
			file_name = file_name + "no_name" + ".json";
		else
			file_name = file_name + map_name + ".json";

		my_file.open(file_name);
		json nav_json;
		nav_json["map_name"] = map_name;

		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			nav_json["nodes"][i]["x"] = nodes[i]->pos.x;
			nav_json["nodes"][i]["y"] = nodes[i]->pos.y;
			nav_json["nodes"][i]["z"] = nodes[i]->pos.z;
			nav_json["nodes"][i]["id"] = nodes[i]->id;
			nav_json["nodes"][i]["corner"] = nodes[i]->corner;
		}

		int counter = 0;
		for (const auto& edge : edges) 
		{
			nav_json["edges"][counter]["from"] = edge->from->id;
			nav_json["edges"][counter]["to"] = edge->to->id;
			nav_json["edges"][counter]["weight"] = edge->weight;
			counter++;
		}

		my_file << nav_json;
		my_file.close();
	}
	catch (std::exception const& e)
	{
		return false;
	}
	return true;
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
		zoom(0.9);

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

void NavmeshEditor::output_error(const QString& message)
{
	output(message, Qt::red);
}

void NavmeshEditor::output_success(const QString& message)
{
	output(message, Qt::green);
}

void NavmeshEditor::output(const std::string& message)
{
	output(QString::fromStdString(message));
}

void NavmeshEditor::output(const QString& message)
{
	output(message, Qt::black);
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
		float weight = json_edge["weight"];

		auto from_node = get_node_pointer_by_id(nodes, from_id);
		auto to_node = get_node_pointer_by_id(nodes, to_id);
		if (!from_node) 
			output_error("Cannot create edge, Node " + QString(from_id) + " not found");
		else if(!to_node)
			output_error("Cannot create edge, Node " + QString(to_id) + " not found");

		edges.push_back(std::make_unique<Editor::Edge>(from_node, to_node, weight));
	}
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


	for (auto& node : nodes)
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
	corner_nodes_placed = true;
}

Editor::Node* NavmeshEditor::get_clicked_node(const std::vector<std::unique_ptr<Editor::Node>>& nodes, const Vec2D<int>& click_pos)
{
	int closest_distance = INT_MAX;
	Editor::Node* result = nullptr;

	for (const auto& node : nodes) 
	{
		int distance = node->canvas_pos.distance(click_pos);
		if ((distance < NODE_SIZE) && (distance < closest_distance))
		{
			closest_distance = distance;
			result = node.get();
		}
	}

	return result;
}

bool NavmeshEditor::add_edge(Editor::Node* from, Editor::Node* to) 
{
	if (from->id == to->id)
		return false;

	for (const auto& edge : edges) 
	{
		if ((from->id == edge->from->id) && (to->id == edge->to->id))
			return false;
	}
	const float distance = from->pos.distance(to->pos);
	edges.push_back(std::make_unique<Editor::Edge>(from, to, distance));

	return true;
}

Editor::Node* NavmeshEditor::get_node_pointer_by_id(const std::vector<std::unique_ptr<Editor::Node>>& nodes, int id) const
{
	for (const auto& node : nodes) 
	{
		if (node->id == id)
			return node.get();
	}

	return nullptr;
}
