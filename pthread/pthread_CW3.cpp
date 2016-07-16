#include "stdafx.h"
#include<fstream>
#include <pthread.h>
#include <iostream>
using namespace std;

pthread_mutex_t	lock[4];	
pthread_cond_t	cond[4];
ofstream		outf[4];


void *thread1(void *arg)
{
	int file=(int )arg;
	pthread_mutex_lock(&lock[0]);
	outf[file]<<1;
	pthread_mutex_unlock(&lock[0]);
	pthread_cond_broadcast(&cond[0]);
	return (void *)0;
}
void *thread2(void *arg)
{
	int file=(int )arg;
	pthread_mutex_lock(&lock[1]);
	outf[file]<<2;
	pthread_mutex_unlock(&lock[1]);
	pthread_cond_broadcast(&cond[1]);
	return (void *)0;
}
void *thread3(void *arg)
{
	int file=(int )arg;
	pthread_mutex_lock(&lock[2]);
	outf[file]<<3;
	pthread_mutex_unlock(&lock[2]);
	pthread_cond_broadcast(&cond[2]);
	return (void *)0;
}
void *thread4(void *arg)
{
	int file=(int )arg;
	pthread_mutex_lock(&lock[3]);
	outf[file]<<4;
	pthread_mutex_unlock(&lock[3]);
	pthread_cond_broadcast(&cond[3]);
	return (void *)0;
}
int main()
{
	int i,n=10;
	cout<<"输入重复次数:";
	cin>>n;
	int now=0;//当前线程0写的文件名(0-3)
    pthread_t id[4];
	outf[0].open("A.txt");
	outf[1].open("B.txt");
	outf[2].open("C.txt");
	outf[3].open("D.txt");
	for(i=0;i<4;i++){
		lock[i]=PTHREAD_MUTEX_INITIALIZER;
		cond[i]=PTHREAD_COND_INITIALIZER;
	}
	
		/*pthread_create(&id[3],NULL,thread4,(void *)((3+now)%4));		
		pthread_create(&id[2],NULL,thread3,(void *)((2+now)%4));
		pthread_create(&id[1],NULL,thread2,(void *)((1+now)%4));
		pthread_create(&id[0],NULL,thread1,(void *)((1+now)%4));
		now=4;*/
	for(i=0;i<4*n+1;i++){
		//cout<<pthread_mutex_trylock(&lock[3]);//若不为0则未解锁
		
		if(pthread_mutex_trylock(&lock[3])!=0)pthread_cond_wait(&cond[(3)],&lock[3]);
		//pthread_join(id[3],NULL);
		pthread_create(&id[3],NULL,thread4,(void *)((3+now)%4));		
		
		
		if(pthread_mutex_trylock(&lock[2])!=0)pthread_cond_wait(&cond[(2)],&lock[2]);	
		//pthread_join(id[2],NULL);		
		pthread_create(&id[2],NULL,thread3,(void *)((2+now)%4));
		
		
		if(pthread_mutex_trylock(&lock[1])!=0)pthread_cond_wait(&cond[(1)],&lock[1]);	
		//pthread_join(id[1],NULL);		
		pthread_create(&id[1],NULL,thread2,(void *)((1+now)%4));
		
		
		if(pthread_mutex_trylock(&lock[0])!=0)pthread_cond_wait(&cond[(0)],&lock[0]);	
		//pthread_join(id[0],NULL);		
		pthread_create(&id[0],NULL,thread1,(void *)((0+now)%4));
		now=(now+3)%4;//下一次的线程1所写的文件名
		if(i%4==0)for(int j=0;j<4;j++)outf[j]<<endl;//-------用于方便查看文件		
	}
	for(i=0;i<4;i++)outf[i].close();
	cout<<"complete!"<<endl;
	system("pause");
	return 0;
}