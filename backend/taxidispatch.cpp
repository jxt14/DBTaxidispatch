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

double TaxiDispatch::travel(Car tc)
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

void TaxiDispatch::query(int st, int dest, vector<int>& res)
{
    res.clear();
    double d1,d2,d3,d4;
    Position stt,ed,ys;
    stt = rd[st];
    ed = rd[dest];
    d4 = dist(stt, ed);
    for (int i = 0; i < carnum; i++) {
        d2 = dist(dcar[i].sef, stt);
        d1 = dcar[i].tdis;
        if (d2 > 10) continue;
        if (dcar[i].cusnum == 4) continue;
        ys = dcar[i].sef;
        dcar[i].cusnum++;
        dcar[i].customer[dcar[i].cusnum] = ed;
        dcar[i].sef = stt;
        d3 = travel(dcar[i]);
        dcar[i].sef = ys;
        dcar[i].cusnum--;
        if (d3 - d4 > 10) continue;
        if (d2 + d3 - d1 > 10)continue;
        res.push_back(i);
        if (res.size() == 5) break;
    }
}

void TaxiDispatch::Init()
{
    ifstream fin;
    string s,sb;
    fin.open("../data/car.txt");
    carnum = 100000;
    rdnum = 338024;
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
        nc.tdis = travel(nc);
        dcar.push_back(nc);
    }
    fin.close();
    printf("Car Input Finished\n");

    int tnum;
    double tlat,tlon;
    fin.open("../data/road.cnode");
    for (int i = 1; i <= rdnum; i++){
        fin >> tnum >> tlon >> tlat;
        Position np(tlat, tlon, tnum);
        rd.push_back(np);
    }
    fin.close();
    printf("Road Input Finished\n");
}
 