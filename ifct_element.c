//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};


typedef struct ifs_ele {
	int index;//번호
	int age;//나이
	int detected_time;//감염 시점
	place_t history_place[N_HISTORY];//감염 직전 이동경로 place_t 배열 (N_HISTORY) 
}ifs_ele_t;


void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]){
	ifs_ele_t*ptr;
	
	ptr = (ifs_ele_t*)malloc(sizeof(ifs_ele_t));//하나 잡기 5시 17분 

	ptr->index = index;
	ptr->age = age;
	ptr->time = detected_time;
	ptr->place = history_place;
	
	//free 넣으면 안됨 
	return ptr;
}

int ifctele_getAge(void* obj){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	
	return (strPtr->age);
};

int ifctele_getHistPlaceIndex(void* obj, int index);
unsigned int ifctele_getinfestedTime(void* obj);

void ifsele_printElement(void*obj){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	
	printf("number : %i\n", strPtr->index);
	printf("age : %i\n", strPtr->age);
	printf("time : %i\n", strPtr->detected_time);
	printf("place : %s\n", strPtr->history_place);
	//print elements
};













