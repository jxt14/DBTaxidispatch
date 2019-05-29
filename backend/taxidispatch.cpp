#include "taxidispatch.h"

using namespace std;

int lowbit(int x)
{
    return x & (-x);
}

double HalfSin(double x)
{
    double v;
    v = sin(x/2);
    return v*v;
}

double DegreeToRadiance(double x)
{
    return x * M_PI / 180;
}

TaxiDispatch::TaxiDispatch()
{
    carnum = 0;
    rdnum = 0;
    dcar.clear();
    rd.clear();
    bt[1] = 1;
    bt[2] = 2;
    bt[4] = 3;
    bt[8] = 4;
    g.clear();
    ednext.clear();
    y.clear();
    dis.clear();
    ednum = 0;
}

double TaxiDispatch::StringToDouble(string s)
{
    int len;
    double t,d;
    d = 1;
    t = 0;
    bool p;
    p = false;
    len = s.length();
    for (int i = 0; i < len; i++){
        if (s[i] == '.') p = true;
        else {
            if (p == false) {
                t = t * 10 + (s[i] - '0');
            }
            else {
                d = d * 0.1;
                t = t + d * (s[i] - '0');
            }
        }
    }
    return t;
}

Position TaxiDispatch::Preprocess(string s)
{
    Position nc;
    string sb;
    int pos;
    double tt;

    pos = s.find(',');
    sb = s.substr(0, pos);
    nc.lon = StringToDouble(sb);
    s = s.substr(pos + 1, s.length() - pos - 1);

    pos = s.find(',');
    sb = s.substr(0, pos);
    nc.lat = StringToDouble(sb);
    s = s.substr(pos + 1, s.length() - pos - 1);

    tt = StringToDouble(s);
    nc.num = (int)tt;

    return nc;
}

double TaxiDispatch::dist(Position d1, Position d2)
{
    double lon1,lat1,lon2,lat2;
    lon1 = DegreeToRadiance(d1.lon);
    lat1 = DegreeToRadiance(d1.lat);
    lon2 = DegreeToRadiance(d2.lon);
    lat2 = DegreeToRadiance(d2.lat);
    double dlat,dlon;
    dlat = fabs(lat1 - lat2);
    dlon = fabs(lon1 - lon2);
    double h;
    h = HalfSin(dlat) + cos(lat1)*cos(lat2)*HalfSin(dlon);
    double dis;
    dis = 2 * 6378.137 * asin(sqrt(h));
    return dis;
}

int TaxiDispatch::travelnet(Car tc)
{
    int f[16][5];
    int k,kd,lim,limt,t;
    int ad;
    lim = tc.cusnum;
    limt = (1<<lim) - 1;
    for (int i = 0; i < (1<<lim); i++)
        for (int j = 1; j <= lim; j++)f[i][j] = -1;
    for (int i = 1; i <= lim; i++)f[limt ^ (1<<(i-1))][i] = tc.dsef[i];
    for (int i = limt - 1; i > 0; i--)
        for (int j = 1; j <= lim; j++)if(f[i][j] > 0){
            t = i;
            while (t != 0) {
                k = lowbit(t);
                kd = bt[k];
                ad = tc.dc[j][kd];
                if (f[i^k][kd] < 0 || f[i][j] + ad < f[i^k][kd])
                    f[i^k][kd] = f[i][j] + ad;
                t = t - k;
            }
        }
    int mn = -1;
    for (int i = 1; i <= tc.cusnum; i++){
        if (mn < 0 || f[0][i] < mn)
            mn = f[0][i];
    }
    return mn;
}

double TaxiDispatch::travelgeo(Car tc)
{
    double f[16][5];
    int k,kd,lim,limt,t;
    double ad;
    lim = tc.cusnum;
    limt = (1<<lim) - 1;
    for (int i = 0; i < (1<<lim); i++)
        for (int j = 1; j <= lim; j++)f[i][j] = -1;
    for (int i = 1; i <= lim; i++)f[limt ^ (1<<(i-1))][i] = dist(tc.customer[i], tc.sef);
    for (int i = limt - 1; i > 0; i--)
        for (int j = 1; j <= lim; j++)if(f[i][j] > 0){
            t = i;
            while (t != 0) {
                k = lowbit(t);
                kd = bt[k];
                ad = dist(tc.customer[j], tc.customer[kd]);
                if (f[i^k][kd] < 0 || f[i][j] + ad < f[i^k][kd])
                    f[i^k][kd] = f[i][j] + ad;
                t = t - k;
            }
        }
    double mn = -1;
    for (int i = 1; i <= tc.cusnum; i++){
        if (mn < 0 || f[0][i] < mn)
            mn = f[0][i];
    }
    return mn;
}

void TaxiDispatch::querynet(int st, int dest, vector<int>& res)
{
    res.clear();

    int d1,d2,d3,d4;
    dij(st, ds);
    dij(dest, dt);

    d4 = ds[dest];
    for (int i = 0; i < carnum; i++){
        Car nc = dcar[i];
        d2 = ds[nc.sef.num];
        d1 = nc.tnet;
        if (d2 > 10000) continue;
        if (nc.cusnum == 4) continue;
        nc.cusnum++;
        nc.customer[nc.cusnum] = rd[dest];
        for (int j = 1; j <= nc.cusnum; j++)nc.dsef[j] = ds[nc.customer[j].num];
        for (int j = 1; j < nc.cusnum; j++){
            nc.dc[j][nc.cusnum] = dt[nc.customer[j].num];
            nc.dc[nc.cusnum][j] = dt[nc.customer[j].num];
        }
        nc.dc[nc.cusnum][nc.cusnum] = 0;
        d3 = travelnet(nc);
        if (d3 - d4 > 10000) continue;
        if (d2 + d3 - d1 > 10000) continue;
        res.push_back(i);
        if (res.size() == 5) break;
    }
}

void TaxiDispatch::querygeo(int st, int dest, vector<int>& res)
{
    res.clear();
    double d1,d2,d3,d4;
    Position stt,ed,ys;
    stt = rd[st];
    ed = rd[dest];
    d4 = dist(stt, ed);
    for (int i = 0; i < carnum; i++) {
        Car nc = dcar[i];
        d2 = dist(nc.sef, stt);
        d1 = nc.tdis;
        if (d2 > 10) continue;
        if (nc.cusnum == 4) continue;
        nc.cusnum++;
        nc.customer[nc.cusnum] = ed;
        nc.sef = stt;
        d3 = travelgeo(nc);
        if (d3 - d4 > 10) continue;
        if (d2 + d3 - d1 > 10)continue;
        res.push_back(i);
        if (res.size() == 5) break;
    }
}

void TaxiDispatch::addedge(int i, int j, int k)
{
    ednum++;
    ednext[ednum] = g[i];
    g[i] = ednum;
    y[ednum] = j;
    dis[ednum] = k;
}

void TaxiDispatch::InitCar()
{
    ifstream fin;
    string s;
    fin.open("../data/car.txt");
    carnum = 100000;
    int k;
    for (int i = 1; i <= carnum; i++){
        Car nc;
        fin >> k;
        nc.id = k;
        fin >> k;
        nc.cusnum = k;
        fin >> s;
        nc.sef = Preprocess(s);
        for (int j = 1; j <= nc.cusnum; j++){
            fin >> s;
            nc.customer[j] = Preprocess(s);
        }
        nc.tdis = travelgeo(nc);
        dcar.push_back(nc);
    }
    fin.close();

    fin.open("../data/cus.txt");
    fin >> carnum;
    assert(carnum == 100000);
    int cusnum,tnet;
    for (int i = 0; i < carnum; i++){
        Car nc = dcar[i];
        fin >> cusnum;
        for (int j = 1; j <= cusnum; j++)fin >> nc.dsef[j];
        for (int j = 1; j <= cusnum; j++)
            for (int k = 1; k <= cusnum; k++)
                fin >> nc.dc[j][k];
        assert(cusnum == nc.cusnum);
        nc.tnet = travelnet(nc);
        dcar[i] = nc;
    }
    fin.close();

    printf("Car Input Finished\n");

}

void TaxiDispatch::dij(int start, vector<int> &res)
{
    for (int i = 0; i < rdnum; i++){
        res[i] = -1;
        p[i] = 0;
    }
    while(!q.empty())q.pop();
    res[start] = 0;
    q.push(make_pair(0,start));
    pi temp;
    int x,k,j;
    while (!q.empty()) {
        temp = q.top();
        q.pop();
        x = temp.second;
        if (p[x] == 1) continue;
        p[x] = 1;
        for (j = g[x]; j != 0; j = ednext[j]) {
            k = y[j];
            if (res[k] == -1 || res[x] + dis[j] < res[k]) {
                res[k] = res[x] + dis[j];
                q.push(make_pair(res[k],k));
            }
        }
    }
}

void TaxiDispatch::spfa(int start, vector<int>& res)
{
    for (int i = 0; i < rdnum; i++){
        res[i] = -1;
        p[i] = 0;
    }
    res[start] = 0;
    p[start] = 1;
    int l,r,x,j,k;
    l = r = 0;
    if (que.size() <= l)que.push_back(0);
    que[l] = start;
    while (l <= r) {
        x = que[l];
        for (j = g[x]; j != 0; j = ednext[j]) {
            k = y[j];
            if (res[k] == -1 || res[x] + dis[j] < res[k]) {
                res[k] = res[x] + dis[j];
                if (p[k] != 1) {
                    r++;
                    if(que.size() <= r) que.push_back(0);
                    que[r] = k;
                    p[k] = 1;
                }
            }
        }
        l++;
        p[x] = 0;
    }
    printf("%d\n",r);
}

void TaxiDispatch::InitRoad()
{
    ifstream fin;
    int tnum;
    double tlat,tlon;
    rdnum = 338024;
    fin.open("../data/road.cnode");
    for (int i = 1; i <= rdnum; i++){
        fin >> tnum >> tlon >> tlat;
        Position np(tlat, tlon, tnum);
        rd.push_back(np);
    }
    fin.close();
    printf("Road Point Input Finished\n");

    fin.open("../data/road.nedge");
    int pnum,edgenum,x,z,q;
    fin >> pnum >> edgenum;
    printf("%d %d\n",pnum, edgenum);
    g.clear();
    ednext.clear();
    y.clear();
    dis.clear();
    que.clear();
    ednum = 0;
    for (int i = 0; i <= pnum; i++){
        g.push_back(0);
        ds.push_back(0);
        dt.push_back(0);
        p.push_back(0);
    }
    for (int i = 0; i <= edgenum*3; i++){
        ednext.push_back(0);
        y.push_back(0);
        dis.push_back(0);
    }
    for (int i = 0; i < edgenum; i++){
        fin >> x >> z >> q;
        addedge(x,z,q);
        addedge(z,x,q);
    }
    fin.close();
    printf("Road Edge Input Finished\n");
}

void TaxiDispatch::Init()
{
    InitCar();
    InitRoad();
}