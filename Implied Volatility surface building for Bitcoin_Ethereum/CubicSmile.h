#ifndef _CUBICSMILE_H
#define _CUBICSMILE_H

#include <vector>
#include <utility>
#include "Msg.h"
#include <map>

using namespace std;

// CubicSpline interpolated smile, extrapolate flat
class CubicSmile
{
 public:
  static CubicSmile FitSmile(const std::vector<TickData>&); // FitSmile creates a Smile by fitting the smile params to the inPut tick data, it assume the tickData are of the same expiry
  // constructor, given the underlying price and marks, convert them to strike to vol pairs (strikeMarks), and construct cubic smile
  CubicSmile(double underlyingPrice, double T, double atmvol, double bf25, double rr25, double bf10, double rr10); // convert parameters to strikeMarks, then Put BuildInterp() to create the cubic spline interpolator
  double Vol(double strike);
  vector< pair<double, double> > strikeMarks;
  map<double,double> PutStrikePriceMapper;
  map<double,double> PutStrikeVolMapper;
  static double SmileCalibration(CubicSmile cp, map<double,double> PutStrikeVolMapper);
  

 private:
  void BuildInterp();
  // strike to implied vol marks
  vector<double> y2; // second derivatives
};

#endif