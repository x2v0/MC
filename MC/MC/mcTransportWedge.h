// Radiation Oncology Monte Carlo open source project
//
// Author: [2005-2017] Gennady Gorlachev (ggorlachev@roiss.ru) 
//---------------------------------------------------------------------------
#pragma once
#include "mctransport.h"

// Класс транспорта в треугольном клине.
// Центр системы координат объекта находится в середине перпендикулярной пучку поверхности.
// Острый край в напралении +Y (согласно IEC 62C).
// Высота толстого края клина только положительная
// Если нужно сделать отрицательной, то следует инвертировать ось Z.
class mcTransportWedge : public mcTransport
{
public:
   mcTransportWedge(void);

   mcTransportWedge(const geomVector3D& orgn, const geomVector3D& vz, const geomVector3D& vx, double ax, double ay,
                    double az);

   virtual ~mcTransportWedge(void);

   void setGeometry(double ax, double ay, double az); // Половины размеров клина в плоскости Z=const, и высота клина
   double ax() const
   {
      return ax_;
   }

   double ay() const
   {
      return ay_;
   }

   double az() const
   {
      return az_;
   }

   void dump(ostream& os) const override;

   void dumpVRML(ostream& os) const override;

protected:
   double getDistanceInside(mcParticle& p) const override;

   double getDistanceOutside(mcParticle& p) const override;

   double getDNearInside(const geomVector3D& p) const override;

protected:
   double ax_;
   double ay_;
   double az_;
   geomVector3D n_;  // нормаль к наклонной плоскости клина, направленная наружу
   geomVector3D cy_; // положение середины ребра острого угла клина
};



