#ifndef STREETWIDGET_H
#define STREETWIDGET_H

//#define BASE_CLASS QOpenGLWidget
//#define BASE_CLASS QGraphicsView
#define BASE_CLASS QWidget

#ifdef BASE_CLASS
#include <QGraphicsView>
#include <QOpenGLWidget>
#endif
#include <QImage>

class StreetWidget : public BASE_CLASS
{
	Q_OBJECT

	QPixmap car_image;
	std::vector<QColor> car_colors;

	// scale factor
	double scale;

	// movement
	int move_x;
	int move_y;

public:
	StreetWidget(QWidget* parent = nullptr);
	virtual ~StreetWidget() override;

	void reset_position();

	// enable the focus of all children and the widget
	void enable_focus();
	// disable the focus of all children and the widget
	void disable_focus();

	void update_data() { emit data_changed(); }

signals:
	void data_changed();

protected:
	virtual void paintEvent(QPaintEvent*) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // STREETWIDGET_H
