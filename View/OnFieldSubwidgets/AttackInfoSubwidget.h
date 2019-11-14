#pragma once
#include <QObject>
#include <QRect>
#include <QSize>
#include "Model/DataOperators/DamageDealingEngine.h"
#include <QTimer>
namespace View {
	using Model::statisticsPtr;
	class AttackInfoSubwidget : public QObject
	{
		Q_OBJECT
	protected:
		QRect InfoRectangle;
		QSize mySize;
		QPoint start;
		statisticsPtr dataToShow;
		bool hasInfo;
		bool hidden;
		QWidget* parentWidget;
		QTimer* timeoutTimer;
	public:
		AttackInfoSubwidget(QWidget * parentwidget, QObject* parent = nullptr);
		AttackInfoSubwidget(QSize size, QPoint start, QWidget* parentwidget, QObject* parent = nullptr);
		void resize(QSize sz);
		void move(QPoint point);
		void assignStatistics(QPoint where, statisticsPtr statptr);
		void show();
		void hide();
		void show(QPoint where);
		void draw(QPainter* painter);
		void clear();
		bool isHidden();
	public slots:
		void eraseAndHide();
	};
}