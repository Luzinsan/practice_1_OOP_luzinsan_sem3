#pragma once
#include <iostream>

namespace luzinsan
{
    class Data
    {
    private:
        int m_D, m_M, m_Y;
        int m_id;
        static int m_idGenerate;
    public:
        Data();
        Data(int,int,int);
        Data(const Data&);
        void SetData(int,int,int);
        void SetDay(int);
        void SetMonth(int);
        void SetYear(int);
        int GetDay() const;
        int GetMonth() const;
        int GetYear() const;
        const Data& operator=(const Data&);
        friend std::ostream& operator<<(std::ostream& out, const Data& outData);
        const Data& operator+=(int day);
        const Data& operator-=(int day);
        bool operator>(const Data& withData);
        bool operator<(const Data& withData);
    };
}