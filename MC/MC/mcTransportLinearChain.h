// Radiation Oncology Monte Carlo open source project
//
// Author: [2017] Gennady Gorlachev (ggorlachev@roiss.ru) 
//---------------------------------------------------------------------------
#pragma once
#include "mcTransport.h"

// Класс оболочки над линейной цепочкой. 
// Внутренние модули обмениваются частицами в обычном отсортированном по Z стиле.
// При окончании транспорта первым или последним модулем транспорта цепочки
// Частица передается данному классу оболочки подобно тому как работают вложенные модули.
// При получении частицы для транспорта сам отыскивает модкль цепочки, куда частица попадет в первую очередь.
class mcTransportLinearChain : public mcTransport
{
public:
   mcTransportLinearChain(const geomVector3D& orgn, const geomVector3D& z, const geomVector3D& x);

   virtual ~mcTransportLinearChain(void);

   void beginTransport(mcParticle& p) override;

   void beginTransportInside(mcParticle& p) override;

   double getDistanceOutside(mcParticle& p) const override;

   mcTransport* getInternalTransportByName(const char* name) override; // Добавление объекта транспорта цепочки.
   // Регистрация участников транспорта необходима для определения 
   // в какой транспорт попадет частица, когда она проникнет в цепочку снаружи.
   void addTransport(mcTransport* t); // Вызывается по окончании парсинга для заверешения установки всех связей
   void completeInit();

   void dump(ostream& os) const override;

   void dumpVRML(ostream& os) const override;

protected:
   std::vector<mcTransport*> chainTransports_;
};



