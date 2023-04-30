#include "CubicSmile.h"
#include "BSAnalytics.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <unordered_map>
#include "Date.h"
#include <Eigen/Core>
#include <LBFGSB.h>

// Rosenbrock
using Eigen::VectorXd;
using namespace LBFGSpp;
typedef double Scalar;
typedef Eigen::Matrix<Scalar, Eigen::Dynamic, 1> Vector;

std::map<std::string, int> months = {
    { "JAN", 1 },
    { "FEB", 2 },
    { "MAR", 3 },
    { "APR", 4 },
    { "MAY", 5 },
    { "JUN", 6 },
    { "JUL", 7 },
    { "AUG", 8 },
    { "SEP", 9 },
    { "OCT", 10 },
    { "NOV", 11 },
    { "DEC", 12 }
};
CubicSmile CubicSmile::FitSmile(const std::vector<TickData>& volTickerSnap) {
    double fwd, T, atmvol, bf25, rr25, bf10, rr10;
    datetime_t latestTimeStamp = datetime_t(0);
    // map strikes to vol
    map<double,double> PutStrikePriceMapper;
    map<double,double> PutStrikeVolMapper;
    TickData LatestTick;
    for (auto itr = volTickerSnap.begin(); itr != volTickerSnap.end(); itr++){
      TickData sameExpiryTick = *itr;
      if (latestTimeStamp - sameExpiryTick.LastUpdateTimeStamp < 0){
        latestTimeStamp = sameExpiryTick.LastUpdateTimeStamp;
        LatestTick = *itr;
      }
    }
    datetime_t maturityDate;
    // get fwd
    fwd = LatestTick.UnderlyingPrice;
    // get T
    std::stringstream eachLine(LatestTick.ContractName);
    std::string ss_item;
    int line_counter = 1;
    while (getline(eachLine, ss_item, '-')){
      line_counter ++;
      if (line_counter == 3){
        if (ss_item.length()==7){
            maturityDate.day = std::stoi(ss_item.substr(0,2));
            maturityDate.month = months.at(ss_item.substr(2,3));
            maturityDate.year = std::stoi("20"+ss_item.substr(5,2));
        }else{
            maturityDate.day = std::stoi(ss_item.substr(0,1));
            maturityDate.month = months.at(ss_item.substr(1,3));
            maturityDate.year = std::stoi("20"+ss_item.substr(4,2));
        }
      }
    }

    // add another day, assuming end of day settlement
    T = maturityDate - latestTimeStamp +1.0/365;
    // std::cout<<T<<std::endl;
    // Create function to calculate the 4 strikes using the qdtoStrike function. Only parameter unknown is atmvol. Return 4 strikes
    // Calculate ATM Vol and generate strike,vol map

    for (auto itrMap = volTickerSnap.begin(); itrMap != volTickerSnap.end(); itrMap++){
      TickData sameExpiryTick = *itrMap;
      int counter = 0;
      std::stringstream eachLine(sameExpiryTick.ContractName);
      std::string ss_item;
      double strike, price, midiv, underlyingprice;

      while (getline(eachLine, ss_item, '-')){
        counter++;
        
        if (counter == 3) {
          strike = std::stod(ss_item);
        }}
        price = sameExpiryTick.MarkPrice;
        midiv = (sameExpiryTick.BestBidIV + sameExpiryTick.BestAskIV) / 2 / 100;
        auto tempPair = std::pair<double,double>(strike,price);
        auto tempPair2 = std::pair<double,double>(strike,midiv);
        auto tempPair3 = std::pair<double,double>(strike,fwd);
        if (counter == 4){
          if (ss_item=="P" && tempPair.second != 0) {
            PutStrikePriceMapper.insert(tempPair);
            // add to PutStrikeVolmapper only if midiv != 0
            if (tempPair2.second != 0)
            {
              PutStrikeVolMapper.insert(tempPair2);
            }         
          }
        }
      }
    // TODO: fit a CubicSmile that is close to the raw tickers
    // - make sure all tickData are on the same expiry and same underlying - done
    // - get latest underlying price from all tickers based on LastUpdateTimeStamp - done
    // - get time to expiry T -done
    // - fit the 5 parameters of the smile, atmvol, bf25, rr25, bf10, and rr10 using L-BFGS-B solver, to the ticker data - doing

    // Our own minimizing function due to lack of time
    double err=100;
    double optimum_atmvol;
    int n=250;
    double FinalFunction(double atmvol, double fwd, double T,std::map<double,double> PutStrikePriceMapper,std::map<double,double> PutStrikeVolMapper);
    for (int i = 1; i<n;i++){
      double j = i/100.0;
      if (err >= FinalFunction(j,T,fwd,PutStrikePriceMapper,PutStrikeVolMapper)){
        err = FinalFunction(j,T,fwd, PutStrikePriceMapper, PutStrikeVolMapper);
        optimum_atmvol=j;
      }
    }
    // void LBFGS(double T, double fwd,std::map<double,double> PutStrikePriceMapper,std::map<double,double> PutStrikePriceMapper,std::map<double,double> PutStrikeVolMapper,std::map<double,double> PutStrikeVolMapper);
    // LBFGS(T,fwd, PutStrikePriceMapper, PutStrikePriceMapper,PutStrikeVolMapper, PutStrikeVolMapper);

    // after the fitting, we can return the resulting smile

    double strikeLower_90, price_90, undiscPrice_90 = 0.0;
    double strikeLower_75, price_75, undiscPrice_75 = 0.0;
    double strikeLower_25, price_25, undiscPrice_25 = 0.0;
    double strikeLower_10, price_10, undiscPrice_10 = 0.0;
    double stdev = optimum_atmvol * sqrt(T);

    double k_qd90 = quickDeltaToStrike(0.9, fwd, stdev);
    double k_qd75 = quickDeltaToStrike(0.75, fwd, stdev);
    double k_qd25 = quickDeltaToStrike(0.25, fwd, stdev);
    double k_qd10 = quickDeltaToStrike(0.1, fwd, stdev);
    
    for (auto itrMap = PutStrikePriceMapper.begin(); itrMap != PutStrikePriceMapper.end(); itrMap++){
      if (itrMap == PutStrikePriceMapper.begin())
      {
        strikeLower_90 = itrMap->first;
        strikeLower_75 = itrMap->first;
        strikeLower_25 = itrMap->first;
        strikeLower_10 = itrMap->first;
        price_90 = itrMap->second;
        price_75 = itrMap->second;
        price_25 = itrMap->second;
        price_10 = itrMap->second;
        continue; 
      }
      if (k_qd25 >= strikeLower_25 && k_qd25 <= itrMap->first)
      {
        undiscPrice_25 = (price_25 + ((k_qd25 - strikeLower_25) / (itrMap->first - strikeLower_25)) * (itrMap->second - price_25)) * fwd;
      }
      if (k_qd10 >= strikeLower_10 && k_qd10 <= itrMap->first)
      {
        undiscPrice_10 = (price_10 + ((k_qd10 - strikeLower_10) / (itrMap->first - strikeLower_10)) * (itrMap->second - price_10)) * fwd;
      }
      if (k_qd90 >= strikeLower_90 && k_qd90 <= itrMap->first)
      {
        undiscPrice_90 = (price_90 + ((k_qd90 - strikeLower_90) / (itrMap->first - strikeLower_90)) * (itrMap->second - price_90)) * fwd ;
      }
      if (k_qd75 >= strikeLower_75 && k_qd75 <= itrMap->first)
      {
        undiscPrice_75 = (price_75 + ((k_qd75 - strikeLower_75) / (itrMap->first - strikeLower_75)) * (itrMap->second - price_75)) * fwd;
      }
      strikeLower_90 = itrMap->first;
      strikeLower_75 = itrMap->first;
      strikeLower_25 = itrMap->first;
      strikeLower_10 = itrMap->first;
      price_90 = itrMap->second;
      price_75 = itrMap->second;
      price_25 = itrMap->second;
      price_10 = itrMap->second;
  }
    
  // using interpolated market observed price to find model impliedVol
  double sigma_90 = impliedVol(Put, k_qd90, fwd, T, undiscPrice_90);
  double sigma_75 = impliedVol(Put, k_qd75, fwd, T, undiscPrice_75);
  double sigma_25 = impliedVol(Put, k_qd25, fwd, T, undiscPrice_25);
  double sigma_10 = impliedVol(Put, k_qd10, fwd, T, undiscPrice_10);

  bf25 = (sigma_75+sigma_25)/2 - optimum_atmvol;
  rr25 = sigma_25-sigma_75;
  bf10 = (sigma_90+sigma_10)/2 - optimum_atmvol;
  rr10 = sigma_10-sigma_90;

  // std::cout << "F = " << fwd << " T = "<< T << " IV = " << optimum_atmvol << " bf25 = " << bf25 << " rr25 = " << rr25 << " bf10 = " << bf10 << " rr10 = " << rr10 << std::endl; 

  return CubicSmile(fwd, T, optimum_atmvol, bf25, rr25, bf10, rr10);
}

CubicSmile::CubicSmile(double underlyingPrice, double T, double atmvol, double bf25, double rr25, double bf10, double rr10) {
    // convert delta marks to strike vol marks, setup strikeMarks, then Put BUildInterp
    double v_qd90 = atmvol + bf10 - rr10 / 2.0;
    double v_qd75 = atmvol + bf25 - rr25 / 2.0;
    double v_qd25 = atmvol + bf25 + rr25 / 2.0;
    double v_qd10 = atmvol + bf10 + rr10 / 2.0;

    // we use quick delta: qd = N(log(F/K / (atmvol) / sqrt(T))
    double stdev = atmvol * sqrt(T);
    double k_qd90 = quickDeltaToStrike(0.9, underlyingPrice, stdev);
    double k_qd75 = quickDeltaToStrike(0.75, underlyingPrice, stdev);
    double k_qd25 = quickDeltaToStrike(0.25, underlyingPrice, stdev);
    double k_qd10 = quickDeltaToStrike(0.1, underlyingPrice, stdev);

    strikeMarks.push_back(std::pair<double, double>(k_qd90, v_qd90));
    strikeMarks.push_back(std::pair<double, double>(k_qd75, v_qd75));
    strikeMarks.push_back(std::pair<double, double>(underlyingPrice, atmvol));
    strikeMarks.push_back(std::pair<double, double>(k_qd25, v_qd25));
    strikeMarks.push_back(std::pair<double, double>(k_qd10, v_qd10));
    BuildInterp();
}

void CubicSmile::BuildInterp()
{
  int n = strikeMarks.size();
  // end y' are zero, flat extrapolation
  double yp1 = 0;
  double ypn = 0;
  y2.resize(n);
  vector<double> u(n-1);

  y2[0] = -0.5;
  u[0]=(3.0/(strikeMarks[1].first-strikeMarks[0].first)) *
    ((strikeMarks[1].second-strikeMarks[0].second) / (strikeMarks[1].first-strikeMarks[0].first) - yp1);

  for(int i = 1; i < n-1; i++) {
    double sig=(strikeMarks[i].first-strikeMarks[i-1].first)/(strikeMarks[i+1].first-strikeMarks[i-1].first);
    double p=sig*y2[i-1]+2.0;
    y2[i]=(sig-1.0)/p;
    u[i]=(strikeMarks[i+1].second-strikeMarks[i].second)/(strikeMarks[i+1].first-strikeMarks[i].first)
      - (strikeMarks[i].second-strikeMarks[i-1].second)/(strikeMarks[i].first-strikeMarks[i-1].first);
    u[i]=(6.0*u[i]/(strikeMarks[i+1].first-strikeMarks[i-1].first)-sig*u[i-1])/p;
  }

  double qn=0.5;
  double un=(3.0/(strikeMarks[n-1].first-strikeMarks[n-2].first)) *
    (ypn-(strikeMarks[n-1].second-strikeMarks[n-2].second)/(strikeMarks[n-1].first-strikeMarks[n-2].first));

  y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0);

//  std::cout << "y2[" << n-1 << "] = " << y2[n-1] << std::endl;
  for (int i=n-2;i>=0;i--) {
    y2[i]=y2[i]*y2[i+1]+u[i];
//    std::cout << "y2[" << i << "] = " << y2[i] << std::endl;
  }
}

double CubicSmile::Vol(double strike) {
    unsigned i; 
    // we use trivial search, but can consider binary search for better performance
    for (i = 0; i < strikeMarks.size(); i++ )
      if (strike < strikeMarks[i].first )
        break; // i stores the index of the right end of the bracket

    // extrapolation
    if (i == 0)
      return strikeMarks[i].second;
    if (i == strikeMarks.size() )
      return strikeMarks[i-1].second;

    // interpolate
    double h = strikeMarks[i].first - strikeMarks[i-1].first;
    double a = (strikeMarks[i].first - strike) / h;
    double b = 1 - a;
    double c = (a*a*a - a) * h * h / 6.0;
    double d = (b*b*b - b) * h * h / 6.0;
    return a*strikeMarks[i-1].second + b*strikeMarks[i].second + c*y2[i-1] + d*y2[i];
  }; // interpolate

double SmileCalibration(CubicSmile cp, map<double,double> PutStrikeVolMapper){
  // last step: return error
  double err = 0.0;
  double strikeMarket, midvolMarket, modelVol = 0.0;
  double strike;
  
  
  for (auto itrMap = PutStrikeVolMapper.begin(); itrMap != PutStrikeVolMapper.end(); itrMap++)
  {
    strikeMarket = itrMap->first;
    midvolMarket = itrMap->second;
    modelVol = cp.Vol(strikeMarket);
    err += (1.0/size(PutStrikeVolMapper)) * std::pow((modelVol - midvolMarket), 2);
    continue; 
  }
  return err;
}

double FinalFunction(double atmvol, double fwd, double T,std::map<double,double> PutStrikePriceMapper,std::map<double,double> PutStrikeVolMapper)
{
    double strike_90, strike_75, strike_25, strike_10;
    double stdev = atmvol * sqrt(T);

    // model strikes using quick delta (ITM ATM OTM Puts)
    double k_qd90 = quickDeltaToStrike(0.9, fwd, stdev);
    double k_qd75 = quickDeltaToStrike(0.75, fwd, stdev);
    double k_qd25 = quickDeltaToStrike(0.25, fwd, stdev);
    double k_qd10 = quickDeltaToStrike(0.1, fwd, stdev);

    // interpolate observed market prices
    double strikeLower_90, price_90, undiscPrice_90 = 0.0;
    double strikeLower_75, price_75, undiscPrice_75 = 0.0;
    double strikeLower_25, price_25, undiscPrice_25 = 0.0;
    double strikeLower_10, price_10, undiscPrice_10 = 0.0;

    // for market implied vol
    std::unordered_map<double, double> marketSmile;

    // interpolate market observed price to find market price of quickDeltaStrikes, a parameter for impliedVol
    for (auto itrMap = PutStrikePriceMapper.begin(); itrMap != PutStrikePriceMapper.end(); itrMap++){
      if (itrMap == PutStrikePriceMapper.begin())
      {
        strikeLower_90 = itrMap->first;
        strikeLower_75 = itrMap->first;
        strikeLower_25 = itrMap->first;
        strikeLower_10 = itrMap->first;
        price_90 = itrMap->second;
        price_75 = itrMap->second;
        price_25 = itrMap->second;
        price_10 = itrMap->second;
        continue; 
      }
      if (k_qd25 >= strikeLower_25 && k_qd25 <= itrMap->first)
      {
        undiscPrice_25 = (price_25 + ((k_qd25 - strikeLower_25) / (itrMap->first - strikeLower_25)) * (itrMap->second - price_25)) * fwd;
      }
      if (k_qd10 >= strikeLower_10 && k_qd10 <= itrMap->first)
      {
        undiscPrice_10 = (price_10 + ((k_qd10 - strikeLower_10) / (itrMap->first - strikeLower_10)) * (itrMap->second - price_10)) * fwd;
      }
      if (k_qd90 >= strikeLower_90 && k_qd90 <= itrMap->first)
      {
        undiscPrice_90 = (price_90 + ((k_qd90 - strikeLower_90) / (itrMap->first - strikeLower_90)) * (itrMap->second - price_90)) * fwd ;
      }
      if (k_qd75 >= strikeLower_75 && k_qd75 <= itrMap->first)
      {
        undiscPrice_75 = (price_75 + ((k_qd75 - strikeLower_75) / (itrMap->first - strikeLower_75)) * (itrMap->second - price_75)) * fwd;
      }
      strikeLower_90 = itrMap->first;
      strikeLower_75 = itrMap->first;
      strikeLower_25 = itrMap->first;
      strikeLower_10 = itrMap->first;
      price_90 = itrMap->second;
      price_75 = itrMap->second;
      price_25 = itrMap->second;
      price_10 = itrMap->second;
  }
  // using interpolated market observed price to find model impliedVol
  double sigma_90 = impliedVol(Put, k_qd90, fwd, T, undiscPrice_90);
  double sigma_75 = impliedVol(Put, k_qd75, fwd, T, undiscPrice_75);
  double sigma_25 = impliedVol(Put, k_qd25, fwd, T, undiscPrice_25);
  double sigma_10 = impliedVol(Put, k_qd10, fwd, T, undiscPrice_10);

  double bf25 = (sigma_75+sigma_25)/2 - atmvol;
  double rr25 = sigma_25-sigma_75;
  double bf10 = (sigma_90+sigma_10)/2 - atmvol;
  double rr10 = sigma_10-sigma_90;
  
  // std::cout << "F = " << fwd << " T = "<< T << " IV = " << atmvol << " bf25 = " << bf25 << " rr25 = " << rr25 << " bf10 = " << bf10 << " rr10 = " << rr10 << std::endl; 
  // return rr,bf values
  auto cp = CubicSmile(fwd, T, atmvol, bf25, rr25, bf10, rr10);

  // last step: return error
  double err = 0.0;
  double strikeMarket, midvolMarket, modelVol = 0.0;
  double strike;
  
  for (auto itrMap = PutStrikeVolMapper.begin(); itrMap != PutStrikeVolMapper.end(); itrMap++)
  {
    strikeMarket = itrMap->first;
    midvolMarket = itrMap->second;
    modelVol = cp.Vol(strikeMarket);
    err += (1.0/size(PutStrikeVolMapper)) * std::pow((modelVol - midvolMarket), 2);
    continue; 
  }
  return err;
};

// // Example from the roptim R package
// // f(x) = (x[0] - 1)^2 + 4 * (x[1] - x[0]^2)^2 + ... + 4 * (x[end] - x[end - 1]^2)^2
// class Rosenbrock
// {
// private:
//     int n;
//     double fwd;
//     double T;
//     std::map<double,double> PutStrikePriceMapper;
//     std::map<double,double> PutStrikePriceMapper;
//     std::map<double,double> PutStrikeVolMapper;
//     std::map<double,double> PutStrikeVolMapper;
// public:
//     Rosenbrock(int n_, double fwd, double T, std::map<double,double> PutStrikePriceMapper,std::map<double,double> PutStrikePriceMapper,std::map<double,double> PutStrikeVolMapper,std::map<double,double> PutStrikeVolMapper) : n(n_) {}
//     Scalar operator()(const Vector& x, Vector& grad)
//     {
//         Scalar fx = (x[0] - 1.0) * (x[0] - 1.0);
//         grad[0] = 2 * (x[0] - 1) + 16 * (x[0] * x[0] - x[1]) * x[0];
//         for(int i = 1; i < n; i++)
//         {
//             // fx += 4 * std::pow(x[i] - x[i - 1] * x[i - 1], 2);
//             fx += CubicSmile::FinalFunction(x[i], fwd, T, PutStrikePriceMapper, PutStrikePriceMapper,PutStrikeVolMapper,PutStrikeVolMapper);
//             if(i == n - 1)
//             {
//                 grad[i] = 8 * (x[i] - x[i - 1] * x[i - 1]);
//             } else {
//                 grad[i] = 8 * (x[i] - x[i - 1] * x[i - 1]) + 16 * (x[i] * x[i] - x[i + 1]) * x[i];
//             }
//         }
//         return fx;
//     }
// };

// // minimizer
// void LBFGS(double T, double fwd, std::map<double,double> PutStrikePriceMapper,std::map<double,double> PutStrikePriceMapper,std::map<double,double> PutStrikeVolMapper,std::map<double,double> PutStrikeVolMapper)
// {
//     const int n = 25;
//     LBFGSBParam<Scalar> param;
//     LBFGSBSolver<Scalar> solver(param);
//     Rosenbrock fun(n, fwd, T, PutStrikePriceMapper,PutStrikePriceMapper,PutStrikeVolMapper, PutStrikeVolMapper);

//     // Variable bounds 
//     Vector lb = Vector::Constant(n, 0.0);
//     Vector ub = Vector::Constant(n, 10.0);
//     // The third variable is unbounded
//     lb[2] = -std::numeric_limits<Scalar>::infinity();
//     ub[2] = std::numeric_limits<Scalar>::infinity();
//     // Initial values
//     Vector x = Vector::Constant(n, 0.5);
//     // Make some initial values at the bounds
//     x[0] = x[1] = 0.0;
//     x[5] = x[7] = 10.0;

//     Scalar fx;
//     int niter = solver.minimize(fun, x, fx, lb, ub);

//     std::cout << niter << " iterations" << std::endl;
//     std::cout << "x = \n" << x.transpose() << std::endl;
//     std::cout << "f(x) = " << fx << std::endl;
// }