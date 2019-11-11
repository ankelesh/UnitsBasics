#include "FieldWidget.h"
#include <QOpenGLContext>
#include <QOpenGlFunctions>
#include <QPainter>
#include "UnitPainting/UnitPaintingFunctions.h"
#include "TerrainPainting/TerrainPaintingFunctions.h"

void View::FieldWidget::initializeGL()
{
	functs = QOpenGLContext::currentContext()->functions();
	functs->glClearColor(255.0f,
		255.0f, 255.0f, 1.0f);
}

void View::FieldWidget::resizeGL(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLint)w, (GLint)h);
	glOrtho(0, width(), height(), 0, -1, -1);
}

void View::FieldWidget::paintGL()
{
	painter->begin(this);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setRenderHint(QPainter::TextAntialiasing);
	auto begin = viewMap.begin();
	Hexagon hg;
	while (begin != viewMap.end())
	{
		auto pix = begin->first.toPixel(50, true);
		hg = Hexagon(QPoint(pix.first, pix.second), 50);
		drawTerrain(begin->second.getTerrainView(), painter, hg);
		drawUnit(begin->second.getUnitView(), painter, hg);
		++begin;
	}
}

View::FieldWidget::FieldWidget(Model::HexViewMap& vMap, QWidget* parent)
	: QOpenGLWidget(parent), viewMap(vMap), painter(new QPainter(this))
{
}
