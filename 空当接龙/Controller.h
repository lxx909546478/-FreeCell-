#pragma once
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "Card.h"
#include <QVector>
#include <qglobal.h>
#include <QTime>
#include <QTimer>

const int X_INTERVAL = 100;
const int Y_INTERVAL = 30;

class Operation
{
public:
	int num;
	int sour;
	int dest;
};

class Controller : public QObject
{
	Q_OBJECT

public:
	Controller(QObject* parent);

	~Controller();

	void addToList();

	void addToScene(QGraphicsScene* scene);

	void initialCardHeap();

	void initialCardRec();

	void initialBuffer();

	void initialDest();

	void shuffleCardHeap();

signals:
	void signalPassed();

	void signalTimeout();

public slots:
	void slotFindList(int source, int x, int y, int value);

	void slotClickRetractionButton();

	void slotBindMoveTogether(double x, double y);

private:
	void isPassed();

	bool isBetween(int lhs, int rhs);

	int moveToList(int num, int source, Card* leaveCard, int x, int y, int value);

	int moveToTemp(int source, Card* leaveCard, int x, int y, int value);

	int moveToDest(int source, Card* leaveCard, int x, int y, int value);

	void retractMulCard(const Operation& last);

	void retractFromList(const Operation& last);

	void retractFromTemp(const Operation& last);

	void retraceFromDest(const Operation& last);

	void findList_SingleCard(int num, int source, int x, int y, int value);

	void findList_SeveralCard(int num, int source, int x, int y, int value);


	Card* buffer[4];
	QVector<Card*> cardHeap;
	QVector<QVector<Card*>> cardList;
	QVector<QVector<Card*>> destList;
	Card* dest[4];
	bool isFull[4];
	int bufferEmpty;
	QVector<Operation> steps;
	QVector<Card*> leaveCardVec;
	QTimer* timer;
};

