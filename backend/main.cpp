#include "taxidispatch.h"

using namespace std;

int main()
{
    TaxiDispatch ths;
    ths.Init();

    vector<Result> rs;
    int sts,edd;
    scanf("%d %d", &sts, &edd);

    ths.querynet(sts,edd,rs);
    if (rs.size() == 0) printf("Can not find any car\n");
    else {
        for (int i = 0; i < rs.size(); i++)rs[i].Output();
    }
    printf("\n");
}