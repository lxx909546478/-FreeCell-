#pragma once
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QObject>
#include <QPixmap>
#include <QPointF>
#include <QPainter>
#include <QRectF>

const int WIDTH = 3834 / 54;
const int HEIGHT = 96;
const int ZMAX = 100;

class Card : public QObject, public QGraphicsItem
{
	Q_OBJECT
		Q_INTERFACES(QGraphicsItem)

public:
	Card(QGraphicsItem* parent = Q_NULLPTR);

	Card(int value, QGraphicsItem* parent = Q_NULLPTR);

	Card(const Card& rhs);

	~Card();

	Card& operator=(const Card& rhs);

	void changeList(int lnum);

	int getValue();

	friend class Controller;

signals:
	void signalFindList(int source, int x, int y, int v);

	void signalMoveTogether(double x, double y);

	void signalStartBind(double x, double y);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

	QRectF boundingRect() const override;


private:
	int value;
	QRectF rect;
	int listNum;
	bool isPressed;
	bool ismovable;
};

