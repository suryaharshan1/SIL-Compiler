#include <iostream>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <stdlib.h>

using namespace std;
void quicksort(int *a,int low,int high);
int partition(int *a,int low,int high);
void swap(int *a,int left,int right);

void quicksort(int *a,int low,int high){
	int pivot;
	if(high>low){
	  pivot=partition(a,low,high);
	  quicksort(a,low,pivot-1);
	  quicksort(a,pivot+1,high);
	}
}

int partition(int *a,int low,int high){
	int left,right,pivot_item=a[low];
	left=low;
	right=high;
	while(left<right){
		while(a[left]<=pivot_item)
		 left++;
		while(a[right]>pivot_item)
		 right--;
		if(left<right)
		  //swap(a,left,right);
		  {
				int temp;
				temp = a[left];
				a[left] = a[right];
				a[right]=temp;
		  }
		  
	}
	a[low]=a[right];
	a[right]=pivot_item;
	return right;
}


main(){

//  int b = rand()%5+1;
//  cout<<b<<"\n";
  clock_t start_c = clock();
  cout<<"Enter the filename :";
  char filename[100];
  cin>>filename;
  fstream fin;
  fin.open(filename);
  long int size;
  fin>>size;

  int a[10000];
  
  for(int i=0;i<size;i++)
   {
	fin>>a[i];
	
   }
   cout<<a[0]<<"\n";
   quicksort(a,0,size);
  fstream fout;
  fout.open("OutputQuickSort.txt",ios::out);
  
  for(int i=1;i<=size;i++){

    cout<<a[i]<<" " ;
    fout<<a[i]<<"\n";
 }
    fin.close();

    clock_t ends_c = clock();
   cout<< "\nRunning time : "<<(double) (ends_c-start_c) / CLOCKS_PER_SEC<<endl;
   fout<< "\nRunning time : "<<(double) (ends_c-start_c) / CLOCKS_PER_SEC<<endl;
   fout.close();

 }

