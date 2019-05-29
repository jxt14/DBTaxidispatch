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
#include <queue>
#include <utility>
#include <assert.h>

typedef std::pair<int, int> pi;

struct Result{
    int carnum;
    int d1,d2,d3,d4;
    Result()
    {
        carnum = 0;
        d1 = d2 = d3 = d4 = 0;
    }
    Result(int carnum, int d1, int d2, int d3, int d4)
    {
        this->carnum = carnum;
        this->d1 = d1;
        this->d2 = d2;
        this->d3 = d3;
        this->d4 = d4;
    }
    void Output()
    {
        printf("Car number %d\n", carnum);
        printf("D1 : %d D2 : %d D3: %d D4 : %d\n", d1, d2, d3, d4);        
    }
};

struct Position{
    double lat,lon;
    int num;//roat network id
    Position(double latt, double lonn, int numm)
    {
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
    int dsef[5];
    int dc[5][5];
    int tnet;
    double tdis;
};

struct TaxiDispatch{
    std::vector<Car> dcar;
    std::vector<Position> rd;
    std::vector<int> g,ednext,y,dis,ds,dt,p,que;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> q;
    int ednum;
    int carnum,rdnum;
    int bt[16];
    TaxiDispatch();
    Position Preprocess(std::string);
    double dist(Position, Position);
    double travelgeo(Car);
    int travelnet(Car);
    double StringToDouble(std::string);
    void querygeo(int, int, std::vector<int>&);
    void querynet(int, int, std::vector<Result>&);
    void Init();
    void InitCar();
    void InitRoad();
    void addedge(int, int, int);
    void spfa(int, std::vector<int>&);
    void dij(int, std::vector<int>&);
};


#endif