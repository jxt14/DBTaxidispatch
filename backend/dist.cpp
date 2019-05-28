#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

double EARTH_RADIUS = 6378.137;

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

double distance(double lon1, double lat1, double lon2, double lat2)
{
    lon1 = DegreeToRadiance(lon1);
    lat1 = DegreeToRadiance(lat1);
    lon2 = DegreeToRadiance(lon2);
    lat2 = DegreeToRadiance(lat2);
    double dlat,dlon;
    dlat = fabs(lat1 - lat2);
    dlon = fabs(lon1 - lon2);
    double h;
    h = HalfSin(dlat) + cos(lat1)*cos(lat2)*HalfSin(dlon);
    double dis;
    dis = 2 * EARTH_RADIUS * asin(sqrt(h));
    return dis;
}

int main()
{
    double x1,y1,x2,y2;
    scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
    double dis;
    dis = distance(x1,y1,x2,y2);
    printf("%.4lf\n",dis);
}