#include "NavmeshEditor.h"


NavmeshEditor::NavmeshEditor(QWidget* parent) : QScrollArea(parent)
{
	displayed_map = new QLabel;
	displayed_map->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setWidget(displayed_map);
}

void NavmeshEditor::left_clicked(QMouseEvent* event)
{
	if (image.isNull())
		return;

	QPoint p = displayed_map->mapFromGlobal(QCursor::pos());
	QPixmap map = QPixmap::fromImage(image);

	QPainter painter(&map);
	painter.setBrush(Qt::red);
	painter.drawEllipse(p, 20, 20);

	displayed_map->setPixmap(map);
	displayed_map->adjustSize();
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
	displayed_map->adjustSize();
}

void NavmeshEditor::load_file(const QString& file_name)
{
	QImage img = QImage(file_name);
	if (img.isNull()) 
	{
		Logging::log_error("Could not load file");
		return;
	}

	image = img;
	draw_editor_map();
}

void NavmeshEditor::wheelEvent(QWheelEvent* event)
{
}

void NavmeshEditor::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		left_clicked(event);
	else if (event->button() == Qt::RightButton)
		;
}

