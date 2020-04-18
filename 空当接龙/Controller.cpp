#include "Controller.h"

Controller::Controller(QObject* parent)
{
	for (int i = 0; i < 8; i++)
	{
		QVector<Card*> vec;
		cardList.push_back(vec);
	}
	for (int i = 0; i < 4; i++)
	{
		QVector<Card*> vec;
		destList.push_back(vec);
		isFull[i] = false;
	}
	bufferEmpty = 4;
	timer = new QTimer();
	timer->start(1000);
	connect(timer, &QTimer::timeout, [=]() {
		emit signalTimeout();
		});
}

Controller::~Controller()
{
	disconnect(timer, &QTimer::timeout, 0, 0);
}

bool Controller::isBetween(int lhs, int rhs)
{
	return (lhs / 4 == rhs / 4 + 1) && ((lhs + rhs) % 2 == 1);
}

//点击一张牌后判断是否要绑定移动信号signalMoveTogether
void Controller::slotBindMoveTogether(double x, double y)
{
	//先将所有牌的移动信号signalMoveTogether解绑
	int iPos = (int)x / X_INTERVAL, jPos = (int)y / Y_INTERVAL;
	for (int i = 0; i < cardList.size(); i++)
	{
		for (int j = 0; j <= cardList[i].size() - 1; j++)
		{
			cardList[i][j]->ismovable = false;
			cardList[i][j]->setFlag(QGraphicsItem::ItemIsMovable, false);
			disconnect(cardList[i][j], &Card::signalMoveTogether, 0, 0);
		}
		if (!cardList[i].empty())
		{
			cardList[i].back()->ismovable = true;
			cardList[i].back()->setFlag(QGraphicsItem::ItemIsMovable, true);
		}
	}
	for (int i = 0; i < 4; i++)
		disconnect(buffer[i], &Card::signalMoveTogether, 0, 0);
	for (int i = 0; i < 4; i++)
		if (!destList[i].empty())
			disconnect(destList[i].back(), &Card::signalMoveTogether, 0, 0);

	if (jPos < 0)//如果牌不在8个队列中，说明点击的是单张牌，返回
		return;

	//判断挪动的张牌是否小于空余缓冲区数+1，若不是则不能移动多张牌，返回
	if (cardList[iPos].size() - jPos > bufferEmpty + 1)
		return;

	//绑定移动信号signalMoveTogether
	int j;
	for (j = cardList[iPos].size() - 2; j >= jPos; j--)//从队列最下方往上遍历
	{
		//如果相邻两张牌花色不同，数字相接，则绑定
		if (isBetween(cardList[iPos][j]->getValue(), cardList[iPos][j + 1]->getValue()))
		{
			leaveCardVec.push_back(cardList[iPos][j + 1]);//把牌放入移动队列
			cardList[iPos][j]->ismovable = true;
			cardList[iPos][j]->setFlag(QGraphicsItem::ItemIsMovable, true);
			connect(cardList[iPos][jPos], &Card::signalMoveTogether, [=](double posX, double posY)
				{
					cardList[iPos][j + 1]->setX(posX);
					cardList[iPos][j + 1]->setY(posY + (j + 1 - jPos) * Y_INTERVAL);
					cardList[iPos][j + 1]->setZValue(ZMAX + j + 1 - jPos);
				});
		}
		else//如果还没到点击的牌就不符合条件，则返回
		{
			leaveCardVec.clear();
			return;
		}
	}
	leaveCardVec.push_back(cardList[iPos][j + 1]);//把点击的牌放入移动队列
}

int Controller::moveToList(int num, int source, Card * leaveCard, int x, int y, int value)//把牌移动到队列
{
	int i;
	for (i = 0; i < cardList.size(); i++)
	{
		int x0 = i * X_INTERVAL;
		int y0 = (cardList[i].size() - 1) * Y_INTERVAL;
		if (x0 - WIDTH < x && x < x0 + WIDTH && y0 - HEIGHT < y && y < y0 + HEIGHT)
		{
			if (cardList[i].isEmpty())//如果目标队列为空，直接到位
			{
				//设置牌的位置和属性
				leaveCard->listNum = i;
				leaveCard->setX(x0);
				leaveCard->setY(y0 + Y_INTERVAL);
				leaveCard->setZValue(cardList[i].size());
				cardList[i].push_back(leaveCard);

				//记录该操作
				Operation op_new = { num, source, i };
				steps.push_back(op_new);

				break;
			}
			else if (value % 2 != cardList[i].back()->value % 2 && value / 4 == cardList[i].back()->value / 4 - 1)
			{
				//把目的队列最后一张牌设为不能移动
				cardList[i].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
				cardList[i].back()->ismovable = false;

				//设置牌的位置和属性
				leaveCard->listNum = i;
				leaveCard->setX(x0);
				leaveCard->setY(y0 + Y_INTERVAL);
				leaveCard->setZValue(cardList[i].size());
				cardList[i].push_back(leaveCard);

				//记录该操作
				Operation op_new = { num, source, i };
				steps.push_back(op_new);

				break;
			}
		}
	}
	return i;
}

int Controller::moveToTemp(int source, Card * leaveCard, int x, int y, int value)//把牌移动到缓冲区
{
	int j;
	for (j = 0; j < 4; j++)
	{
		int x0 = j * X_INTERVAL;
		int y0 = -200;
		if (isFull[j] == false && x0 - WIDTH < x && x < x0 + WIDTH && y0 - HEIGHT < y && y < y0 + HEIGHT)
		{
			//设置牌的位置和属性
			leaveCard->listNum = 8 + j;
			leaveCard->setX(x0);
			leaveCard->setY(y0);
			leaveCard->setZValue(0);

			//设置缓冲区
			buffer[j] = leaveCard;
			isFull[j] = true;
			bufferEmpty--;

			//记录该操作
			Operation op_new = { 1, source, 8 + j };
			steps.push_back(op_new);

			break;
		}
	}
	return j;
}

int Controller::moveToDest(int source, Card * leaveCard, int x, int y, int value)//把牌移动到最终区域
{
	int k;
	for (k = 0; k < 4; k++)
	{
		int x0 = (k + 5) * X_INTERVAL, y0 = -200;
		if (x0 - WIDTH < x && x < x0 + WIDTH && y0 - HEIGHT < y && y < y0 + HEIGHT)
		{
			if ((destList[k].isEmpty() && value == k)
				|| (!destList[k].isEmpty() && (value - k) % 4 == 0 && destList[k].back()->value + 4 == value)
				)
			{
				//设置牌的位置和属性
				leaveCard->listNum = k + 12;
				leaveCard->setX(x0);
				leaveCard->setY(y0);
				leaveCard->setZValue(destList[k].size());
				leaveCard->ismovable = false;
				leaveCard->setFlag(QGraphicsItem::ItemIsMovable, false);
				destList[k].push_back(leaveCard);

				//记录该操作
				Operation op_new = { 1, source, k + 12 };
				steps.push_back(op_new);

				break;
			}
			else
			{
				k = 4;
				break;
			}
		}
	}
	return k;
}

void Controller::findList_SeveralCard(int num, int source, int x, int y, int value)//移动多张牌
{
	int i;
	for (int i = 0; i < num; i++)//从源队列删除牌，并设置属性
		cardList[source].pop_back();
	if (!cardList[source].empty())
	{
		cardList[source].back()->ismovable = true;
		cardList[source].back()->setFlag(QGraphicsItem::ItemIsMovable);
	}

	//尝试把第一张牌挪到目的队列，成功则返回队列号
	int loc = moveToList(num, source, leaveCardVec.back(), x, y, value);

	//如果成功，把剩下的牌挪到目的队列
	if (loc != cardList.size())
	{
		for (i = leaveCardVec.size() - 2; i >= 0; i--)
		{
			int x0 = loc * X_INTERVAL;
			int y0 = (cardList[loc].size() - 1) * Y_INTERVAL;
			leaveCardVec[i]->setX(x0);
			leaveCardVec[i]->setY(y0 + Y_INTERVAL);
			leaveCardVec[i]->setZValue(cardList[loc].size());
			leaveCardVec[i]->listNum = loc;
			if (!cardList[loc].empty())
			{
				cardList[loc].back()->ismovable = false;
				cardList[loc].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
			}
			cardList[loc].push_back(leaveCardVec[i]);
		}
	}
	else//否则回归原位
	{
		for (i = leaveCardVec.size() - 1; i >= 0; i--)
		{
			int x0 = source * X_INTERVAL;
			int y0 = (cardList[source].size() - 1) * Y_INTERVAL;
			leaveCardVec[i]->setX(x0);
			leaveCardVec[i]->setY(y0 + Y_INTERVAL);
			leaveCardVec[i]->setZValue(cardList[source].size());
			if (!cardList[source].empty())
			{
				cardList[source].back()->ismovable = false;
				cardList[source].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
			}
			cardList[source].push_back(leaveCardVec[i]);
		}
	}
}

void Controller::findList_SingleCard(int num, int source, int x, int y, int value)//移动单张牌
{
	qDebug() << x << y;
	Card* leaveCard;

	//0到7代表从队列出发，8,9,10,11从缓冲区出发
	if (0 <= source && 8 > source) //从某个队列出发
	{
		//从源队列删除牌，并把最后一张牌设为可以移动
		leaveCard = cardList[source].back();
		cardList[source].pop_back();
		if (!cardList[source].empty())
		{
			cardList[source].back()->setFlag(QGraphicsItem::ItemIsMovable);
			cardList[source].back()->ismovable = true;
		}

		//尝试牌挪到目的队列，成功则返回队列号
		int i = moveToList(num, source, leaveCard, x, y, value);

		if (i == cardList.size())//不去队列
		{
			//尝试牌挪到缓冲区，成功则返回缓冲区号
			int j = moveToTemp(source, leaveCard, x, y, value);

			if (j != 4)//判断是否去缓冲区
				return;

			//尝试牌挪到最终区域，成功则返回最终区域号
			int k = moveToDest(source, leaveCard, x, y, value);

			if (k != 4)//判断是否能放到最终区域
				return;

			//都不行，放回原位
			int x0 = source * X_INTERVAL;
			int y0 = (cardList[source].size() - 1) * Y_INTERVAL;
			leaveCard->setX(x0);
			leaveCard->setY(y0 + 30);
			leaveCard->setZValue(cardList[source].size());
			if (!cardList[source].isEmpty())
			{
				cardList[source].back()->ismovable = false;
				cardList[source].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
			}
			cardList[source].push_back(leaveCard);
		}
	}
	//从缓冲区出发
	else if (8 <= source && 12 > source)
	{
		leaveCard = buffer[source - 8];
		int i = moveToList(num, source, leaveCard, x, y, value);
		isFull[source - 8] = false;
		bufferEmpty++;

		if (i == 8)//不去队列
		{
			int k = moveToDest(source, leaveCard, x, y, value);
			if (k == 4)//返回原位
			{
				leaveCard->setX((source - 8) * X_INTERVAL);
				leaveCard->setY(-200);
				leaveCard->setZValue(0);
				isFull[source - 8] = true;
				bufferEmpty--;
			}
		}
	}
}

void Controller::isPassed()//判断是否通关
{
	for (int i = 0; i < 4; i++)
		if (destList[i].size() < 13)
			return;
	emit signalPassed();
}

void Controller::slotFindList(int source, int x, int y, int value)//移动牌
{
	int num = leaveCardVec.size();
	//qDebug() << "leavecard:" << num << endl;
	if (num > 1)//如果是多张
		findList_SeveralCard(num, source, x, y, value);
	else//单张
		findList_SingleCard(num, source, x, y, value);

	//清空移动队列
	leaveCardVec.clear();

	//判断是否通关
	isPassed();
}

void Controller::retractMulCard(const Operation & last)//移动多张牌的悔牌
{
	for (int i = 0; i < last.num; i++)
	{
		leaveCardVec.push_back(cardList[last.dest].back());
		leaveCardVec.back()->listNum = last.sour;
		cardList[last.dest].pop_back();
	}

	if (!cardList[last.dest].empty())
	{
		cardList[last.dest].back()->setFlag(QGraphicsItem::ItemIsMovable, true);
		cardList[last.dest].back()->ismovable = true;
	}
	if (!cardList[last.sour].empty())
	{
		cardList[last.sour].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
		cardList[last.sour].back()->ismovable = false;
	}

	int x0 = last.sour * X_INTERVAL;
	for (int i = 0; i < last.num; i++)
	{
		int y0 = (cardList[last.sour].size() - 1) * Y_INTERVAL;
		leaveCardVec[last.num - i - 1]->setX(x0);
		leaveCardVec[last.num - i - 1]->setY(y0 + Y_INTERVAL);
		leaveCardVec[last.num - i - 1]->setZValue(cardList[last.sour].size());
		cardList[last.sour].push_back(leaveCardVec[last.num - i - 1]);
	}
	leaveCardVec.clear();
}

void Controller::retractFromList(const Operation & last)//移动单张牌的悔牌，目的地是队列
{
	//取出牌
	Card* leaveCard = cardList[last.dest].back();
	cardList[last.dest].pop_back();

	//设置目的队列最后一张牌可移动
	if (!cardList[last.dest].empty())
	{
		cardList[last.dest].back()->setFlag(QGraphicsItem::ItemIsMovable, true);
		cardList[last.dest].back()->ismovable = true;
	}

	//设置队列号
	leaveCard->listNum = last.sour;

	if (0 <= last.sour && 8 > last.sour)//队列到队列
	{
		//设置源队列最后一张牌不可移动
		if (!cardList[last.sour].empty())
		{
			cardList[last.sour].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
			cardList[last.sour].back()->ismovable = false;
		}

		//设置牌的属性和位置
		int x0 = last.sour * X_INTERVAL;
		int y0 = (cardList[last.sour].size() - 1) * Y_INTERVAL;
		leaveCard->setX(x0);
		leaveCard->setY(y0 + Y_INTERVAL);
		leaveCard->setZValue(cardList[last.sour].size());
		leaveCard->ismovable = true;
		leaveCard->setFlag(QGraphicsItem::ItemIsMovable, true);
		cardList[last.sour].push_back(leaveCard);
	}
	else if (8 <= last.sour && 12 > last.sour)//缓冲区到队列
	{
		//设置牌的属性和位置
		int x0 = (last.sour - 8) * X_INTERVAL;
		int y0 = -200;
		leaveCard->setX(x0);
		leaveCard->setY(y0);
		leaveCard->setZValue(0);

		//设置缓冲区
		buffer[last.sour - 8] = leaveCard;
		isFull[last.sour - 8] = true;
		bufferEmpty--;
	}
}

void Controller::retraceFromDest(const Operation & last)//移动单张牌的悔牌，目的地是最终区域
{
	//取出牌
	Card* leaveCard = destList[last.dest - 12].back();
	destList[last.dest - 12].pop_back();

	//设置牌的队列号
	leaveCard->listNum = last.sour;

	if (0 <= last.sour && 8 > last.sour)//队列到最终区域
	{
		//设置源队列最后一张牌不可移动
		if (!cardList[last.sour].empty())
		{
			cardList[last.sour].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
			cardList[last.sour].back()->ismovable = false;
		}

		//设置牌的属性和位置
		int x0 = last.sour * X_INTERVAL;
		int y0 = (cardList[last.sour].size() - 1) * Y_INTERVAL;
		leaveCard->setX(x0);
		leaveCard->setY(y0 + Y_INTERVAL);
		leaveCard->setZValue(cardList[last.sour].size());
		leaveCard->ismovable = true;
		leaveCard->setFlag(QGraphicsItem::ItemIsMovable, true);
		cardList[last.sour].push_back(leaveCard);
	}
	else if (8 <= last.sour && 12 > last.sour)//缓冲区到最终区域
	{
		//设置牌的属性和位置
		int x0 = (last.sour - 8) * X_INTERVAL;
		int y0 = -200;
		leaveCard->setX(x0);
		leaveCard->setY(y0);
		leaveCard->setZValue(0);
		leaveCard->ismovable = true;
		leaveCard->setFlag(QGraphicsItem::ItemIsMovable, true);

		//设置缓冲区
		buffer[last.sour - 8] = leaveCard;
		isFull[last.sour - 8] = true;
		bufferEmpty--;
	}
}

void Controller::retractFromTemp(const Operation & last)//移动单张牌的悔牌，目的地是缓冲区
{
	//取出牌
	Card* leaveCard = buffer[last.dest - 8];
	isFull[last.dest - 8] = false;
	bufferEmpty++;

	//设置牌的队列号
	leaveCard->listNum = last.sour;

	//设置源队列最后一张牌不可移动
	if (!cardList[last.sour].empty())
	{
		cardList[last.sour].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
		cardList[last.sour].back()->ismovable = false;
	}

	//设置牌的属性和位置
	int x0 = last.sour * X_INTERVAL;
	int y0 = (cardList[last.sour].size() - 1) * Y_INTERVAL;
	leaveCard->setX(x0);
	leaveCard->setY(y0 + Y_INTERVAL);
	leaveCard->setZValue(cardList[last.sour].size());
	cardList[last.sour].push_back(leaveCard);
}

void Controller::slotClickRetractionButton()
{
	if (steps.empty())//如果记录队列为空，返回
		return;

	//取出最后一条记录
	Operation last = steps.back();
	steps.pop_back();

	if (last.num > 1)//如果该记录移动多张牌
	{
		retractMulCard(last);
		return;
	}

	//如果该记录移动单张牌
	if (0 <= last.dest && 8 > last.dest)//目标是队列
	{
		retractFromList(last);
	}
	else if (8 <= last.dest && 12 > last.dest)//目标是缓冲区，只能从队列出发
	{
		retractFromTemp(last);
	}
	else if (12 <= last.dest && 16 > last.dest)//目标是最终区域
	{
		retraceFromDest(last);
	}
}

void Controller::initialCardHeap()//初始化牌堆
{
	for (int i = 0; i < 52; i++)
	{
		Card* tmp = new Card(i);
		cardHeap.push_back(tmp);
	}
}

void Controller::shuffleCardHeap()//Knuth-shuffle 算法
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	for (int i = 0; i < 52; i++)
	{
		int reExcNum = qrand() % (52 - i) + i;//生成一个随机交换位置
		if (i != reExcNum)
		{
			Card* tmp = cardHeap[i];
			cardHeap[i] = cardHeap[reExcNum];
			cardHeap[reExcNum] = tmp;
		}
	}
}

void Controller::addToList()//把牌堆中的牌放入队列
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			cardList[i].push_back(cardHeap[i * 7 + j]);
		}
		for (int j = 0; j < 6; j++)
		{
			cardList[4 + i].push_back(cardHeap[28 + i * 6 + j]);
		}
	}
	for (int m = 0; m < 52; m++)
	{
		connect(cardHeap[m], &Card::signalStartBind, this, &Controller::slotBindMoveTogether);
	}
}

void Controller::addToScene(QGraphicsScene * scene)//把牌放入scene中
{
	for (int i = 0; i < 52; i++)
	{
		scene->addItem(cardHeap[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		scene->addItem(buffer[i]);
		scene->addItem(dest[i]);
	}
}

void Controller::initialCardRec()//设置牌的位置等属性
{
	for (int i = 0; i < cardList.size(); i++)
	{
		for (int j = 0; j < cardList[i].size(); j++)
		{
			connect(cardList[i][j], &Card::signalFindList, this, &Controller::slotFindList);
			cardList[i][j]->setX(i * X_INTERVAL);
			cardList[i][j]->setY(j * Y_INTERVAL);
			cardList[i][j]->setZValue(j);
			cardList[i][j]->changeList(i);
			if (j != cardList[i].size() - 1)
			{
				cardList[i][j]->ismovable = false;
				cardList[i][j]->setFlag(QGraphicsItem::ItemIsMovable, false);
			}
			else
			{
				cardList[i][j]->ismovable = true;
				cardList[i][j]->setFlag(QGraphicsItem::ItemIsMovable, true);
			}
		}
	}
	qDebug() << cardList.size() << ":" << cardList[0].size() << endl;
}

void Controller::initialBuffer()//初始化缓冲区
{
	for (int i = 0; i < 4; i++)
	{
		Card* tmp = new Card(52);
		tmp->setX(i * X_INTERVAL);
		tmp->setY(-200);
		tmp->setZValue(-1);
		tmp->setFlag(QGraphicsItem::ItemIsMovable, false);
		buffer[i] = tmp;
	}

}

void Controller::initialDest()//初始化最终区域
{
	for (int i = 0; i < 4; i++)
	{
		Card* tmp = new Card(53 + i);
		tmp->setX((i + 5) * X_INTERVAL);
		tmp->setY(-200);
		tmp->setZValue(-1);
		tmp->setFlag(QGraphicsItem::ItemIsMovable, false);
		dest[i] = tmp;
	}
}
