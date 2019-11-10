#pragma once
#ifndef OSTR_TYPE
#define OSTR_TYPE std::wstring
#define OSTR_RETURN return sout.str();
#define OSTR_CHTYPE wchar_t
#endif
#include <iostream>
#include <memory>
#include "Defences.h"

namespace Model
{
	const int MODEL_ABSTRACT_CLASS_ID = 1;	// Не помню зачем эта константа тут
	class abs_damage
	{
		/*
			Базовый класс для всех видов урона. Идеальные условия : все объекты сначала проверяются
			на радиус через inRange(), потом рассчитывают countDamage() и makeNoise(). 
			getInitiative() -> для инициализации PriorityQueue;
			
			У каждого юнита обычно есть несколько типов встроенного урона которые применяются по очереди,
			распределяясь по параметру инициативы. Каждый тип урона сам решает как обрабатывать
			защитные параметры.

			Каждый тип урона после нанесения генерирует шум, который отмечается на карте радиозвуковой 
			разведки. 

			Каждый тип урона может наносить урон по фортификациям, при этом он не может превысить ограничений 
			самих укреплений.

			См. защиту - каждый тип урона имеет параметр бронебойности, который влияет на то, может ли 
			быть урон нанесён. То как воспринимать защиту - решает сам вид урона. К примеру рокское пехотное
			оружие учитывает как противопехотную, так и HE-броню.

			Тем не менее, весь функционал класса вынесен в небольшое количество публичных методов
		*/
	protected:
		const int class_id = 0; // Идентификатор класса нужен для того чтобы при десериализации понять какой тип урона хранится в строке

		virtual OSTR_TYPE serialization() = 0;
		virtual void deserialization(OSTR_TYPE&) = 0;
		virtual void deserialization(std::wistream&) = 0;
		virtual int calcnoise() = 0;	//Возвращает количество шума данного типа урона. 
		//Можно рассчитывать его на основе урона или ещё чего-либо
		virtual bool canRetaliate() = 0;	//Возвращает возможность контратаки
		virtual int calcdmg(const int, const bool) = 0;	//возвращает amount. 
		virtual bool canReach(const int) = 0;	//проверяет достигаемость 
		virtual bool canPenetrate(Defences&) = 0;	//проверяет бронебойность
		virtual double calcResist(Defences&) = 0;	//возвращает процент поглощённого урона
		virtual int calcDP(Defences&) = 0;	//возвращает число поглощённых единиц урона
		virtual int damageFortifications(Defences&, const bool) = 0; // возвращает урон по фортификациям
		virtual int calcIni(const bool) = 0;		//	Возвращает инициативу
		virtual abs_damage* fabricate() = 0;
	public:
		// Возвращает количество произведённого шума
		int makeNoise();
		// Возвращает количество нанесённого урона
		int countDamage(Defences&, const int, const bool );		//  интерфейс объекта, все внешние взаимодействия только через него
		// Возвращает true если урон может быть нанесён на таком расстоянии
		int inRange(const int);
		// Возвращает инициативу этого типа урона
		int getInitiative(const bool);
		// возвращает итоговое количество урона по укреплениям
		int countDamageToForts(Defences&, const bool);
		//сериализует объект вне зависимости от его класса
		OSTR_TYPE serialize();
		// клонирует объект
		abs_damage* clone();
	};


	typedef std::shared_ptr<abs_damage> damagePointer;	// этот поинтер предназначен для полиморфного хранения урона
}
