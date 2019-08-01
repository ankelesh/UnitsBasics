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
	const int MODEL_ABSTRACT_CLASS_ID = 1;
	class abs_damage
	{
		/*
			Базовый класс для всех видов урона. Идеальные условия : все объекты сначала проверяются
			на радиус через inRange(), потом рассчитывают countDamage() и makeNoise(). 
			getInitiative() -> для инициализации PriorityQueue;
			
		*/
	protected:
		const int class_id = 0;

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
		int makeNoise();
		int countDamage(Defences&, const int, const bool );		//  интерфейс объекта, все внешние взаимодействия только через него
		int inRange(const int);
		int getInitiative(const bool);
		int countDamageToForts(Defences&, const bool);
		OSTR_TYPE serialize();
		abs_damage* clone();
	};


	typedef std::unique_ptr<abs_damage> damagePointer;
}
