#include <iostream>
#include <list>

using namespace std;

int main(){
	list<int> lista;
	lista.push_front(1);
	lista.push_front(2);
	lista.push_front(3);
	lista.pop_back();
	cout<<lista.back();
	return 0;
}
