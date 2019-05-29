#include "taxidispatch.h"

using namespace std;

int main()
{
    TaxiDispatch ths;
    ths.Init();

    vector<int> rs;
    int sts,edd;
    scanf("%d %d", &sts, &edd);

    ths.querynet(sts,edd,rs);
    for (int i = 0; i < rs.size(); i++)printf("%d ", rs[i]);
    printf("\n");
}