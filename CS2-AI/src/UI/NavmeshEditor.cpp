#include "UI/NavmeshEditor.h"

template <typename T>
static void setElementsVisible(const std::vector<T>& elems, bool visible)
{
	for (const auto& elem : elems)
		elem->render = visible;
}

// Gets the minimum distance from a point p to a line segment defined by v and w
template <typename T>
static std::optional<float> minimum_distance(Vec2D<T> v, Vec2D<T> w, Vec2D<T> p)
{
	const float lineSegmentLengthSquared = distanceSquared(v, w);
	if (lineSegmentLengthSquared == 0.0)
		return distance(p, v); // True if v == w

	const auto lineSegmentDirection = w - v;
	const float t = dotProduct(p - v, lineSegmentDirection) / lineSegmentLengthSquared;
	if (t < 0)
		return {};	// Projected point is not on the line segment
	if (t > 1)
		return {};	// Projected point is not on the line segment

	// We project the vector p - v onto the line segment defined by w - v
	const Vec2D<T> projection = v + t * lineSegmentDirection;

	return distance(p, projection);
}

NavmeshEditor::NavmeshEditor(QWidget* parent, QLineEdit* output_line) : QScrollArea(parent), m_output_line(output_line), m_displayed_map(new QLabel)
{
	m_displayed_map->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_displayed_map->setScaledContents(true);
	setWidget(m_displayed_map);
}

Vec2D<int> NavmeshEditor::get_cursor_pos_on_canvas() const
{
	auto mapped_pos = m_displayed_map->mapFromGlobal(QCursor::pos());
	auto scaled_cursor_pos = mapped_pos / m_zoom_factor;
	return Vec2D<int>(scaled_cursor_pos.x(), scaled_cursor_pos.y());
}

void NavmeshEditor::left_clicked(QMouseEvent* event)
{
	if (m_image.isNull())
		return;

	auto cursor_pos_on_canvas = get_cursor_pos_on_canvas();

	if (m_editor_state == State::PLACE_CORNER_1)
	{
		m_corner_node_1->canvas_pos = cursor_pos_on_canvas;
		m_corner_node_1->render = true;
		m_editor_state = State::PLACE_CORNER_2;
		output("Place second node: " + QString::fromStdString(m_corner_node_2->pos.to_string()));
	}
	else if (m_editor_state == State::PLACE_CORNER_2)
	{
		m_corner_node_2->canvas_pos = cursor_pos_on_canvas;
		m_corner_node_2->render = true;
		m_editor_state = State::ADD_EDGES;
		output_success("Corner nodes set: edges can be added");
		adjust_all_nodes();
		setElementsVisible(m_edges, true);
	}
	else if (m_editor_state == State::ADD_EDGES)
	{
		if (!m_selected_node_1)
		{
			m_selected_node_1 = get_clicked_node(m_nodes, cursor_pos_on_canvas);
			if (m_selected_node_1)
				output("Selected Node: " + std::to_string(m_selected_node_1->id) + ", Ingame coordinates: " + m_selected_node_1->pos.to_string());
		}
		else if (!m_selected_node_2)
		{
			m_selected_node_2 = get_clicked_node(m_nodes, cursor_pos_on_canvas);
		}

		if (m_selected_node_1 && m_selected_node_2)
		{
			if (m_selected_node_1->id == m_selected_node_2->id)
			{
				output_error("Can't create an edge from and to the same node");
			}
			else
			{
				// Right now only create bidirectional edges
				add_edge(m_selected_node_1, m_selected_node_2);
				add_edge(m_selected_node_2, m_selected_node_1);
				output("Edge created between ID: " + std::to_string(m_selected_node_1->id) + " and: " + std::to_string(m_selected_node_2->id));
			}

			m_selected_node_1 = nullptr;
			m_selected_node_2 = nullptr;
		}
	}
	render_editor();
}

void NavmeshEditor::delete_node_and_corresponding_edges(Editor::Node* node)
{
	const int node_id = node->id;
	for (std::size_t i = 0; i < m_edges.size();)
	{
		if ((m_edges[i]->from->id == node_id) || (m_edges[i]->to->id == node_id))
			m_edges.erase(m_edges.begin() + i);
		else
			i++;
	}

	for (std::size_t i = 0; i < m_nodes.size();)
	{
		if (m_nodes[i]->id == node_id)
			m_nodes.erase(m_nodes.begin() + i);
		else
			i++;
	}
}

void NavmeshEditor::delete_edge(Editor::Edge* edge)
{
	const int fromId = edge->from->id;
	const int toId = edge->to->id;
	for(std::size_t i = 0; i < m_edges.size();) 
	{
		// Remove two edges, one for each direction
		if ((fromId == m_edges[i]->from->id) && (toId == m_edges[i]->to->id)
			|| (fromId == m_edges[i]->to->id) && (toId == m_edges[i]->from->id))
		{
			m_edges.erase(m_edges.begin() + i);
			continue;
		}
		i++;
	}
}

void NavmeshEditor::right_clicked(QMouseEvent* event)
{
	auto cursor = get_cursor_pos_on_canvas();
	const auto node = get_clicked_node(m_nodes, cursor);
	if(node)
	{
		auto messageBoxSelection = QMessageBox(QMessageBox::Question, "Remove node", "Do you want to remove the node?", QMessageBox::Yes | QMessageBox::No).exec();
		if (QMessageBox::Yes == messageBoxSelection)
			delete_node_and_corresponding_edges(node);

		render_editor();
		return;
	}

	const auto edge = get_clicked_edge(m_edges, cursor);
	if (!edge)
		return;

	auto messageBoxSelection = QMessageBox(QMessageBox::Question, "Remove edge", "Do you want to remove the edge?", QMessageBox::Yes | QMessageBox::No).exec();
	if (QMessageBox::Yes == messageBoxSelection)
		delete_edge(edge);

	render_editor();
}

NavmeshEditor::~NavmeshEditor()
{
	if (m_displayed_map)
		delete m_displayed_map;
}

void NavmeshEditor::render_editor()
{
	if (m_image.isNull())
		return;

	QPixmap map = QPixmap::fromImage(m_image);
	render_edges(map);
	render_nodes(map);
	m_displayed_map->setPixmap(map);
	m_displayed_map->resize(m_zoom_factor * map.size());
}

void NavmeshEditor::render_edges(QPixmap& map)
{
	if (!m_corner_nodes_placed)
		return;

	QPainter painter(&map);

	painter.setPen(QPen(Qt::black, 4));
	for (const auto& edge : m_edges)
	{
		if (!edge->render)
			continue;

		QPoint from = QPoint(edge->from->canvas_pos.x, edge->from->canvas_pos.y);
		QPoint to = QPoint(edge->to->canvas_pos.x, edge->to->canvas_pos.y);
		painter.drawLine(from, to);
	}
}

void NavmeshEditor::render_nodes(QPixmap& map)
{
	QPainter painter(&map);

	painter.setBrush(Qt::red);
	for (const auto& node : m_nodes)
	{
		if (node->render)
			painter.drawEllipse(QPoint(node->canvas_pos.x, node->canvas_pos.y), (int)(NODE_SIZE / m_zoom_factor), (int)(NODE_SIZE / m_zoom_factor));
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

	m_image = img;
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
		m_map_name = navmesh_json["map_name"];
	}
	catch (std::exception const& e)
	{
		output_error("Could not load Navmesh: " + QString(e.what()));
	}
}

void NavmeshEditor::place_corner_points()
{
	set_corner_nodes();

	if (!m_corner_node_1 || !m_corner_node_2)
	{
		output_error("Couldn't find two corner points, please load a valid navmesh");
		return;
	}

	m_editor_state = State::PLACE_CORNER_1;
	setNodesAndEdgesInvisible();
	render_editor();
	output("Place first node: " + QString::fromStdString(m_corner_node_1->pos.to_string()));
}

bool NavmeshEditor::save_navmesh()
{
	try
	{
		std::ofstream my_file;
		std::string file_name = "Navmesh/json/";
		if (m_map_name == "")
			file_name = file_name + "no_name" + ".json";
		else
			file_name = file_name + m_map_name + ".json";

		my_file.open(file_name);
		json nav_json;
		nav_json["map_name"] = m_map_name;

		for (unsigned int i = 0; i < m_nodes.size(); i++)
		{
			nav_json["nodes"][i]["x"] = m_nodes[i]->pos.x;
			nav_json["nodes"][i]["y"] = m_nodes[i]->pos.y;
			nav_json["nodes"][i]["z"] = m_nodes[i]->pos.z;
			nav_json["nodes"][i]["id"] = m_nodes[i]->id;
			nav_json["nodes"][i]["corner"] = m_nodes[i]->corner;
		}

		int counter = 0;
		for (const auto& edge : m_edges)
		{
			nav_json["edges"][counter]["from"] = edge->from->id;
			nav_json["edges"][counter]["to"] = edge->to->id;
			nav_json["edges"][counter]["weight"] = edge->weight;
			counter++;
		}

		my_file << nav_json;
		my_file.close();
		output_success("File successfully saved");
	}
	catch (std::exception const& e)
	{
		output_error("Can't save navmesh file: " + QString(e.what()));
		return false;
	}
	return true;
}

void NavmeshEditor::add_node(const Vec3D<float>& cs2_pos)
{
	if (!m_valid_json_loaded)
	{
		output_error("No valid json loaded, can't add node");
		return;
	}

	m_nodes.push_back(std::make_unique<Editor::Node>(m_next_node_id, false, cs2_pos));
	output("Node created: " + std::to_string(m_next_node_id) + ", Ingame coordinates: " + cs2_pos.to_string());
	adjust_all_nodes();
	render_editor();
	m_next_node_id++;
}

void NavmeshEditor::wheelEvent(QWheelEvent* event)
{
	QPoint movement = event->angleDelta();

	if (m_image.isNull() || (movement.x() != 0))
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
		right_clicked(event);
}

void NavmeshEditor::zoom(double factor)
{
	constexpr double max_factor = 5;
	constexpr double min_factor = 0.2;

	m_zoom_factor *= factor;
	m_zoom_factor = std::min(m_zoom_factor, max_factor);
	m_zoom_factor = std::max(m_zoom_factor, min_factor);

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

	m_output_line->setPalette(palette);
	m_output_line->setText(message);
}

void NavmeshEditor::load_nodes(const json& navmesh_json)
{
	if (!navmesh_json.contains("nodes"))
		return;

	m_nodes.clear();
	m_valid_json_loaded = false;
	int corner_count = 0;
	int highest_id = 0;
	for (const auto& json_node : navmesh_json["nodes"])
	{
		Vec3D<float> pos;
		int id = json_node["id"];
		pos.x = json_node["x"];
		pos.y = json_node["y"];
		pos.z = json_node["z"];
		bool corner = json_node["corner"];
		highest_id = std::max(highest_id, id);

		if (corner)
			corner_count++;

		m_nodes.push_back(std::make_unique<Editor::Node>(id, corner, pos));
	}
	m_next_node_id = highest_id + 1;

	if (corner_count >= 2)
		m_valid_json_loaded = true;
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

		auto from_node = get_node_pointer_by_id(m_nodes, from_id);
		auto to_node = get_node_pointer_by_id(m_nodes, to_id);
		if (!from_node)
			output_error("Cannot create edge, Node " + QString::number(from_id) + " not found");
		else if (!to_node)
			output_error("Cannot create edge, Node " + QString::number(to_id) + " not found");

		m_edges.push_back(std::make_unique<Editor::Edge>(from_node, to_node, weight));
	}
}

void NavmeshEditor::set_corner_nodes()
{
	m_corner_node_1 = nullptr;
	m_corner_node_2 = nullptr;

	for (const auto& node : m_nodes)
	{
		if (!node->corner)
			continue;

		if (!m_corner_node_1)
		{
			m_corner_node_1 = node.get();
		}
		else if (!m_corner_node_2)
		{
			m_corner_node_2 = node.get();
			return;
		}
	}
}

void NavmeshEditor::adjust_all_nodes()
{
	auto cs2_diff_vec = m_corner_node_2->pos - m_corner_node_1->pos;
	auto canvas_diff_vec = m_corner_node_2->canvas_pos - m_corner_node_1->canvas_pos;

	int x_go_coord_diff = cs2_diff_vec.x;
	int y_go_coord_diff = cs2_diff_vec.y;

	if (x_go_coord_diff == 0 || y_go_coord_diff == 0)
		return;

	int x_canvas_coord_diff = canvas_diff_vec.x;
	int y_canvas_coord_diff = canvas_diff_vec.y;

	float x_ratio = static_cast<float>(x_canvas_coord_diff) / x_go_coord_diff;
	float y_ratio = static_cast<float>(y_canvas_coord_diff) / y_go_coord_diff;


	for (auto& node : m_nodes)
	{
		if (node->id == m_corner_node_1->id || node->id == m_corner_node_2->id)
			continue;

		auto go_diff_vec = node->pos - m_corner_node_1->pos;
		Vec2D<int> canvas_pos = m_corner_node_1->canvas_pos;

		canvas_pos.x += go_diff_vec.x * x_ratio;
		canvas_pos.y += go_diff_vec.y * y_ratio;

		node->canvas_pos = canvas_pos;
		node->render = true;
	}
	m_corner_nodes_placed = true;
}

Editor::Node* NavmeshEditor::get_clicked_node(const std::vector<std::unique_ptr<Editor::Node>>& nodes, const Vec2D<int>& click_pos)
{
	int closest_distance = INT_MAX;
	Editor::Node* result = nullptr;

	for (const auto& node : nodes)
	{
		int distance = node->canvas_pos.distance(click_pos);
		if ((distance < (int)(NODE_SIZE / m_zoom_factor)) && (distance < closest_distance))
		{
			closest_distance = distance;
			result = node.get();
		}
	}

	return result;
}

Editor::Edge* NavmeshEditor::get_clicked_edge(const std::vector<std::unique_ptr<Editor::Edge>>& edges, const Vec2D<int>& click_pos)
{
	constexpr int edgeSelectedThreshold = 12;

	int closest_distance = INT_MAX;
	Editor::Edge* result = nullptr;

	for (const auto& edge : edges)
	{
		auto distanceToEdge = minimum_distance(edge->from->canvas_pos, edge->to->canvas_pos, click_pos);
		if (!distanceToEdge)
			continue;

		closest_distance = std::min(static_cast<int>(distanceToEdge.value()), closest_distance);

		if (distanceToEdge.value() < (edgeSelectedThreshold / m_zoom_factor))
			result = edge.get();
	}
	std::cout << closest_distance << std::endl;
	return result;
}

bool NavmeshEditor::add_edge(Editor::Node* from, Editor::Node* to, bool render)
{
	if (from->id == to->id)
		return false;

	for (const auto& edge : m_edges)
	{
		if ((from->id == edge->from->id) && (to->id == edge->to->id))
			return false;
	}
	const float distance = from->pos.distance(to->pos);
	m_edges.emplace_back(std::make_unique<Editor::Edge>(from, to, distance, render));

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

void NavmeshEditor::setNodesAndEdgesInvisible() const
{
	setElementsVisible(m_nodes, false);
	setElementsVisible(m_edges, false);
}
