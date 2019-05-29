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
    void querynet(int, int, std::vector<int>&);
    void Init();
    void InitCar();
    void InitRoad();
    void addedge(int, int, int);
    void spfa(int, std::vector<int>&);
    void dij(int, std::vector<int>&);
};


#endif