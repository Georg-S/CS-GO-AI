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
	/*
	if (image.isNull())
		return;

	QPoint p = displayed_map->mapFromGlobal(QCursor::pos());
	QPixmap map = QPixmap::fromImage(image);

	QPainter painter(&map);
	painter.setBrush(Qt::red);
	painter.drawEllipse(p, 20, 20);

	displayed_map->setPixmap(map);
	displayed_map->adjustSize();
	*/
}

NavmeshEditor::~NavmeshEditor()
{
	if (displayed_map)
		delete displayed_map;
}

void NavmeshEditor::draw_editor_map(const QPoint& mouse_pos)
{
	QPixmap map = QPixmap::fromImage(image);
	QPainter painter(&map);

	painter.setBrush(Qt::red);
	painter.drawEllipse(mouse_pos, 10, 10);
	displayed_map->setPixmap(map);
	displayed_map->resize(zoom_factor * map.size());
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
	draw_editor_map({ 0,0 });
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
	editor_state = State::PLACE_CORNERS;
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

	draw_editor_map({ 0,0 });
}

void NavmeshEditor::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		left_clicked(event);
	else if (event->button() == Qt::RightButton)
		;
}

void NavmeshEditor::mouseMoveEvent(QMouseEvent* event)
{
	auto cursor_pos = QCursor::pos();
	auto mapped_pos = displayed_map->mapFromGlobal(cursor_pos);
	auto scaled_pos = mapped_pos / zoom_factor;

	draw_editor_map(scaled_pos);
}

void NavmeshEditor::zoom(double factor)
{
	constexpr double max_factor = 5;
	constexpr double min_factor = 0.2;

	zoom_factor *= factor;
	zoom_factor = std::min(zoom_factor, max_factor);
	zoom_factor = std::max(zoom_factor, min_factor);

	draw_editor_map({0,0});
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

		nodes.push_back(Editor::Node{ id, corner, pos });
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

		edges.push_back(Editor::Edge{ from_id, to_id, weight});
	}
}
