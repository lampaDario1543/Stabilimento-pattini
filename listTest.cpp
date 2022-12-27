#include <iostream>
#include <list>
#include <queue>

using namespace std;

int main(){
	/*list<int> lista;
	lista.push_back(1);
	lista.push_back(2);
	lista.push_back(4);
	lista.push_back(3);
	lista.pop_front();
	cout<<lista.front();*/
	queue <int *> coda;
	int v[]={1,2,3,4};
	int v2[]={5,6};
	int v3[]={7,8,9};
	coda.push(v);
	coda.push(v2);
	coda.push(v3);
	coda.pop();
	cout<<coda.front()[0];
	return 0;
}
