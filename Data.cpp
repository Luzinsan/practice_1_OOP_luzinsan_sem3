#include "Data.h"
#include <iostream>
#include <cassert>// for assert

namespace luzinsan
{
	int Data::m_idGenerate = 1;

	//  онструктор по умолчанию.
	Data::Data() : Data(1,1,1970) // ƒелегирование выполнени€ конструктору с параметрами
	{
		std::cout << "\n«десь было делегирование конструктору с параметрами из конструктора по умолчанию. —оздан объект #" << m_id << std::endl;
	}
	// онструктор с параметрами
	Data::Data(int D, int M, int Y) : m_D{ D }, m_M{ M }, m_Y{ Y }, m_id{ m_idGenerate++ }
	{
		std::cout << "\n—работал конструктор по умолчанию. —оздан объект #" << m_id << std::endl;
	}
	// онструктор глубокого копировани€
	Data::Data(const Data& fromData) : Data(fromData.m_D, fromData.m_M, fromData.m_Y) // ƒелегирование выполнени€ конструктору с параметрами
	{
		std::cout << "\n«десь было делегирование конструктору с параметрами из конструктора глубокого копировани€. —оздан объект #" << m_id 
			      << " по подобию объекта #" << fromData.m_id << std::endl;
	}
	bool IsLeap(int year)
	{
		return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
	}
	void Data::SetData(int D, int M, int Y)
	{
		assert(D <= 31 && D >= 1 && M <= 12 && M >= 1 && Y >= 1 && "’от€ бы одно из заданных значений не входит во множество допустимых значений");
		if (IsLeap(Y) && M == 2) // високостный год и февраль
				assert((D >= 1 && D <= 29) && "¬ феврале високосного года день может может быть числом из отрезка [1;29]");
				
		switch(M)
		{
		case 2: case 4: case 6: case 9: case 11:  // провер€ем мес€цы, содержащие по 30 дней
			assert((D >= 1 && D <= 30) && "¬ этом мес€це день может быть числом из отрезка [1;30]");
		}

		// оставшиес€ мес€цы уже удовлетвор€ют всем услови€м, так как проверены в первом assert
		m_D = D; m_M = M; m_Y = Y;
	}
	void Data::SetDay(int D)
	{
		SetData(D, m_M, m_Y);
	}
	void Data::SetMonth(int M)
	{
		SetData(m_D, M, m_Y);
	}
	void Data::SetYear(int Y)
	{
		SetData(m_D, m_M, Y);
	}

	int Data::GetDay() const 
	{
		return m_D;
	}
	int Data::GetMonth() const
	{
		return m_M;
	}
	int Data::GetYear() const
	{
		return m_Y;
	}

	const Data& Data::operator=(const Data& fromData)
	{
		m_D = fromData.m_D;
		m_M = fromData.m_M;
		m_Y = fromData.m_Y;
		return *this;
	}
	std::ostream& operator<<(std::ostream& out, const Data& outData)
	{
		out << "\nќбъект  #" << outData.m_id << "=> "<< outData.m_D <<'/'<< outData.m_M << '/' << outData.m_Y << std::endl;
		return out;
	}
	bool Data::operator>(const Data& withData) 
	{
		return m_Y > withData.m_Y ? true 
			: (m_Y < withData.m_Y ? false     /*m_Y == withData.m_Y */
			: (m_M > withData.m_M ? true
			: (m_M < withData.m_M ? false     /*m_M == withData.m_M*/
			: (m_D > withData.m_D ? true
			:                       false))));/*this <= withData*/
	}
	bool Data::operator<(const Data& withData)
	{
		return m_Y < withData.m_Y ? true
			: (m_Y > withData.m_Y ? false     /*m_Y == withData.m_Y */
			: (m_M < withData.m_M ? true
			: (m_M > withData.m_M ? false     /*m_M == withData.m_M*/
			: (m_D < withData.m_D ? true
						          : false))));/*this >= withData*/
	}

	
	const Data& Data::operator+=(int day)
	{
		do{

			if(IsLeap(m_Y) && m_M == 2)
			{
				if (m_D + day > 29)
				{
					m_M++;
					day -= 30 - m_D;
					m_D = 1;
					continue;
				}
				else
				{
					m_D += day;
					return *this;
				}
			}
			switch (m_M)
			{
			case 2:
				if (m_D + day > 28)
				{
					m_M++;
					day -= 29 - m_D;
					m_D = 1;
				}
				else
				{
					m_D += day;
					return *this;
				}
				break;
				
			case 4: case 6: case 9: case 11:  // провер€ем мес€цы, содержащие по 30 дней
				if (m_D + day > 30)
				{
					m_M++;
					day -= 31 - m_D;
					m_D = 1;
				}
				else
				{
					m_D += day;
					return *this;
				}
				break;
			case 1: case 3: case 5: case 7: case 8: case 10:   // провер€ем мес€цы, содержащие по 31 день
				if (m_D + day > 31)
				{
					m_M++;
					day -= 32 - m_D;
					m_D = 1;
				}
				else
				{
					m_D += day;
					return *this;
				}
				break;
			case 12:
				if (m_D + day > 31)
				{
					m_Y++; 
					m_M = 1; 
					day -= 32 - m_D;
					m_D = 1;
				}
				else 
				{
					m_D += day;
					return *this;
				}
			}

		} while (true);
	
		return *this;
	}
	const Data& Data::operator-=(int day)
	{
		do {

			if (IsLeap(m_Y) && m_M == 3)
			{
				if (m_D - day <= 0)
				{
					m_M--;
					day -= m_D;
					m_D = 29;
					continue;
				}
				else
				{
					m_D -= day;
					return *this;
				}
			}
			switch (m_M)
			{
			case 3:
				if (m_D - day <= 0)
				{
					m_M--;
					day -= m_D;
					m_D = 28;
					continue;
				}
				else
				{
					m_D -= day;
					return *this;
				}
				break;
			case 2: case 4: case 6: case 8: case 9: case 11:  // провер€ем мес€цы, содержащие в предыдущих мес€цах по 30 дней
				if (m_D - day <= 0)
				{
					m_M--;
					day -= m_D;
					m_D = 31;
					continue;
				}
				else
				{
					m_D -= day;
					return *this;
				}
				break;
			case 5: case 7: case 10: case 12:    // провер€ем мес€цы, содержащие в предыдущих мес€цах по 31 день
				if (m_D - day <= 0)
				{
					m_M--;
					day -= m_D;
					m_D = 30;
					continue;
				}
				else
				{
					m_D -= day;
					return *this;
				}
				break;
			case 1:
				if (m_D - day <= 0)
				{
					m_Y--;
					m_M = 12;
					day -= m_D;
					m_D = 31;
					continue;
				}
				else
				{
					m_D -= day;
					return *this;
				}
			}

		} while (true);

		return *this;

	}

}