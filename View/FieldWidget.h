#pragma once
#define QT_NO_DEBUG_STREAM
#include "Model/Hexbattlefield/HexMainMap.h"
#include "Model/Hexbattlefield/Hexagon.h"
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
namespace View 
{
	class FieldWidget : public QOpenGLWidget
	{
	private:
		Model::HexViewMap & viewMap;
		QPainter* painter;
		QOpenGLFunctions* functs;
		
	protected:
		virtual void initializeGL() override;
		virtual void resizeGL(int w, int h) override;
		virtual void paintGL() override;
	public:
		FieldWidget(Model::HexViewMap& vMap, QWidget* parent);





	};
}