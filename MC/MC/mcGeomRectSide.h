// Radiation Oncology Monte Carlo open source project
//
// Author: [2005-2017] Gennady Gorlachev (ggorlachev@roiss.ru) 
//---------------------------------------------------------------------------
#pragma once
#include "mcgeomside.h"

//Purpose:  Вспомогательный класс для расчета расстояния до.
//прямоугольника, произвольно расположенного в пространстве и 
//представляющего грань трехмерного объекта.
class mcGeomRectSide : public mcGeomSide
{
public:
   mcGeomRectSide(const geomVector3D& p, const geomVector3D& Vx, const geomVector3D& Vy, double ax, double ay);

   double getDistance(const geomVector3D& p, const geomVector3D& v, bool inside) const override;

   double getDNear(const geomVector3D& p) const override;

   void dump(ostream& os) const override {}
protected:           // Параметры сообщаемые извне
   geomVector3D P_;  // координаты основной вершины
   geomVector3D Vx_; // векторы сторон из вершины (в мировой системе)
   geomVector3D Vy_;
   double ax_;      // размеры сторон прямоугольника
   double ay_;      // Производные параметры
   geomVector3D N_; // нормаль к плоскости прямоугольника
};
