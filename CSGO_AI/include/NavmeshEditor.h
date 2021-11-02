#pragma once
#include <qevent.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qscrollarea.h>
#include <Logging.h>
#include <qlineedit.h>
#include "json.hpp"

using json = nlohmann::json;

class NavmeshEditor : public QScrollArea
{
	Q_OBJECT

public:
	explicit NavmeshEditor(QWidget* parent, QLineEdit* output);
	~NavmeshEditor();

	void left_clicked(QMouseEvent* event);
	void draw_editor_map();
	void load_image(const QString& file_name);
	void load_navmesh(const QString& file_name);
protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	void zoom(double factor);
	void output(const QString& message);
	void output_error(const QString& message);
	void output(const QString& message, Qt::GlobalColor color);

	QLabel* displayed_map = nullptr;
	QImage image;
	double zoom_factor = 1;
	QLineEdit* output_line = nullptr;
};