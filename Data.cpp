#include "Data.h"
#include <iostream>
#include <cassert>// for assert

namespace luzinsan
{
	int Data::m_idCounter = 1;

	// Конструктор по умолчанию.
	Data::Data() : Data(1,1,1970) // Делегирование выполнения конструктору с параметрами
	{
		std::cout << "\nЗдесь было делегирование конструктору с параметрами из конструктора по умолчанию. Создан объект #" << m_id << std::endl;
	}
	//Конструктор с параметрами
	Data::Data(int D, int M, int Y) : m_Day{ D }, m_Month{ M }, m_Y{ Y }, m_id{ m_idCounter++ }
	{
		std::cout << "\nСработал конструктор по умолчанию. Создан объект #" << m_id << std::endl;
	}
	//Конструктор глубокого копирования
	Data::Data(const Data& fromData) : Data(fromData.m_Day, fromData.m_Month, fromData.m_Y) // Делегирование выполнения конструктору с параметрами
	{
		std::cout << "\nЗдесь было делегирование конструктору с параметрами из конструктора глубокого копирования. Создан объект #" << m_id 
			      << " по подобию объекта #" << fromData.m_id << std::endl;
	}
	bool IsLeap(int year)
	{
		return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
	}
	void Data::SetData(int D, int M, int Y)
	{
		assert(D <= 31 && D >= 1 && M <= 12 && M >= 1 && Y >= 1 && "Хотя бы одно из заданных значений не входит во множество допустимых значений");
		if (IsLeap(Y) && M == 2) // високостный год и февраль
				assert((D >= 1 && D <= 29) && "В феврале високосного года день может может быть числом из отрезка [1;29]");
				
		switch(M)
		{
		case 2: case 4: case 6: case 9: case 11:  // проверяем месяцы, содержащие по 30 дней
			assert((D >= 1 && D <= 30) && "В этом месяце день может быть числом из отрезка [1;30]");
		}

		// оставшиеся месяцы уже удовлетворяют всем условиям, так как проверены в первом assert
		m_Day = D; m_Month = M; m_Y = Y;
	}
	void Data::SetDay(int D)
	{
		SetData(D, m_Month, m_Y);
	}
	void Data::SetMonth(int M)
	{
		SetData(m_Day, M, m_Y);
	}
	void Data::SetYear(int Y)
	{
		SetData(m_Day, m_Month, Y);
	}

	int Data::GetDay() const 
	{
		return m_Day;
	}
	int Data::GetMonth() const
	{
		return m_Month;
	}
	int Data::GetYear() const
	{
		return m_Y;
	}
	int Data::GetId() const
	{
		return m_id;
	}

	const Data& Data::operator=(const Data& fromData)
	{
		m_Day = fromData.m_Day;
		m_Month = fromData.m_Month;
		m_Y = fromData.m_Y;
		return *this;
	}
	std::ostream& operator<<(std::ostream& out, const Data& outData)
	{
		out << "\nОбъект  #" << outData.m_id << "=> "<< outData.m_Day <<'/'<< outData.m_Month << '/' << outData.m_Y << std::endl;
		return out;
	}
	bool Data::operator>(const Data& withData) 
	{
		return m_Y > withData.m_Y ? true 
			: (m_Y < withData.m_Y ? false     /*m_Y == withData.m_Y */
			: (m_Month > withData.m_Month ? true
			: (m_Month < withData.m_Month ? false     /*m_M == withData.m_M*/
			: (m_Day > withData.m_Day ? true
			:                       false))));/*this <= withData*/
	}
	bool Data::operator<(const Data& withData)
	{
		return m_Y < withData.m_Y ? true
			: (m_Y > withData.m_Y ? false     /*m_Y == withData.m_Y */
			: (m_Month < withData.m_Month ? true
			: (m_Month > withData.m_Month ? false     /*m_M == withData.m_M*/
			: (m_Day < withData.m_Day ? true
						          : false))));/*this >= withData*/
	}

	
	const Data& Data::operator+=(int day)
	{
		do{

			if(IsLeap(m_Y) && m_Month == 2) // если високосный год и февраль
			{
				if (m_Day + day > 29)
				{
					m_Month++;
					day -= 30 - m_Day;
					m_Day = 1;
					continue;
				}
				else
				{
					m_Day += day;
					return *this;
				}
			}
			switch (m_Month)
			{
			case 2: // обработка февраля в невисокосном году
				if (m_Day + day > 28)
				{
					m_Month++;
					day -= 29 - m_Day;
					m_Day = 1;
				}
				else
				{
					m_Day += day;
					return *this;
				}
				break;
				
			case 4: case 6: case 9: case 11:  // обработка месяцев, содержащих по 30 дней
				if (m_Day + day > 30)
				{
					m_Month++;
					day -= 31 - m_Day;
					m_Day = 1;
				}
				else
				{
					m_Day += day;
					return *this;
				}
				break;
			case 1: case 3: case 5: case 7: case 8: case 10:   // обработка месяцев, содержащих по 31 день
				if (m_Day + day > 31)
				{
					m_Month++;
					day -= 32 - m_Day;
					m_Day = 1;
				}
				else
				{
					m_Day += day;
					return *this;
				}
				break;
			case 12: // обработка декабря
				if (m_Day + day > 31)
				{
					m_Y++; 
					m_Month = 1; 
					day -= 32 - m_Day;
					m_Day = 1;
				}
				else 
				{
					m_Day += day;
					return *this;
				}
			}

		} while (true);
	
		return *this;
	}
	const Data& Data::operator-=(int day)
	{
		do {

			if (IsLeap(m_Y) && m_Month == 3) // если год високосный и предыдущий месяц - февраль
			{
				if (m_Day - day <= 0)
				{
					m_Month--;
					day -= m_Day;
					m_Day = 29;
					continue;
				}
				else
				{
					m_Day -= day;
					return *this;
				}
			}
			switch (m_Month)
			{
			case 3: // если год невисокосный и предыдущий месяц - февраль
				if (m_Day - day <= 0)
				{
					m_Month--;
					day -= m_Day;
					m_Day = 28;
					continue;
				}
				else
				{
					m_Day -= day;
					return *this;
				}
				break;
			case 2: case 4: case 6: case 8: case 9: case 11:  // обработка месяцев, содержащие в предыдущих месяцах по 30 дней
				if (m_Day - day <= 0)
				{
					m_Month--;
					day -= m_Day;
					m_Day = 31;
					continue;
				}
				else
				{
					m_Day -= day;
					return *this;
				}
				break;
			case 5: case 7: case 10: case 12:    // обработка месяцев, содержащих в предыдущих месяцах по 31 день
				if (m_Day - day <= 0)
				{
					m_Month--;
					day -= m_Day;
					m_Day = 30;
					continue;
				}
				else
				{
					m_Day -= day;
					return *this;
				}
				break;
			case 1: // обработка первого месяца в году - декабря
				if (m_Day - day <= 0) // если при вычитании мы выходим за рамки месяца, а значит и года
				{
					m_Y--;
					m_Month = 12;
					day -= m_Day;
					m_Day = 31;
					continue;
				}
				else // если при вычитании дней мы остаемся в рамках этого месяца, то остаёмся и в рамках этого года
				{
					m_Day -= day;
					return *this;
				}
			}

		} while (true);

		return *this;

	}

}
