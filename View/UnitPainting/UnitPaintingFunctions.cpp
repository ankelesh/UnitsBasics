#include "UnitPaintingFunctions.h"
#include <array>
#include <QPixmap>
#include <QHash>
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif


std::array<QColor, 3> teamcolors = { QColor(140, 70, 78), QColor(7, 103, 237), QColor(137, 0, 222) };
QHash<uint, QPixmap> pixmapHash;


void drawFrontmap(Hexagon& hg, QPainter* painter, const std::array<short, 6>& fm, const short teamcolor)
{
	auto start = fm.begin();
	int corner = 1;
	while (start != fm.end())
	{
		painter->setPen((
			(*start) > Model::front::normal) ? 
					QPen(teamcolors.at(teamcolor), 2) :
					QPen(QColor(255, 0, 30), 2, Qt::DotLine));
		switch (*start)
		{
		case Model::front::strong:
		case Model::front::vulnerable:
			painter->drawLine(
				find_corner(hg.center, hg.size * 0.8, corner - 1),
				find_corner(hg.center, hg.size * 0.8, corner)
			);
		case Model::front::reinforced:
		case Model::front::unprotected:
			painter->drawLine(
				find_corner(hg.center, hg.size * 0.9, corner - 1),
				find_corner(hg.center, hg.size * 0.9, corner)
			);
		default:
			break;
		}
		++corner;
		++start;
	}
}



void unit_shader_1(const Model::unit_map_view_charset& uch, QPainter* painter, Hexagon& hg)
{
	painter->setPen(QPen(teamcolors.at(uch.teamcolor), 2));
	painter->setBrush(Qt::NoBrush);
	QRect rect(find_corner(hg.center, hg.size * 0.4, 4), find_corner(hg.center, hg.size * 0.4, 1));
	painter->drawRect(rect);
	painter->drawLine(rect.topLeft(), rect.bottomRight());
	painter->drawLine(rect.bottomLeft(), rect.topRight());
	painter->setPen(QPen());
	painter->setBrush(QBrush());
}



void View::drawUnit(const Model::unit_map_view_charset& uch, QPainter* painter, Hexagon & hg)
{
	detrace_METHCALL("drawUnit of team" << uch.teamcolor << "and color "<< teamcolors.at(uch.teamcolor).red());
	if (!uch.isTextured)
	{
		switch (uch.shaderId)
		{
		case 1:
			unit_shader_1(uch, painter, hg);
			break;
		case 0:
			return;
		default:
			break;
		}
	}
	else
	{
		if (!pixmapHash.contains(uch.textureHash))
		{
			pixmapHash.insert(uch.textureHash, QPixmap(QString::fromStdWString(uch.texturePath)));
		}
		QRect rect(find_corner(hg.center, hg.size * 0.7, 4), find_corner(hg.center, hg.size * 0.7, 1));
		painter->drawPixmap(rect, pixmapHash.value(uch.textureHash));
	}
	QPoint center = find_corner(hg.center, hg.size * 0.5, 2);
	QRect rect = QRect(center, QSize(hg.size * 0.43, hg.size * 0.25));
	rect.moveCenter(center);
	painter->setBrush(teamcolors.at(uch.teamcolor));
	painter->setPen(Qt::black);
	painter->drawRect(rect);
	painter->setPen(Qt::white);
	painter->drawText(rect, Qt::AlignHCenter | Qt::AlignTop, QString::number(uch.force));
	drawFrontmap(hg, painter, uch.frontmap, uch.teamcolor);
	painter->setPen(QPen());
	painter->setBrush(QBrush());
}
