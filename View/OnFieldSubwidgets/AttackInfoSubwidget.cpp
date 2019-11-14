#include "AttackInfoSubwidget.h"
#include <qwidget.h>
#include <QPainter>
namespace View {

	AttackInfoSubwidget::AttackInfoSubwidget(QWidget* parentwidget, QObject* parent)
		: QObject(parent), InfoRectangle(), mySize(200, 100), start(0, 0), dataToShow(nullptr), hasInfo(false), hidden(true), parentWidget(parentwidget)
		, timeoutTimer(new QTimer(this))
	{
		timeoutTimer->setSingleShot(true);
		timeoutTimer->setInterval(4000);
		QObject::connect(timeoutTimer, &QTimer::timeout, this, &AttackInfoSubwidget::eraseAndHide);
	}

	AttackInfoSubwidget::AttackInfoSubwidget(QSize size, QPoint Start, QWidget* parentwidget, QObject* parent)
		: QObject(parent), InfoRectangle(), mySize(size), start(Start), dataToShow(nullptr), hasInfo(false), hidden(true), parentWidget(parentwidget)
		, timeoutTimer(new QTimer(this))
	{
		timeoutTimer->setSingleShot(true);
		timeoutTimer->setInterval(4000);
		QObject::connect(timeoutTimer, &QTimer::timeout, this, &AttackInfoSubwidget::eraseAndHide);
	}

	void View::AttackInfoSubwidget::resize(QSize sz)
	{
		mySize = sz;
		if (!hidden) {
			parentWidget->update();
		}
	}

	void View::AttackInfoSubwidget::move(QPoint point)
	{
		start = point;
		if (!hidden) {
			parentWidget->update();
		}
	}

	void View::AttackInfoSubwidget::assignStatistics(QPoint where, statisticsPtr statptr)
	{
		start = where;
		if (hasInfo)
		{
			delete dataToShow.release();
			dataToShow = std::move(statptr);
		}
		else
		{
			dataToShow = std::move(statptr);
			hasInfo = true;
		}
	}

	void View::AttackInfoSubwidget::show()
	{
		timeoutTimer->start();
		hidden = false;
		parentWidget->update();
	}

	void View::AttackInfoSubwidget::hide()
	{
		hidden = true;
		timeoutTimer->stop();
	}

	void View::AttackInfoSubwidget::show(QPoint where)
	{
		start = where;
		hidden = false;
		timeoutTimer->start();
		parentWidget->update();
	}

	void View::AttackInfoSubwidget::draw(QPainter* painter)
	{
		if (!hasInfo)
			return;
		painter->save();
		QRect myRect(start, mySize);
		painter->setBrush(QColor(247, 238, 151));
		painter->setPen(QPen(QColor(99, 91, 14), 3));
		painter->drawRect(myRect);
		painter->setPen(QPen(Qt::black, 1));
		QString view;
		view += "Battle statistics:\n" + QString::number(dataToShow->damageDealt.first) + " damage dealt by attacker\n from them "
			+ QString::number(dataToShow->damageBlocked.second)+ " damage was blocked\n" 
			+ QString::number(dataToShow->damageDealt.second) + " damage dealt by defender\nFrom them "
			+ QString::number(dataToShow->damageBlocked.first) + " damage was blocked\n" + ((dataToShow->wasRetaliation) ? "defender retaliated" : "defender did not retaliated");
		painter->drawText(myRect, view);
		painter->restore();
	}

	void View::AttackInfoSubwidget::clear()
	{
		if (hasInfo)
		{
			delete dataToShow.release();
			hasInfo = false;
		}
	}
	bool AttackInfoSubwidget::isHidden()
	{
		return hidden;
	}
	void AttackInfoSubwidget::eraseAndHide()
	{
		hide();
		clear();
		parentWidget->update();
	}
}