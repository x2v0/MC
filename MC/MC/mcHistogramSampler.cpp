﻿#include "mcHistogramSampler.h"
#include <strstream>
using namespace std;
mcHistogramSampler::mcHistogramSampler() : np_(0), step_(0), data_(nullptr) {}

mcHistogramSampler::~mcHistogramSampler()
{
   if (data_ != nullptr)
      delete[] data_;
}

double mcHistogramSampler::sample(double f) const
{
   if (f < 0 || f > 1)
      throw std::exception("mcHistogramSampler::sample supports sumpling from 0 to 1 inclusive only");
   int idx = int(f / step_), idx2 = idx + 1;
   if (idx2 >= np_) {
      idx2 = np_ - 1;
      idx = idx2 - 1;
   }
   double e = data_[idx] + (data_[idx2] - data_[idx]) * (f / step_ - idx);
   return e;
}

void mcHistogramSampler::init(int np)
{
   if (np < 2)
      throw std::exception("mcHistogramSampler::init number of points should be 2 or more");
   if (data_ != nullptr)
      delete[] data_;
   np_ = np;
   data_ = new double[np_];
   step_ = 1.0 / (np_ - 1);
}

void mcHistogramSampler::restore(int np, const double* data)
{
   init(np);
   memcpy(data_, data, np * sizeof(double));
}

void mcHistogramSampler::restore(int np, const vector<double>& data)
{
   init(np);
   double* p = data_;
   for (int i = 0; i < np; i++, p++)
      *p = data[i];
}

void mcHistogramSampler::make_comulative(vector<double>& c, int nbins, const double* data)
{
   c.resize(nbins + 1);
   c[0] = 0;
   int i;
   for (i = 0; i < nbins; i++)
      c[i + 1] = c[i] + data[i];
   if (c[nbins] > 0) {
      double f = 1.0 / c[nbins];
      for (i = 0; i < nbins; i++)
         c[i] *= f;
   } else {
      for (i = 1; i < nbins; i++)
         c[i] = 1.0;
   }
   c[nbins] = 1.0;
}

void mcHistogramSampler::make_comulative(vector<double>& c, const vector<double>& data)
{
   int nbins = static_cast<int>(data.size());
   c.resize(nbins + 1);
   c[0] = 0;
   int i;
   for (i = 0; i < nbins; i++)
      c[i + 1] = c[i] + data[i];
   if (c[nbins] > 0) {
      double f = 1.0 / c[nbins];
      for (i = 0; i < nbins; i++)
         c[i] *= f;
   } else {
      for (i = 1; i < nbins; i++)
         c[i] = 1.0;
   }
   c[nbins] = 1.0;
}

void mcHistogramSampler::set_from_comulative(int np, double par_min, double bin_size, const vector<double>& data)
{
   init(np + 1);
   int i, k = 1, nk = static_cast<int>(data.size());
   double dprev = data[0];
   for (i = 1; i < np; i++) {
      double f = i * step_;
      while (data[k] < f && k < nk) {
         dprev = data[k];
         k++;
      }
      data_[i] = par_min + (k - 1 + (f - dprev) / (data[k] - dprev)) * bin_size;
   }
   *data_ = par_min;
   data_[np] = par_min + bin_size * (nk - 1);
}

void mcHistogramSampler::setFromDistribution(int np, int nbins, double par_min, double bin_size, const double* data)
{
   vector<double> c;
   make_comulative(c, nbins, data);
   set_from_comulative(np, par_min, bin_size, c);
}

void mcHistogramSampler::setFromDistribution(int np, double par_min, double bin_size, const vector<double>& data)
{
   vector<double> c;
   make_comulative(c, data);
   set_from_comulative(np, par_min, bin_size, c);
}

void mcHistogramSampler::setFromDistribution(int np, int nbins, const double* ebins, const double* data)
{
   vector<double> c;
   make_comulative(c, nbins, data);
   init(np + 1); // Energies argument contains energies in the medle of bin.
   // Calculate energy bins boundaries.
   vector<double> e(nbins + 1);
   e[0] = 0;
   int i;
   for (i = 0; i < nbins; i++)
      e[i + 1] = ebins[i];
   int k = 1, nk = static_cast<int>(c.size());
   data_[0] = 0;
   for (i = 1; i <= np; i++) {
      double f = i * step_;
      while (c[k] < f && k < nk) {
         k++;
      }
      data_[i] = e[k - 1] + (e[k] - e[k - 1]) * (f - c[k - 1]) / (c[k] - c[k - 1]);
   }
}

ostream& operator <<(ostream& os, const mcHistogramSampler& h)
{
   os << "NP =" << '\t' << h.np_ << endl;
   for (int i = 0; i < h.np_; i++)
      os << h.step_ * i << '\t' << h.data_[i] << endl;
   return os;
}

ostream& operator <<(ostream& os, const vector<mcHistogramSampler>& ha)
{
   unsigned int i, j, nh = static_cast<unsigned>(ha.size());
   if (!nh)
      return os; // skip empty
   unsigned int np = ha[0].np_;
   if (!np)
      return os; // skip empty
   os << "nhistograms = " << '\t' << nh << '\t' << "np =" << '\t' << np << endl;
   for (j = 0; j < np; j++) {
      os << ha[0].step_ * j;
      for (i = 0; i < nh; i++) {
         if (ha[i].np_ != np)
            throw std::exception("несогласованные размеры гистограмм");
         os << '\t' << ha[i].data_[j];
      }
      os << endl;
   }
   return os;
}





