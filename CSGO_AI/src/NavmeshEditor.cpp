#include "NavmeshEditor.h"


NavmeshEditor::NavmeshEditor(QWidget* parent) : QScrollArea(parent)
{
	displayed_map = new QLabel;
	displayed_map->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	this->setWidget(displayed_map);
	this->setVisible(false);
}
/*
void NavmeshEditor::left_clicked(QMouseEvent* event)
{
	QPoint p = displayed_map->mapFromGlobal(QCursor::pos());

	if (event->button() == Qt::LeftButton)
	{
		QPixmap map = QPixmap::fromImage(image);

		QPainter painter(&map);
		painter.setBrush(Qt::red);
		painter.drawEllipse(p, 20, 20);

		displayed_map->setPixmap(map);
		this->setVisible(true);
	}
}
*/

void NavmeshEditor::draw_editor_map()
{
	/*
	QPainter painter(&map);
	painter.setBrush(Qt::red);
	painter.drawEllipse({ 0,0 }, 10, 10);
	*/

	QPixmap map = QPixmap::fromImage(image);
	displayed_map->setPixmap(map);
	this->setVisible(true);
}

void NavmeshEditor::load_file(const QString& file_name)
{
	QImage img = QImage(file_name);
	if (img.isNull())
		Logging::log_error("Could not load file");

	image = img;

	draw_editor_map();
}

void NavmeshEditor::wheelEvent(QEvent* event)
{
	int a = 2;
	a += 2;
}
/*
void NavmeshEditor::mousePressEvent(QMouseEvent* event)
{
	left_clicked(event);
}
*/
