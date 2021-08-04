// Radiation Oncology Monte Carlo open source project
//
// Author: [2005-2017] Gennady Gorlachev (ggorlachev@roiss.ru) 
//---------------------------------------------------------------------------
#pragma once
#include <math.h>
#include <iostream>
using namespace std;

class geomVector2D
{
public:
   geomVector2D()
   {
      p_[0] = 0;
      p_[1] = 0;
   }

   geomVector2D(double x, double y)
   {
      p_[0] = x;
      p_[1] = y;
   }

   double& operator()(int n)
   {
      return p_[n];
   }

   double operator()(int n) const
   {
      return p_[n];
   }

   double x() const
   {
      return p_[0];
   }

   double y() const
   {
      return p_[1];
   }

   void set(double x, double y)
   {
      p_[0] = x;
      p_[1] = y;
   }

   double length() const
   {
      return sqrt(p_[0] * p_[0] + p_[1] * p_[1]);
   }

   double sqLength() const
   {
      return p_[0] * p_[0] + p_[1] * p_[1];
   }

   void normalize()
   {
      double f = length();
      p_[0] /= f;
      p_[1] /= f;
   }

   void turnLeft()
   {
      double f = p_[0];
      p_[0] = -p_[1];
      p_[1] = f;
   }

   void turnRight()
   {
      double f = p_[0];
      p_[0] = p_[1];
      p_[1] = -f;
   } // ������� ������� �� ������� ������� �� ���� � ��������
   void turn(double a)
   {
      double x = p_[0], y = p_[1], sa = sin(a), ca = cos(a);
      p_[0] = x * ca + y * sa;
      p_[1] = -x * sa + y * ca;
   }

   geomVector2D operator*(double f) const
   {
      return geomVector2D(p_[0] * f, p_[1] * f);
   }

   geomVector2D operator+(const geomVector2D& v) const
   {
      return geomVector2D(p_[0] + v.x(), p_[1] + v.y());
   }

   geomVector2D operator-(const geomVector2D& v) const
   {
      return geomVector2D(p_[0] - v.x(), p_[1] - v.y());
   }

   double operator*(const geomVector2D& v) const
   {
      return p_[0] * v.x() + p_[1] * v.y();
   }

   void operator*=(double f)
   {
      p_[0] *= f;
      p_[1] *= f;
   }

   void operator/=(double f)
   {
      p_[0] /= f;
      p_[1] /= f;
   }

   void operator+=(const geomVector2D& v)
   {
      p_[0] += v.x();
      p_[1] += v.y();
   }

   void operator-=(const geomVector2D& v)
   {
      p_[0] -= v.x();
      p_[1] -= v.y();
   }

   bool operator==(const geomVector2D& v) const
   {
      return p_[0] == v.x() && p_[1] == v.y();
   }

   void operator=(const geomVector2D& v)
   {
      p_[0] = v.x();
      p_[1] = v.y();
   }

protected:
   double p_[2];
};

geomVector2D crossTwo2DLines(const geomVector2D& p1, const geomVector2D& p2, const geomVector2D& p3,
                             const geomVector2D& p4);
