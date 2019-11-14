#include "FieldWidget.h"
#include <QOpenGLContext>
#include <QOpenGlFunctions>
#include <QPainter>
#include "UnitPainting/UnitPaintingFunctions.h"
#include "TerrainPainting/TerrainPaintingFunctions.h"
#include <QtOpenGL/QGlWidget>
#include "debugtrace.h"

void View::FieldWidget::initializeGL()
{
	functs = QOpenGLContext::currentContext()->functions();
	functs->glClearColor(255.0f,
		255.0f, 255.0f, 1.0f);
}

void View::FieldWidget::resizeGL(int w, int h)
{
	int side = qMin(w, h);
	glViewport((w - side) / 2, (h - side) / 2, side, side);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef QT_OPENGL_ES_1
	glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
	glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
	glMatrixMode(GL_MODELVIEW);
}

void View::FieldWidget::paintGL()
{
	painter->begin(this);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setRenderHint(QPainter::TextAntialiasing, true);
	if (offset_invalid(viewMap.offsetBorder()))
	{
		painter->end();
		return;
	}
	auto begin = viewMap.begin();
	Hexagon hg;
	while (begin != viewMap.end())
	{
		auto pix = begin->first.toPixel(50, true);
		if (checkOffsetCompatibility(pix, offset.x(), offset.y(), width(), height(), 50))
		{
			hg = Hexagon(QPoint(pix.first - offset.x(), pix.second - offset.y()), 50);
			drawTerrain(begin->second.getTerrainView(), painter, hg);
			drawUnit(begin->second.getUnitView(), painter, hg);
			++begin;
		}
		else
		{
			++begin;
			continue;
		}
	}
	if (!actionInfo->isHidden())
	{
		actionInfo->draw(painter);
	}
	painter->end();
}

void View::FieldWidget::mouseMoveEvent(QMouseEvent* mev)
{
	if (oldMousePos == QPoint(0, 0))
	{
		oldMousePos = mev->pos();
		return;
	}
	else
	{
		offset -= oldMousePos - mev->pos();
		if (scrollUpdateTimer.msecsTo(QTime::currentTime()) > 25)
		{
			scrollUpdateTimer = QTime::currentTime();
			update();
		}
		oldMousePos = mev->pos();
		return;
	}
}

void View::FieldWidget::mousePressEvent(QMouseEvent* mev)
{
	oldMousePos = mev->pos();
	QWidget::mousePressEvent(mev);
}

bool View::FieldWidget::offset_invalid(QRect& mapRect)
{
	QPoint offsettedHexfieldBorder(offset.x() + width(), offset.y() + height());
	if (mapRect.contains(offset.x() - 50, offset.y() - 50))
	{
		if (mapRect.contains(offsettedHexfieldBorder))
		{
			return false;
		}
	}
	return false;

}

View::FieldWidget::FieldWidget(Model::HexViewMap& vMap, QWidget* parent)
	: QOpenGLWidget(parent), viewMap(vMap), painter(new QPainter(this)), offset(-50, -50), oldMousePos(0,0), scrollUpdateTimer(QTime::currentTime()),
	actionInfo(new AttackInfoSubwidget(this, this))
{
	QSurfaceFormat fmt;
	fmt.setSamples(8);
	this->setFormat(fmt);
}

void View::FieldWidget::setFightResult(statisticsPtr statptr, ICube defender)
{
	auto pix = defender.toPixel(50, true);
	actionInfo->assignStatistics(
		QPoint(pix.first - offset.x() + 60, pix.second - offset.y() + 10), std::move(statptr));
	actionInfo->show();

}
