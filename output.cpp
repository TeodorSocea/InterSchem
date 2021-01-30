#include<iostream>
#include<cmath>
using namespace std;
float a = 0, b = 0, c = 0, delta = 0;
int main(){
	cin>>a;
	cin>>b;
	cin>>c;
	delta = b^2 - 4*a*c;
	if(delta == 0){
		cout<<(0-b)/2*a;
	}
	else{
		if(delta > 0){
			cout<<((0-b)+sqrt(delta))/2*a;
			cout<<((0-b)-sqrt(delta))/2*a;
		}
		else{
			cout<<"nu exista";
		}
	}
return 0;
}