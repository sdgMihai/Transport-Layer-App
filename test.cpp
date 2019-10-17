// Example program
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <map>
#include <sstream>
#include <arpa/inet.h>

using namespace std;
int main()
{   
    /*map<int, vector<int>> mymap;
    mymap[1].push_back(3);
    cout << mymap[1][0];
    stringstream ss("ana are 1");
    string s;
    ss >> s;
    cout << s;
    ss >> s;
    cout << ' '  << s;
    string nr;
    ss >> nr;
    cout << ' ' << atoi(nr.c_str());*/
    /*uint32_t a[2] = {htonl(10), 0};
    char* p = (char*)&a;
    string s(p);
    a[1] = ntohl(atoi(s.c_str()));
    cout << a[1];*/

    char str[10];
    double x;
    uint32_t  unsi;
    sprintf(str, "%u", 3);
    sscanf(str, "%u", &unsi);
    printf("%u", unsi);
}
/*
subscribe a_non_negative_int 1
unsubscribe a_non_negative_int
*/