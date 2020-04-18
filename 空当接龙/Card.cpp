#include "Card.h"


Card::Card(QGraphicsItem* parent /* = Q_NULLPTR */)
{

}

Card::Card(int value, QGraphicsItem* parent)
{
	this->value = value;
	rect = QRectF(0, 0, 3834 / 54, 96);
	isPressed = false;
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

Card::Card(const Card & rhs)
{
	this->value = rhs.value;
	this->rect = rhs.rect;
	this->isPressed = rhs.isPressed;
}

Card& Card::operator=(const Card & rhs)
{
	this->value = rhs.value;
	this->rect = rhs.rect;
	this->isPressed = rhs.isPressed;
	return *this;
}


Card::~Card()
{

}

int Card::getValue()
{
	return value;
}

QRectF Card::boundingRect() const
{
	return rect;
}

void Card::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if (value == 52) {
		QPixmap original(":/pic/Resources/52.png");
		painter->drawPixmap(0, 0, WIDTH, HEIGHT, original);
	}
	else if (value == 53) {
		QPixmap original(":/pic/Resources/534.png");
		painter->drawPixmap(0, 0, WIDTH, HEIGHT, original);
	}
	else if (value == 54) {
		QPixmap original(":/pic/Resources/531.png");
		painter->drawPixmap(0, 0, WIDTH, HEIGHT, original);
	}
	else if (value == 55) {
		QPixmap original(":/pic/Resources/532.png");
		painter->drawPixmap(0, 0, WIDTH, HEIGHT, original);
	}
	else if (value == 56) {
		QPixmap original(":/pic/Resources/533.png");
		painter->drawPixmap(0, 0, WIDTH, HEIGHT, original);
	}
	else {
		int pos = 0;
		if (0 <= value && 8 > value)
		{
			pos = 44 + value;
		}
		else if (8 <= value && 52 > value)
		{
			pos = value - 8;
		}
		else
		{
			pos = value;
		}
		QRect rect(WIDTH * pos, 0, WIDTH, HEIGHT);
		QPixmap original(":/pic/cards");
		QPixmap cropped = original.copy(rect);
		painter->drawPixmap(0, 0, WIDTH, HEIGHT, cropped);
	}
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	qDebug() << endl;

	if (/*ismovable&&*/event->button() == Qt::LeftButton)
	{
		isPressed = true;
		emit signalStartBind(pos().x(), pos().y());
	}
	QGraphicsItem::mousePressEvent(event);
	qDebug() << this->pos().x() << ":" << this->pos().y() << endl;
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (isPressed/*&&ismovable*/)
	{
		if (ismovable)
			setZValue(ZMAX);
		this->scene()->update();
		emit signalMoveTogether(pos().x(), pos().y());
	}
	QGraphicsItem::mouseMoveEvent(event);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (ismovable)
	{
		emit signalFindList(listNum, x(), y(), value);
		qDebug() << listNum;
	}
	if (event->button() == Qt::LeftButton)
	{
		isPressed = false;
	}
	QGraphicsItem::mouseReleaseEvent(event);
}

void Card::changeList(int lnum)
{
	listNum = lnum;
}

