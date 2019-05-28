#ifndef TX
#define TX

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>

struct Position{
    double lat,lon;
    int num;//roat network id
    Position(double latt, double lonn, int numm){
        lat = latt;
        lon = lonn;
        num = numm;
    }
    Position()
    {
        lat = 0;
        lon = 0;
        num = 0;
    }
};

struct Car{
    Position sef;
    int id;
    Position customer[5];
    int cusnum;
    double tdis;
};

struct TaxiDispatch{
    std::vector<Car> dcar;
    std::vector<Position> rd;
    int carnum,rdnum;
    int bt[16];
    TaxiDispatch();
    Position Preprocess(std::string s);
    double dist(Position d1, Position d2);
    double travel(Car tc);
    double StringToDouble(std::string s);
    void query(int st, int dest, std::vector<int>& res);
    void Init();
};


#endif