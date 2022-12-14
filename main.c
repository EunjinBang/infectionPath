//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no);							//전파자를 찾는 함수 
int isMet(int patient_no, int entered_patient);				//전파자와 만난 시점을 reutrn하는 함수 
int convertTimetoIndex(int time, int infested_time);		//시점을 시간 index로 변환하는 함수  

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int index, age, detected_time;
    int history_place[N_HISTORY]; 
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
    //1-2. loading each patient informations
    while(3 == (fscanf(fp,"%i %i %i", &index, &age, &detected_time))) //인덱스, 나이, 시간 읽기 
    {
    	int i;
    	for(i=0;i<5;i++){
			fscanf(fp, "%i", &history_place[i]);					//5가지 위치 읽기 
		}
		
		ifct_element = ifctele_genElement(index, age, detected_time, history_place);
		ifctdb_addTail(ifct_element);		//맨 끝에 데이터 삽입 
	}
	
    //1-3. FILE pointer close
    fclose(fp);
    
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
        	int j;
			int i;
        	int scan_index; 					//입력받은 환자 번호 
    		char scan_place[MAX_PLACENAME]; 	//입력받은 장소 이름							 
   	 		int min_age;						//입력받은 최소 나이
			int max_age; 						//입력받은 최대 나이
			int root_patient;					//최초 전파자를 찾기 위해 입력받은 환자 번호
			int p_index;						//현재 환자 
			int spreader;						//전파자
			int pre_spreader;					//이전 전파자 
			int initial_spreader;				//최초 전파자  
			int met_infectedtime;				//두 환자가 만나서 전염된 시점 
			char* met_place_name;				//두 환자가 만나서 전염된 장소 이름
        	
            case MENU_EXIT:										//0번 선택->프로그램 종료  
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT: 									//1번 선택->지정된 환자의 정보 출력 
            
				printf("Select a patient : "); 					//환자 선택 
				scanf("%i", &scan_index);
				printf("\n");
				
				if (0 <= scan_index && scan_index <= ifctdb_len()-1){	//db안의 환자를 선택하면  
					ifct_element = ifctdb_getData(scan_index);
					ifctele_printElement(ifct_element);			//선택한 환자에 대한 정보 출력 
				}
				else
					printf("[ERROR] You should enter between 0~%i\n", ifctdb_len()-1);		//db 밖의 환자 번호를 입력하면 에러 출력 
				                                                 
                break;
                
            case MENU_PLACE:									//2번 선택->지정된 장소에서 감염이 확인된 환자 출력 
            	
            	printf("Enter a location : ");					//지역 입력 받기  
            	scanf("%s", &scan_place);
            	printf("\n");
            	
            	
            	for(j=0;j<ifctdb_len();j++){					//모든 환자 데이터를 하나씩 점검 
                	ifct_element = ifctdb_getData(j);
                	
					if (strcmp(scan_place, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1))) == 0){	//환자의 마지막 장소와 입력받은 문자열이 같으면 출력 
						ifctele_printElement(ifct_element);
					}
					
				}
                
                break;
                
            case MENU_AGE:										//3번 선택->특정 나이 범위의 환자 정보 출력  
                
                printf("Enter a minimum age : ");
                scanf("%i", &min_age);							//나이 최솟값 입력받음 
                printf("\nEnter a maximum age : ");
                scanf("%i", &max_age);							//나이 최댓값 입력받음 
                printf("\n");
                
                for(i=0;i<ifctdb_len();i++){					//범위 안에 드는지 모든 환자 데이터를 점검 
                	ifct_element = ifctdb_getData(i);
					if(min_age <= ifctele_getAge(ifct_element) && ifctele_getAge(ifct_element) <= max_age){
						ifctele_printElement(ifct_element);		//범위 안에 든다면 환자 정보 출력 
					}
				}
				 
                break;
                
            case MENU_TRACK:									//4번 선택->전파자를 찾아 만난 시점, 장소를 출력하고 최종적으로 최초 전파자를 출력  
            	
            	printf("Select a patient : ");					//전파자를 찾을 환자를 입력받음  
            	scanf("%i", &root_patient);
            	p_index = root_patient;
            	
            	if (0 <= p_index && p_index <= ifctdb_len()-1){	//db안의 환자를 입력하면  
				
            			spreader = trackInfester(p_index);		//trackInfester 함수를 통해 현재 환자의 전파자를 찾음  
            			
            			if(spreader != p_index){			//전파자가 존재한다면 (본인이 최초 전파자가 아니라면)  
            				ifct_element = ifctdb_getData(spreader);
            				met_infectedtime = isMet(p_index, spreader);		//둘이 만난 시점 저장  
            				met_place_name = ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, convertTimetoIndex(met_infectedtime, ifctele_getinfestedTime(ifct_element))));	//둘이 만난 장소 저장  
            				
            				printf("Patient %i was insfected with patient %i. (met at %i, %s)\n", p_index, spreader, met_infectedtime, met_place_name);		//현재 환자의 전파자, 만난 시점, 장소 출력  
            			}	
            			else if(spreader == p_index){		//전파자가 존재하지 않는다면(본인이 최초 전파자라면) 
            				initial_spreader = spreader;	//현재 환자를 최초 전파자로 저장  
            			}	
            				
            			while(p_index != spreader){		//최초 전파자가 나올때까지 반복  
            				pre_spreader = trackInfester(spreader);				//위와 동일한 방식으로 반복  
            				
            				if(pre_spreader != spreader){
            					ifct_element = ifctdb_getData(pre_spreader);
            					met_infectedtime = isMet(spreader, pre_spreader);
            					met_place_name = ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, convertTimetoIndex(met_infectedtime, ifctele_getinfestedTime(ifct_element))));
            					
            					printf("Patient %i was insfected with patient %i. (met at %i, %s)\n", spreader, pre_spreader, met_infectedtime, met_place_name);
            					
            					spreader = pre_spreader;
            					initial_spreader = pre_spreader;
							}
							else if(pre_spreader == spreader){
								p_index = spreader;					//최초 전파자가 나왔다면 p_index에 spreader값을 저장해서 while문을 빠져나가도록 함  
								initial_spreader = pre_spreader;
							}
						}
						
					printf("The first spreader is number %i patient.\n", initial_spreader);		//최초 전파자 출력  
				}
				else
					printf("[ERROR] You should enter between 0~%i\n", ifctdb_len()-1);	//db밖의 환자를 입력하면 에러 출력  
                    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}


int trackInfester(int patient_no){		//전파자를 찾는 함수
	int i;
	int spreader = -1;		//전파자 초기값 -1 
	int max;				//만난시간이 가장 이른지 알기위한 값 
	int met_time;			//만난 시간 
	void *ifct_element;
	
	ifct_element = ifctdb_getData(patient_no);
	max = ifctele_getinfestedTime(ifct_element);		//max 값을 발병 확인시간으로 정함 
	
	for(i=0;i<ifctdb_len();i++){						  
		if(patient_no != i){							//자신을 제외한 모든 환자를 살펴 전파자 찾기  
			met_time = isMet(patient_no, i);			//isMet 함수(두 환자가 언제만났는지 리턴하는 함수)로 만난 시점 저장  
			if(met_time > 0){							//한번이라도 만났다면 
				if(met_time <= max){					
					max = met_time;						//max값을 만난 시점으로 바꿔서 지금까지 만난 환자 중 가장 이른 시점인지 확인 
					spreader = i;						//전파자를 i번째 환자로 저장 
				}
			}
		} 
	}
	
	if(spreader == -1){									//전파자가 없다면 
		spreader = patient_no;							//자기자신을 전파자로 저장  
	}
	
	return spreader;
}

int isMet(int patient_no, int entered_patient){		//전파자와 만난 시점을 reutrn하는 함수
	int i;
	int p_move_time;		//현재 환자의 시점  
	int p_move_place;		//현재 환자의 해당 시점의 장소  
	int t_move_time;		//대상 환자의 시간 인덱스  
	int t_move_place;		//대상 환자의 해당 시간의 장소  
	int met_time = -1;		//만난 시점 (초기값 -1) 
	void *ifct_element;
	
	for(i=TIME_HIDE;i<N_HISTORY;i++){						//현재 환자의 0,1,2일쨰 날짜(감염될 수 있는 날짜)와 비교
		ifct_element = ifctdb_getData(patient_no);
		p_move_time = (ifctele_getinfestedTime(ifct_element) - i);				//현재 환자의 0일째, 1일째, 2일째의 시점 
		p_move_place = ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1-i);	//그 시점의 장소 인덱스 
		 
		ifct_element = ifctdb_getData(entered_patient);
		t_move_time = convertTimetoIndex(p_move_time, ifctele_getinfestedTime(ifct_element));	//대상 환자의 위 시점에서의 시간 인덱스 저장, 위 시점의 인덱스가 없다면 -1이 저장됨 
		
		if(t_move_time >= N_HISTORY - TIME_HIDE){														//시간 인덱스가 대상 환자의 전파 가능 시기에 해당한다면  
			t_move_place = ifctele_getHistPlaceIndex(ifct_element, t_move_time);				//대상 환자의 위 시간 인덱스에서의 장소 저장 
		
			if(p_move_place == t_move_place){	//현재 환자와 대상 환자의 장소가 같다면 
				met_time = p_move_time;			//결과값에 만난 시점 저장
			}
		}
	}
	
	return met_time;		//만난 시점 출력(만나지 않았다면 초기값인 -1이 return) 
}

int convertTimetoIndex(int time, int infested_time){	//시점을 시간 index로 변환하는 함수
	int index = -1; 		//인덱스 초기값 -1
	
	if (time <= infested_time && time > infested_time - N_HISTORY){
		index = N_HISTORY-(infested_time - time) - 1;			//입력받은 시점을 시간 index로 변환 
	} 
	
	return index;		//시간 인덱스를 반환(해당 시점의 인덱스가 없다면 -1 반환) 
}

















