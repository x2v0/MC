﻿#include "ProfileProcessor.h"
using namespace std;

void ProfileProcessor::ProfileParameters(const std::vector<double>& x, const std::vector<double>& d,
                                         std::vector<double>& dd, double& width, double& penumbra, double& dindex)
{
   // Определяем примерный размер поля по полу высоте относительно максимальной дозы
   int i, imax = 0, np = static_cast<int>(x.size());
   double dmax = 0;
   dd.resize(np);
   for (i = 0; i < np; i++) {
      dd[i] = d[i];
      if (d[i] > dmax) {
         dmax = d[i];
         imax = i;
      }
   }
   dmax /= 2;
   for (i = 1; i < imax; i++) {
      if (d[i] > dmax)
         break;
   }
   double x1 = (x[i] + x[i - 1]) / 2;
   for (i = imax; i < np - 1; i++) {
      if (d[i] < dmax)
         break;
   }
   double x2 = (x[i] + x[i - 1]) / 2;
   double x0 = (x1 + x2) / 2;
   if (x2 < x1)
      throw std::exception("ProfileProcessor::ProfileParameters: point coordinates should be in increasing order");
   // Пределы апроксимации плато +/- 0.2 размера поля 
   width = x2 - x1;
   x1 += width * 0.3;
   x2 -= width * 0.3; // Аппроксимируем открытую часть симметричной параболой и подгоняем методом наименьших квадратов
   // ( d = a * x^2 + dmax )
   double A00 = 0, A01 = 0, A10 = 0, A11 = 0, B0 = 0, B1 = 0;
   for (i = 0; i < np; i++) {
      if (x[i] > x1 && x[i] < x2) {
         double xi = x[i] - x0;
         xi *= xi;
         A11 += 1.0;
         B1 += d[i];
         A10 += xi;
         A01 += xi;
         B0 += xi * d[i];
         A00 += xi * xi;
      }
   }
   dmax = (B0 * A10 - B1 * A00) / (A01 * A10 - A11 * A00);
   double a = (B0 * A11 - B1 * A01) / (A00 * A11 - A10 * A01);
   for (i = 0; i < np; i++) {
      if (x[i] > x1 && x[i] < x2) {
         double xi = x[i] - x0;
         dd[i] = a * xi * xi + dmax;
      }
   } // Аппроксимируем полутени на высоте +/- 0.2 dmax от полу высоты
   // ( d = a * x + b )
   double d1 = dmax * 0.3, d2 = dmax * 0.7;
   A00 = A01 = A10 = A11 = B0 = B1 = 0;
   for (i = 0; i < imax; i++) {
      double xi = x[i] - x0;
      if (d[i] > d1 && d[i] < d2) {
         A11 += 1.0;
         B1 += d[i];
         A10 += xi;
         A01 += xi;
         B0 += xi * d[i];
         A00 += xi * xi;
      }
   }
   double b = (B0 * A10 - B1 * A00) / (A01 * A10 - A11 * A00);
   a = (B0 * A11 - B1 * A01) / (A00 * A11 - A10 * A01);
   x1 = x0 + (dmax / 2 - b) / a;
   double pen1 = 0.6 * dmax / a;
   for (i = 0; i < imax; i++) {
      double xi = x[i] - x0;
      if (d[i] > d1 && d[i] < d2)
         dd[i] = a * xi + b;
   } // dindex1 - пределы стартуют от предсказания положения 80% по аппроксимации прямой линией
   double dindex1 = 0;
   int count = 0;
   double xx2 = x0 + (dmax * 0.8 - b) / a;
   double xx1 = xx2 - 3.0;
   for (i = 0; i < imax; i++) {
      if (x[i] >= xx1 && x[i] <= xx2) {
         dindex1 += d[i];
         count++;
      }
   }
   dindex1 /= count * dmax; // Правая полутень
   A00 = A01 = A10 = A11 = B0 = B1 = 0;
   for (i = imax; i < np; i++) {
      double xi = x[i] - x0;
      if (d[i] > d1 && d[i] < d2) {
         A11 += 1.0;
         B1 += d[i];
         A10 += xi;
         A01 += xi;
         B0 += xi * d[i];
         A00 += xi * xi;
      }
   }
   b = (B0 * A10 - B1 * A00) / (A01 * A10 - A11 * A00);
   a = (B0 * A11 - B1 * A01) / (A00 * A11 - A10 * A01);
   x2 = x0 + (dmax / 2 - b) / a;
   double pen2 = -0.6 * dmax / a;
   for (i = imax; i < np; i++) {
      double xi = x[i] - x0;
      if (d[i] > d1 && d[i] < d2)
         dd[i] = a * xi + b;
   } // dindex1 - пределы стартуют от предсказания положения 80% по аппроксимации прямой линией
   double dindex2 = 0;
   count = 0;
   xx1 = x0 + (dmax * 0.8 - b) / a;
   xx2 = xx1 + 3.0;
   for (i = imax; i < np; i++) {
      if (x[i] >= xx1 && x[i] <= xx2) {
         dindex2 += d[i];
         count++;
      }
   }
   dindex2 /= count * dmax;
   width = x2 - x1;
   penumbra = (pen1 + pen2) / 2;
   dindex = (dindex1 + dindex2) / (2 * width);
}

std::shared_ptr<vector<vector<double>>> ProfileProcessor::SmoothFanRZ(const vector<vector<double>>& srcMatrix,
                                                                      unsigned nr, unsigned nz, double rstep)
{
   //auto smatrix = make_shared<vector<vector<double>>>(nz, vector<double>(nr, 0));
   // Копия исходной матрицы
   auto smatrix = make_shared<vector<vector<double>>>(srcMatrix);
   // Определяем размер поля и пределы индексов для сглаживания профилей параболой.
   int ir50 = 0, izc = nz / 2;
   double d50 = srcMatrix[izc][0] / 2;
   for (ir50 = 0; ir50 < static_cast<int>(nr); ir50++) {
      if (srcMatrix[izc][ir50] <= d50)
         break;
   }
   if (ir50 == 0 || ir50 == nr)
      throw std::exception("ProfileProcessor::SmoothFanRZ: wrong profile (penumbra not found)");
   // Пределы индексов для параболической аппроксимации минимум половины поля и не ближе 1.5 см до его размера.
   int nr2 = (ir50 * rstep) > 3.0 ? ir50 / 2 : ir50 - int(1.5 / rstep);
   // Для слишком маленьких полей параболу не используем
   unsigned ir, iz;
   if (nr2 > 5) {
      for (iz = 0; iz < nz; iz++)
         SmoothRZProfile((*smatrix)[iz], nr2);
   } // Сглаживание вдоль FanLine
   vector<double> ptmp(nz);
   for (ir = 0; ir < nr; ir++) {
      for (iz = 0; iz < nz; iz++)
         ptmp[iz] = (*smatrix)[iz][ir];
      SmoothSG1D(ptmp, 2, 5, 5);
      for (iz = 0; iz < nz; iz++)
         (*smatrix)[iz][ir] = ptmp[iz];
   }
   return smatrix;
}

void ProfileProcessor::SmoothRZProfile(vector<double>& p, unsigned nr)
{
   // Аппроксимируем открытую часть симметричной параболой и подгоняем методом наименьших квадратов
   // ( y = y0 + a * x^2 )
   // Учитываем статистическую погрешность данных согласно площади колец детектора.
   unsigned i;
   double A00 = 0, A01 = 0, A10 = 0, A11 = 0, B0 = 0, B1 = 0;
   for (i = 0; i < nr; i++) {
      double xi = i + 0.5;
      double xi3 = xi * xi * xi;
      double xi5 = xi3 * xi * xi;
      A00 += xi5;
      A01 += xi3;
      A10 += xi3;
      A11 += xi;
      B0 += xi3 * p[i];
      B1 += xi * p[i];
   }
   double b = (B0 * A10 - B1 * A00) / (A01 * A10 - A11 * A00);
   double a = (B0 * A11 - B1 * A01) / (A00 * A11 - A10 * A01);
   for (i = 0; i < nr; i++)
      p[i] = a * (i + 0.5) * (i + 0.5) + b;
}

void ProfileProcessor::SmoothSG1D(vector<double>& p, unsigned m, unsigned nl, unsigned nr)
{
   const double m2l2r2[] = {-0.086, 0.343, 0.486, 0.343, -0.086};
   const double m2l1r3[] = {0.257, 0.372, 0.343, 0.171, -0.143};
   const double m2l5r5[] = {-0.084, 0.021, 0.103, 0.161, 0.196, 0.206, 0.196, 0.161, 0.103, 0.021, -0.084};
   const double m4l4r4[] = {0.035, -0.128, 0.07, 0.315, 0.416, 0.315, 0.07, -0.128, 0.035};
   unsigned np = static_cast<unsigned>(p.size());
   if (np <= nl + nr)
      return; // оставляем без изменения
   const double* c;
   if (m == 2 && nl == 2 && nr == 2)
      c = m2l2r2;
   else if (m == 2 && nl == 1 && nr == 3)
      c = m2l1r3;
   else if (m == 2 && nl == 5 && nr == 5)
      c = m2l5r5;
   else if (m == 4 && nl == 4 && nr == 4)
      c = m4l4r4;
   else
      throw std::exception("ProfileProcessor::SmoothSG1D: unsupported SG window)");
   int i, j;
   vector<double> s(p);
   for (i = nl; i < static_cast<int>(np - nr); i++) {
      double f = 0;
      for (j = -static_cast<int>(nl); j <= static_cast<int>(nr); j++)
         f += s[i + j] * c[j + nl];
      p[i] = f;
   }
}





