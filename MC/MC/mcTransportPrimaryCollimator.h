﻿// Radiation Oncology Monte Carlo open source project
//
// Author: [2005-2017] Gennady Gorlachev (ggorlachev@roiss.ru) 
//---------------------------------------------------------------------------
#pragma once
#include "mctransport.h"

//Класс транспорта в первичном коллиматоре.
//Первичный коллиматор представляет собой бесконечный слой(slab),
//в котором сделано коническое (cone) отверстие
//Примечание: из-за ошибок округления может получится ситуация,
//когда частица формально В/Вне коллиматора, а на самом деле 
//частица Вне/В коллиматоре. Такая ошибка может привести к 
//"залипанию" частицы на границе раздела
class mcTransportPrimaryCollimator : public mcTransport
{
public:
   mcTransportPrimaryCollimator(void);

   mcTransportPrimaryCollimator(const geomVector3D& orgn, const geomVector3D& z, const geomVector3D& x, double r0,
                                double r1, double h);

   virtual ~mcTransportPrimaryCollimator(void);

   void setGeometry(double r0, double r1, double h);

   double R0() const
   {
      return r0_;
   }

   double R1() const
   {
      return r1_;
   }

   double getHeight() const
   {
      return h_col_;
   }

   void dump(ostream& os) const override;

   void dumpVRML(ostream& os) const override;

protected:
   double getDistanceInside(mcParticle& p) const override;

   double getDistanceOutside(mcParticle& p) const override;

   double getDNearInside(const geomVector3D& p) const override;

protected:
   double r0_;    //	верхний(малый) радиус коллиматора
   double r1_;    //	нижний(большой) радиус коллиматора
   double h_col_; //	толщина коллиматора
   double A_;     //	квадрат тангенса угла накллона конуса
};



