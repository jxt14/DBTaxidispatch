#include "taxidispatch.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include <iostream>

using namespace std;

string DoubleToString(double d)
{
    char str[101];
    sprintf(str, "%lf", d);
    string res = str;
    return res;
}

string PosToString(Position rs)
{
    string res = "";
    res = res + DoubleToString(rs.lon) + ',' + DoubleToString(rs.lat);
    return res;
}

int main()
{

    int serversocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if( bind(serversocket, (struct sockaddr*) &server_addr, sizeof(server_addr))==-1){
        perror("bind error");
    }
    else printf("bind succeed\n");
    if(listen(serversocket, 5) == -1) {
        perror("listen");
        exit(1);
    }
    else printf("listen succeed\n");
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    ///成功返回非负描述字，出错返回-1
    int conn = accept(serversocket, (struct sockaddr*)&client_addr, &length);
    if( conn < 0 ) {
        perror("connect");
    }
    else printf("connect succeed\n");

    TaxiDispatch ths;
    ths.Init();

    vector<Result> rs;
    int sts,edd;
    printf("please input starting and destination point\n");
    scanf("%d %d", &sts, &edd);
    string ss;
    ss = PosToString(ths.rd[sts]);
    send(conn, ss.c_str(), ss.length(), 0);

    ths.querynet(sts,edd,rs);
    if (rs.size() == 0) printf("Can not find any car\n");
    else {
        for (int i = 0; i < rs.size(); i++){
            rs[i].Output();
            int num = ths.dcar[rs[i].carnum].sef.num;
            ss = PosToString(ths.rd[num]);
            sleep(1);
            send(conn, ss.c_str(), ss.length(), 0);
        }
    }
    sleep(1);
    ss = "end";
    send(conn, ss.c_str(), ss.length(), 0);
    printf("\n");


}