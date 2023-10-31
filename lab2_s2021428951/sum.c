#include<stdio.h>
int main(){
	int res=0;
	for(int i=1;i<=500;i++){
		res+=i;
	}
	printf("The sum of 1 to 500 is <%d>.",res);
}
