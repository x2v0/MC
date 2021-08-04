// Radiation Oncology Monte Carlo open source project
//
// Author: [2005-2017] Gennady Gorlachev (ggorlachev@roiss.ru) 
//---------------------------------------------------------------------------
#pragma once
// ����� �������� �������� ��������� �������.
// �� ����� ���� ����������� ���� ������� ��������� ����� � ��������� [0, 1].
class mcSamplers
{
public: // ��������� ������������ ������������� �� ������� [-1, 1].
   static double SamplePrism(double rnd);

   // ��������� ������������� ������������ ������������� �� ������� [-1, 1] � ����� � 0.
   static double SampleTriangle(double rnd); // ��������� ����������� ��������� � ����� = 1.
   static double SampleGauss(double rnd);

   // ��������� ������� x*exp(-x^2/2), ������������, ��������, ��� ��������� �������
   // ��������� �������������� ������� ���� ���������.
   static double SampleGauss2D(double rnd); // �������� ������� �� ����������������� ������������� ������������� ��� 
   // ��������� ���������� ����������.
   // ��������� ������� x*exp(-x), ������������, ��������, ��� ��������� �������
   // ��������������� ������������� ����� ����������.
   static double SampleExponent2D(double rnd);
};
