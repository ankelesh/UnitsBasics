#pragma once
#define QT_NO_DEBUG_STREAM
#include "View/FieldWidget.h"
#include "Model/Hexbattlefield/HexMainMap.h"
#include <QObject>
#include "Model/DataOperators/DamageDealingEngine.h"

/*
	Простейший из контроллеров. Используется для тестов. 



*/



namespace Controller
{
	using namespace Model;
	using namespace View;
	class SimpliestController : public QObject
	{
		Q_OBJECT
	private:
		HexMainMap mainMap;
		HexViewMap viewMap;


		FieldWidget* innerWidget;

	public:
		SimpliestController(QObject* parent);
		void show();
		void attack(ICube fcoord, ICube scoord);
		~SimpliestController();
	public slots:
		void do_thing();
	};
}