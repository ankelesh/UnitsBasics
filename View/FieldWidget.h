#pragma once
#define QT_NO_DEBUG_STREAM
#include "Model/Hexbattlefield/HexMainMap.h"
#include "Model/Hexbattlefield/Hexagon.h"
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include <QTime>
#include "View/OnFieldSubwidgets/AttackInfoSubwidget.h"

namespace View 
{
	class FieldWidget : public QOpenGLWidget
	{
	private:
		Model::HexViewMap & viewMap;
		QPainter* painter;
		QOpenGLFunctions* functs;
		QPoint offset;
		QPoint oldMousePos;
		QTime scrollUpdateTimer;
		AttackInfoSubwidget* actionInfo;
		QTimer* popupTimeoutTimer;
	protected:
		virtual void initializeGL() override;
		virtual void resizeGL(int w, int h) override;
		virtual void paintGL() override;
		virtual void mouseMoveEvent(QMouseEvent*) override;
		virtual void mousePressEvent(QMouseEvent*) override;
		bool offset_invalid(QRect&);
	public:
		FieldWidget(Model::HexViewMap& vMap, QWidget* parent);
		void setFightResult(statisticsPtr statptr, ICube defender);




	};
}