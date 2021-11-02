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

void NavmeshEditor::draw_editor_map()
{
	QPixmap map = QPixmap::fromImage(image);
	QPainter painter(&map);

	painter.setBrush(Qt::red);
	painter.drawEllipse({ 0,0 }, 10, 10);
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
	draw_editor_map();
}

void NavmeshEditor::load_navmesh(const QString& file_name)
{
	auto file = file_name.toStdString();
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

	draw_editor_map();
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

	draw_editor_map();
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
