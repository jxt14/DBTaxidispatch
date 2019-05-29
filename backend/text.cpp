#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>

using namespace std;

int main()
{
    ofstream fout;
    fout.open("rr.txt");
    fout << 33 << endl;
    fout.close();
}