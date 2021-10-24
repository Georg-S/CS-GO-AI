#pragma once
#include <qevent.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qscrollarea.h>
#include <Logging.h>

class NavmeshEditor : public QScrollArea
{
	Q_OBJECT

public:
	explicit NavmeshEditor(QWidget* parent = nullptr);
	~NavmeshEditor();

	void left_clicked(QMouseEvent* event);
	void draw_editor_map();
	void load_file(const QString& file_name);
protected:
	void wheelEvent(QEvent* event);
	void mousePressEvent(QMouseEvent* event) override;

private:
	QLabel* displayed_map = nullptr;
	QImage image;
	double zoom_factor = 1.;
};