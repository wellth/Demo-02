#include "Main.h"
#include <sys/time.h>
#include <sqlite3.h>

#define IN_TEXT_FILE                "MDB/in.txt"
#define IN_ENTERY_FILE           "MDB/Inpt.csv"
#define OT_TEXT_FILE               "MDB/out.txt"
#define OT_ENTERY_FILE          "MDB/Data.csv"
#define USER_INFO_FILE           "MDB/user.csv"
//-----Used by UI---
#define LOGIN_SUMMARY_FILE		"MDB/login_sum.csv"
#define BMI_SUMMARY_FILE		"MDB/bmi_sum.csv"
#define BP_SUMMARY_FILE			"MDB/bp_sum.csv"
#define BF_SUMMARY_FILE			"MDB/bf_sum.csv"
#define CO_SUMMARY_FILE			"MDB/co_sum.csv"

#define DAEMON_DB_FILE			"MDB/bmi.db"
//-----Used by UI---
#define USER_SUMMARY_FILE		"MDB/user_summary.csv"

#define WEIGHT_CHECK_TIMEOUT_SEC	30
#define BF_CHECK_TIMEOUT_SEC		30
#define CO_CHECK_TIMEOUT_SEC		90
#define BP_CHECK_TIMEOUT_SEC		150

int WELLTH_CHECK_TIMEOUT_SEC=180;

#define CO_PORT_OPEN_TIMEOUT_SEC	3

//#define RUN_DAEMON_WITHOT_SERIAL_PORT_CONNECTED

char openCO_port ();

typedef struct{
	char Weight[10];
	char Comma;
	char Hight[10];
}_INPUT_DATA;
_INPUT_DATA Din;


typedef struct{
	char *Name;
	char *Password;
	char *Gender;
}_LOGIN_DATA;
_LOGIN_DATA Uin;
char CurRFIDScaned[16]; 

typedef struct{
	char InBuf[10];
}_IN_FILE;
_IN_FILE Indata;

typedef struct{
	char Weight[10];
	char Comma;
	char BmiValue[10];
}_BMI_TEST;
_BMI_TEST Bmi;

typedef struct{
	char SysP[10];
	char Comma1;
	char DiaP[10];
	char Comma2;
	char PulseRate[10];
}_BP_TEST;
_BP_TEST Bp;

typedef struct{
	char BliValue[10];
}_BLI_TEST;
_BLI_TEST Bli;

typedef struct{
	char LoginReply[15];
	char Name[64];
	char Age[8];
	char Gender[8];
}_LOGIN_TEST;
_LOGIN_TEST login;


char *SerialPortName[] = {	"/dev/ttyUSB0",
							"/dev/ttyUSB1",
							"/dev/ttyUSB2",
							"/dev/ttyUSB3",
							"/dev/ttyUSB4",
							"/dev/ttyUSB5",
							"/dev/ttyUSB6",
							"/dev/ttyUSB7",
							"/dev/ttyUSB8",
							"/dev/ttyUSB9",    
							"/dev/ttyS0",
							"/dev/ttyS1",                                                           
							"/dev/ttyS2",
							"/dev/ttyS3",
							"/dev/ttyS4",
							"/dev/ttyS5",
							"/dev/ttyS6",
							"/dev/ttyS7",
							"/dev/ttyS8",
							"/dev/ttyS9"
						};
/********************************************************************************************
			Function Declarations
*********************************************************************************************/
void CheckTest(void);
void CallWhileLoop(void);
void ReadInputFile(void);
void CheckUserInfo(void);
void CheckEndOfTest(void);
void DisplayMessage(void);
char OpenSerialPort(void);   	
void CompareUserInfo(void);
float ConvertHexToFloat(void);
void ClearDemonInputFile();
void f(void);
extern unsigned char  ModbusOP(unsigned char  slaveNo, unsigned char  functionCode, short int startAddress, short int  numItems, unsigned char  *dataBuffer, unsigned char  numRetries);
extern unsigned char  ConstructRequest(unsigned char  slaveNo, unsigned char  functionCode, short int startAddress, short int  numItems, unsigned char  *dataBuffer,MDB_REQ_ADU *MbReqAdu, MDB_RSP_ADU *MbRspAdu);
/********************************************************************************************
			Variable Declarations
*********************************************************************************************/
int fd, RFID_fd, CO_fd;
FILE *fp;

FILE* fp1, *fpUI;

float BMIValue;
float BLIValue;
int baud,TestNo;
char cbuf, UserMode=0;
char SerialPortStatus = 0;
short int MdbResponse = 0;	
int Hight=0,TempWeight;
char UserName[10];
char UserPassword[10];
unsigned char MdbFlag = 0;
unsigned char RxBuf[100];
unsigned char TxBuf[30];
unsigned char BmiBuf[10];
unsigned char TempBuf[10];
volatile unsigned int LoopCase = 1;
unsigned char Buf[5];
float Weight,Sub,Mul;
long long Current_MobileNo = 0;
int Current_Age = 0, Current_Gender =0;
	
extern unsigned short int BmiTestEn;
extern unsigned short int BpTestEn;
extern unsigned short int BliTestEn;
extern unsigned char HoldingReg[10];


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	int rowpr=argc-1;
	NotUsed=0;

	for(i=0; i<rowpr; i++)
		printf("%s ",azColName[i]);

	printf("%s\n",azColName[rowpr]);

	for(i=0; i<rowpr; i++){
	//	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		printf("%s ",  argv[i] ? argv[i] : "NULL");
	}
	printf("%s\n",  argv[rowpr] ? argv[rowpr] : "NULL");

	return 0;
}
/********************************************************************************************
			Main Starts Here
*********************************************************************************************/
int main(void){
    //CompareUserInfo();
    //while(1);

	ClearDemonInputFile();
    OpenSerialPort();
	memset(CurRFIDScaned, 0, 16);
/*
	while(1){
		memset(CurRFIDScaned, 0, 16);
		read(RFID_fd, CurRFIDScaned, 16);
		printf("%s",CurRFIDScaned);
	}
*/
//	system("/bin/dmesg | grep \"pl2303 converter now\" > /home/ab.c");
	//system("/bin/xinput\ set-prop 11 'Coordinate Transformation Matrix' 8 0 0 0 8 0 0 0 1");
	//system("/bin/xinput\ set-prop 10 'Coordinate Transformation Matrix' 8 0 0 0 8 0 0 0 1");

    if(SerialPortStatus==1){
        LoopCase = 1;
        MdbFlag = 0;
        printf(" Wellth System is running \n");
        CallWhileLoop();
        close(fd);
    }
    else{
    	return 0;
    }
}

void ReadInputFile(void){
    FILE *ffp,*fp;
    ffp = fopen (IN_TEXT_FILE, "r");
    if(ffp<0){
        printf(" System is not able to open file \n");
       LoopCase = 0;    
    }
    else{
        fread(&Indata,sizeof(Indata),1,ffp);
        fclose(ffp);
        if(strcmp(Indata.InBuf,"WeTest") == 0){
            memset(Indata.InBuf,0,10);
            TestNo = 1;
        }
        else if(strcmp(Indata.InBuf,"BpTest") == 0){
            memset(Indata.InBuf,0,10);
            TestNo = 2;
        }
        else if(strcmp(Indata.InBuf,"BliTest") == 0){
            memset(Indata.InBuf,0,10);
            TestNo = 3;
        }
	else if (strcmp(Indata.InBuf,"CoTest") == 0) {
		memset(Indata.InBuf,0,10);
		TestNo = 4;
	}
    else if(strcmp(Indata.InBuf,"Login") == 0){
			printf("\n*******login flag set\n");
            memset(Indata.InBuf,0,10);
            UserMode = 1;
        }
	else if (strcmp(Indata.InBuf,"Swipe") == 0) {
		memset(Indata.InBuf,0,10);
		UserMode = 2;
	}
        else{
            TestNo =0;
			UserMode = 0;
        }
       LoopCase = 1;    
    }
	//printf("loop case=%s", Indata.InBuf);
}

struct timeval start, end;
long prevSecVal = 0;
int testStartTime =0;
char tmpBLIValue[10];
char tmpBLIValue1[10];
float temp_bli_val;
char query[512];

void CallWhileLoop(void){
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char timeBuf[64];
	char Line[512];
	FILE *fpt;
	char mob_buf[16],age_buf[16],gender_buf[16];
	float Bmi_Val,Bli_Value;
	int Health_Score,Syst_Pressure,Diast_Pressure;

	FILE * app_fp = fopen("/home/time_app.c", "w");
	system("/bin/chmod 777 /home/time_app.c");
	fchmod (app_fp, S_IROTH | S_IWOTH | S_IXOTH);
	fclose(app_fp);	

	while(1){
		if (LoopCase != 1){
			/*****************************
			check the time if it is more than 2 min then 
			1) clear the daemon input files
			2) give a msg to the UI that sensors are not responding
			3) change loopcase to 1
			*************************/
 			gettimeofday(&start, NULL);
			if(prevSecVal != start.tv_sec){
				prevSecVal = start.tv_sec;
				testStartTime++;
				printf("\n__%d__\n", testStartTime);
			}
		} else {
			testStartTime = 0;
		}
		if(testStartTime > WELLTH_CHECK_TIMEOUT_SEC){
			LoopCase = 1;
			ClearDemonInputFile();	
			printf("/**********************/\nRetry Test..! No Responce From Sensor resetted the Daemon....!\n/**********************/\n");
		}
		switch(LoopCase){		
			case 1:
                  ReadInputFile();
                  CheckTest();
                  CheckUserInfo();
             break;
             //<<<<<<<<<<<<.................................................. Sensor No 1 Interface.......................................................................................>>>>>>>>>>>>>//
			case 2:
				WELLTH_CHECK_TIMEOUT_SEC = WEIGHT_CHECK_TIMEOUT_SEC;

				printf(" loop case value is = %d", LoopCase);
		       HoldingReg[1] = 0x00;   
               HoldingReg[0] = 0x01;
               MdbResponse = ModbusOP(0x01,FC_WRITE_SINGLE_COIL, 0x000A, 0x000l, (unsigned char *)HoldingReg,1);
               if(MdbResponse!=0){
                    #if defined	SHOW_DISPLAY
                        printf("Modbus Error Response %d \n",MdbResponse);
                     #endif
               }
               else{
               printf("Modbus Error Response %d \n",MdbResponse);
                    HoldingReg[1] = 0x00;   
                    HoldingReg[0] = 0x00;
     				LoopCase = LoopCase +1;
                }
			break;			
			case 3:
				printf(" loop case value is = %d", LoopCase);
                MdbResponse = ModbusOP(0x01,FC_READ_COILS, 0x000F, 0x0000, (unsigned char *)HoldingReg, 1);
                if((RxBuf[0] == 0x01) && (RxBuf[1] == 0x01) && (RxBuf[2] == 0x01) && (RxBuf[3] == 0x01)){
    				LoopCase = LoopCase +1;
    				BmiTestEn = 1;
                }
             break;
             case 4:   
				printf(" loop case value is = %d", LoopCase);   
                 MdbResponse = ModbusOP(0x01,FC_READ_HOLD_REGS, 0x000A, 0x0002, (unsigned char *)HoldingReg, 1);
                 if(MdbResponse!=0){
                    #if defined	SHOW_DISPLAY
                        printf("Modbus Error Response %d \n",MdbResponse);
                     #endif
                 }
                else{  
                    Weight = ConvertHexToFloat();
                    printf("Modbus Got Sensor Response\n");
                    printf("Your Weight is = %f\n",Weight); 
					if(Weight < 0)
						Weight *= (-1);

                    BMIValue  = (Weight/Hight)*10000;
                    BMIValue = BMIValue / Hight;
                    printf("Your BMI Level is = %f\n",BMIValue);   
    				Buf[0] = '1';
                    memset(TempBuf,0,10);
                    sprintf(TempBuf,"%f",Weight);
                    strcpy(Bmi.Weight,TempBuf);
                    memset(TempBuf,0,10);
                    sprintf(TempBuf,"%f",BMIValue);
                    strcpy(Bmi.BmiValue,TempBuf);
                    memset(TempBuf,0,10);
                    
                    fp = fopen (OT_ENTERY_FILE, "w");
                    if(fp<0){
                        printf(" System is not able to open file \n");
                    }
                    else{
                        fwrite(&Bmi,sizeof(Bmi),1,fp);
                        fclose(fp);
                    }

					fpUI = fopen (BMI_SUMMARY_FILE, "w"); //open file in append mode now onwards
					printf("1>>BMI summary file opened : %s\n", BMI_SUMMARY_FILE);
                    if(fpUI<0){
                        printf(" System is not able to open file \n");
                    } else {
						printf("BMI summary file opened : %s\n", BMI_SUMMARY_FILE);
						printf(">>>>>>>>weight:%s bmi-%s height-- %s",Bmi.Weight, Bmi.BmiValue, Din.Hight);
						Bmi.Comma = ',';

						
						fwrite(Bmi.Weight,strlen(Bmi.Weight),1,fpUI);
						fwrite(&Bmi.Comma,1,1,fpUI);
						fwrite(Bmi.BmiValue,strlen(Bmi.BmiValue),1,fpUI);
						fwrite(&Bmi.Comma,1,1,fpUI);
						fwrite(Din.Hight, strlen(Din.Hight), 1, fpUI);
//---------------------Calculate Score ----------------------------------------


			Bmi_Val = atof(Bmi.BmiValue);
			if (Current_Gender == 1)  // Female
			{
				if (Current_Age  < 18)
					Health_Score = 1;
				else if ((Current_Age >= 18) && (Current_Age <= 20))
				{
					if ( (Bmi_Val >= 15.05) && (Bmi_Val <= 25.25))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 21) && (Current_Age <= 25))
				{
					if ( (Bmi_Val >= 15.24) && (Bmi_Val <= 27.05))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 26) && (Current_Age <= 30))
				{
					if ( (Bmi_Val >= 15.20) && (Bmi_Val <= 28.43))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 31) && (Current_Age <= 35))
				{
					if ( (Bmi_Val >= 16.36) && (Bmi_Val <= 30.83))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 36) && (Current_Age <= 40))
				{
					if ( (Bmi_Val >= 17.0) && (Bmi_Val <= 33.75))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 41) && (Current_Age <= 45))
				{
					if ( (Bmi_Val >= 16.76) && (Bmi_Val <= 34.01))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 46) && (Current_Age <= 50))
				{
					if ( (Bmi_Val >= 16.08) && (Bmi_Val <= 32.50))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 51) && (Current_Age <= 55))
				{
					if ( (Bmi_Val >= 18.50) && (Bmi_Val <= 34.51))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 56) && (Current_Age <= 60))
				{
					if ( (Bmi_Val >= 16.39) && (Bmi_Val <= 33.44))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else  // Age Greater than 60
				{
					if ( (Bmi_Val >= 16.15) && (Bmi_Val <= 31.16))
						Health_Score = 1;
					else
						Health_Score = 0;
				}




			}
			else   // Male
			{

				if (Current_Age  < 18)
					Health_Score = 1;
				else if ((Current_Age >= 18) && (Current_Age <= 20))
				{
					if ( (Bmi_Val >= 15.57) && (Bmi_Val <= 25.03))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 21) && (Current_Age <= 25))
				{
					if ( (Bmi_Val >= 15.87) && (Bmi_Val <= 26.51))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 26) && (Current_Age <= 30))
				{
					if ( (Bmi_Val >= 16.43) && (Bmi_Val <= 27.54))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 31) && (Current_Age <= 35))
				{
					if ( (Bmi_Val >= 16.33) && (Bmi_Val <= 28.67))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 36) && (Current_Age <= 40))
				{
					if ( (Bmi_Val >= 17.21) && (Bmi_Val <= 29.89))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 41) && (Current_Age <= 45))
				{
					if ( (Bmi_Val >= 17.51) && (Bmi_Val <= 30.11))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 46) && (Current_Age <= 50))
				{
					if ( (Bmi_Val >= 17.99) && (Bmi_Val <= 30.20))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 51) && (Current_Age <= 55))
				{
					if ( (Bmi_Val >= 18.17) && (Bmi_Val <= 35.76))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else if ((Current_Age >= 56) && (Current_Age <= 60))
				{
					if ( (Bmi_Val >= 16.99) && (Bmi_Val <= 32.24))
						Health_Score = 1;
					else
						Health_Score = 0;
				}
				else  // Age Greater than 60
				{
					if ( (Bmi_Val >= 17.53) && (Bmi_Val <= 30.82))
						Health_Score = 1;
					else
						Health_Score = 0;
				}



			}
						
//----------------------database loging--------------------------------------------------
						
						system("/bin/date > /home/time.c");
						fpt = fopen("/home/time.c", "r");
						while( GetLine(fpt, Line) != 0){
							strcpy(timeBuf, Line);
							//printf("%s\n", Line);
						}
						sprintf(mob_buf,"%ll",Current_MobileNo);
						sprintf(age_buf,"%d",Current_Age);
						if ( Current_Gender == 1)
							strcpy(gender_buf,"Female");
						else
							strcpy(gender_buf,"Male");
						sprintf(query, "insert into bmi_t (mobile_number,weight, bmi, height,health_score, log_time) values ('%s','%s', '%s', '%s','%d', '%s') ", mob_buf, Bmi.Weight, Bmi.BmiValue, Din.Hight,Health_Score, timeBuf);
						// sprintf(query, "insert into bmi_t (weight, bmi, height, log_time) values ('%s', '%s', '%s', '%s') ", Bmi.Weight, Bmi.BmiValue, Din.Hight, timeBuf);

						if (sqlite3_open(DAEMON_DB_FILE, &db) != SQLITE_OK)
						{
							fprintf(stderr, "Can't open database: \n");
							sqlite3_close(db);
							exit(1);
						  }
						  rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
						  if( rc!=SQLITE_OK ){
							fprintf(stderr, "SQL error: %s\n", zErrMsg);
						  }
						  sqlite3_close(db);
					}
					fclose(fpUI);

                    fp = fopen (OT_TEXT_FILE, "w");
                    if(fp<0){
                        printf(" System is not able to open file \n");
                    }
                    else{
                        fwrite(&Buf,1,1,fp);
                        fclose(fp);
                    }
                    printf(" Demon has completed the  Wellth BMI Test\n");
                    BmiTestEn = 0;
             		LoopCase = 1;
                    printf("LoopCase Value is %d\n",LoopCase);
                    fp  = fopen(IN_ENTERY_FILE,"w");
                    fclose(fp);
                    fp = fopen(IN_TEXT_FILE,"w");
                    fclose(fp);                    
                }
             break;
             case 5:
             break;

             //<<<<<<<<<<<<.................................................. Sensor No 2 Interface.......................................................................................>>>>>>>>>>>>>//
             
			case 6:	
				WELLTH_CHECK_TIMEOUT_SEC = BP_CHECK_TIMEOUT_SEC;
				printf(" loop case value is = %d", LoopCase);	
		       HoldingReg[1] = 0x00;   
               HoldingReg[0] = 0x01;
               MdbResponse = ModbusOP(0x01,FC_WRITE_SINGLE_COIL, 0x000B, 0x000l, (unsigned char *)HoldingReg,1);
               if(MdbResponse!=0){
                   // #if defined	SHOW_DISPLAY
                        printf("Modbus Error Response %d \n",MdbResponse);
                    // #endif
               }
               else{
                    HoldingReg[1] = 0x00;   
                    HoldingReg[0] = 0x00;
     				LoopCase = LoopCase +1;
                }
			break;			
			case 7:
				printf(" loop case value is = %d", LoopCase);
                MdbResponse = ModbusOP(0x01,FC_READ_COILS, 0x000F, 0x0000, (unsigned char *)HoldingReg, 1);
                if((RxBuf[0] == 0x01) && (RxBuf[1] == 0x01) && (RxBuf[2] == 0x01) && (RxBuf[3] == 0x01)){
    				LoopCase = LoopCase +1;
                    BpTestEn = 1;
                }
             break;
             case 8:  
				printf(" loop case value is = %d", LoopCase);    
                 MdbResponse = ModbusOP(0x01,FC_READ_HOLD_REGS, 0x000B, 0x0002, (unsigned char *)HoldingReg, 1);
                 if(MdbResponse!=0){
                    #if defined	SHOW_DISPLAY
                        printf("Modbus Error Response %d \n",MdbResponse);
                     #endif
                 }
                else{  
                    printf("Modbus Got Sensor Response\n");
                    printf("Systolic Pressure is = %d\n",BmiBuf[0]);   
                    printf("Diastolic Pressure is = %d\n",BmiBuf[1]);   
                    printf("Pulse Rate is = %d\n",BmiBuf[2]);   
                    BpTestEn = 0;
                    
                    sprintf(Bp.SysP,"%d",BmiBuf[0]);
                    sprintf(Bp.DiaP,"%d",BmiBuf[1]);
                    sprintf(Bp.PulseRate,"%d",BmiBuf[2]);
									
                    fp = fopen (OT_ENTERY_FILE, "w");
                    if(fp<0){
                        printf(" System is not able to open file \n");
                    }
                    else{
                        fwrite(&Bp,sizeof(Bp),1,fp);
                        fclose(fp);
                    }
					Buf[0] = ',';
					fpUI = fopen (BP_SUMMARY_FILE, "w"); //open file in append mode now onwards
                    if(fpUI<0){
                        printf(" System is not able to open file \n");
                    } else {
						fwrite(Bp.SysP,strlen(Bp.SysP),1,fpUI);
						fwrite(&Buf[0],1,1,fpUI);
						fwrite(Bp.DiaP,strlen(Bp.DiaP),1,fpUI);
						fwrite(&Buf[0],1,1,fpUI);
						fwrite(Bp.PulseRate,strlen(Bp.PulseRate),1,fpUI);
						fclose(fpUI);
					}
	//------------------Health_SCore Calculation -----------------------

		Syst_Pressure = atoi(Bp.SysP); 
		Diast_Pressure = atoi(Bp.DiaP);

		if ((Syst_Pressure < 140) && (Diast_Pressure < 90))
			Health_Score = 1;
		else
			Health_Score = 0;

	//----------------------database loging--------------------------------------------------
					system("/bin/date > /home/time.c");
					fpt = fopen("/home/time.c", "r");
					while( GetLine(fpt, Line) != 0){
						strcpy(timeBuf, Line);
						//printf("%s\n", Line);
					}
					sprintf(mob_buf,"%ll",Current_MobileNo);

					sprintf(query, "insert into bp_t (mobile_no,sbp_val, dbp_val, pr_val,health_score, log_time) values ('%s','%s', '%s', '%s','%d', '%s') ",mob_buf, Bp.SysP, Bp.DiaP, Bp.PulseRate,Health_Score, timeBuf);

					if (sqlite3_open(DAEMON_DB_FILE, &db) != SQLITE_OK)
					{
						fprintf(stderr, "Can't open database: \n");
						sqlite3_close(db);
						exit(1);
					  }
					  rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
					  if( rc!=SQLITE_OK ){
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
					  }
					  sqlite3_close(db);

					Buf[0] = '1';
                    fp = fopen (OT_TEXT_FILE, "w");
                    if(fp<0){
                        printf(" System is not able to open file \n");
                    }
                    else{
                        fwrite(&Buf,1,1,fp);
                        fclose(fp);
                    }
                    printf(" Demon has completed the  Wellth BMI Test\n");
             		LoopCase = 1;
                    printf("LoopCase Value is %d\n",LoopCase);
                    fp  = fopen(IN_ENTERY_FILE,"w");
                    fclose(fp);
                    fp = fopen(IN_TEXT_FILE,"w");
                    fclose(fp);   
                }
             break;       
             
             //<<<<<<<<<<<<.................................................. Sensor No 3 Interface.......................................................................................>>>>>>>>>>>>>//
           case 9:
				printf(" loop case value is = %d", LoopCase);
                LoopCase = LoopCase +1;
  			 break;  			 	  
			case 10:
				WELLTH_CHECK_TIMEOUT_SEC = BF_CHECK_TIMEOUT_SEC;

				printf(" loop case value is = %d", LoopCase);		
		       HoldingReg[1] = 0x00;   
               HoldingReg[0] = 0x01;
               MdbResponse = ModbusOP(0x01,FC_WRITE_SINGLE_COIL, 0x000C, 0x000l, (unsigned char *)HoldingReg,1);
               if(MdbResponse!=0){
                    #if defined	SHOW_DISPLAY
                        printf("Modbus Error Response %d \n",MdbResponse);
                     #endif
               }
               else{
                    HoldingReg[1] = 0x00;   
                    HoldingReg[0] = 0x00;
     				LoopCase = LoopCase +1;
                }
			break;			
			case 11:
				printf(" loop case value is = %d", LoopCase);
                MdbResponse = ModbusOP(0x01,FC_READ_COILS, 0x000F, 0x0000, (unsigned char *)HoldingReg, 1);
                if((RxBuf[0] == 0x01) && (RxBuf[1] == 0x01) && (RxBuf[2] == 0x01) && (RxBuf[3] == 0x01)){
    				LoopCase = LoopCase +1;
                    BpTestEn = 1;
                }
             break;
             case 12:    
				printf(" loop case value is = %d", LoopCase);  
                 MdbResponse = ModbusOP(0x01,FC_READ_HOLD_REGS, 0x000C, 0x0002, (unsigned char *)HoldingReg, 1);
                 if(MdbResponse!=0){
                    #if defined	SHOW_DISPLAY
                        printf("Modbus Error Response %d \n",MdbResponse);
                     #endif
                 }
                else{  
                    BliTestEn =  1;
                    BLIValue = ConvertHexToFloat();
                    printf("Modbus Got Sensor Response\n");
                    printf("Sensor Response is  = %f\n",BLIValue); 
					sprintf(tmpBLIValue,"%f", BLIValue);
					tmpBLIValue[5] = 0;  
					temp_bli_val = atof(tmpBLIValue);
					if( (temp_bli_val >= 3.276) || (temp_bli_val <= 0.000) ) {
		                strcpy(Bli.BliValue, "-1");
                    } else {
						BLIValue  = BLIValue*10000;
		                Mul = (Hight*Hight)/BLIValue;
		                Sub  = 12.297 + (0.287*Mul);
		                BLIValue  = 0.697 * TempWeight;
						printf("temp weight %d", TempWeight);
		                BLIValue = BLIValue  - Sub;
		                printf("Your BMI Level is = %f\n",BLIValue);   
						LoopCase = 1;
		                BliTestEn =  0;
						Buf[0] = '1';
		                memset(TempBuf,0,10);
		                sprintf(TempBuf,"%f",BLIValue);
		                strcpy(Bli.BliValue,TempBuf);
		                memset(TempBuf,0,10);
					}
                    fp = fopen (OT_ENTERY_FILE, "w");
                    if(fp<0){
                        printf(" System is not able to open file \n");
                    }
                    else{
                        fwrite(&Bli,sizeof(Bli),1,fp);
                        fclose(fp);
                    }

					fpUI = fopen (BF_SUMMARY_FILE, "w"); //open file in append mode now onwards
                    if(fpUI<0){
                        printf(" System is not able to open file \n");
                    } else {
						fwrite(Bli.BliValue,strlen(Bli.BliValue),1,fpUI);
						fclose(fpUI);
					}
//-----------------------Health_SCore Calculation-------------------------

		Bli_Value = atof(Bli.BliValue);
		if (Current_Gender == 1)  // Female
		{
			if (Current_Age < 20)
				Health_Score = 1;
			else if ((Current_Age >=20) && (Current_Age <=39))
			{
				if ((Bli_Value > 21.0) && (Bli_Value < 39.0))
					Health_Score = 1;
				else
					Health_Score = 0;
			}
			else if ((Current_Age >=40) && (Current_Age <=59))
			{
				if ((Bli_Value > 23.0) && (Bli_Value < 40.0))
					Health_Score = 1;
				else
					Health_Score = 0;
			}
			else if ((Current_Age >=60) && (Current_Age <=79))
			{
				if ((Bli_Value > 24.0) && (Bli_Value < 42.0))
					Health_Score = 1;
				else
					Health_Score = 0;
			}
			else
				Health_Score = 1;

		}
		else
		{

			if (Current_Age < 20)
				Health_Score = 1;
			else if ((Current_Age >=20) && (Current_Age <=39))
			{
				if ((Bli_Value > 8.0) && (Bli_Value < 25.0))
					Health_Score = 1;
				else
					Health_Score = 0;
			}
			else if ((Current_Age >=40) && (Current_Age <=59))
			{
				if ((Bli_Value > 11.0) && (Bli_Value < 28.0))
					Health_Score = 1;
				else
					Health_Score = 0;
			}
			else if ((Current_Age >=60) && (Current_Age <=79))
			{
				if ((Bli_Value > 13.0) && (Bli_Value < 30.0))
					Health_Score = 1;
				else
					Health_Score = 0;
			}
			else
				Health_Score = 1;

		}



//----------------------database loging--------------------------------------------------
					system("/bin/date > /home/time.c");
					fpt = fopen("/home/time.c", "r");
					while( GetLine(fpt, Line) != 0){
						strcpy(timeBuf, Line);
						//printf("%s\n", Line);
					}

					sprintf(mob_buf,"%ll",Current_MobileNo);

					sprintf(query, "insert into bf_t (mobile_number,bf_val,health_score, log_time) values ('%s','%s','%d','%s') ",mob_buf, Bli.BliValue,Health_Score, timeBuf);

					if (sqlite3_open(DAEMON_DB_FILE, &db) != SQLITE_OK)
					{
						fprintf(stderr, "Can't open database: \n");
						sqlite3_close(db);
						exit(1);
					  }
					  rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
					  if( rc!=SQLITE_OK ){
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
					  }
					  sqlite3_close(db);
					
                    fp = fopen (OT_TEXT_FILE, "w");
                    if(fp<0){
                        printf(" System is not able to open file \n");
                    }
                    else{
                        fwrite(&Buf,1,1,fp);
                        fclose(fp);
                    }
                    printf(" Demon has completed the  Wellth BLI Test\n");
                    BliTestEn = 0;
             		LoopCase = 1;
                    printf("LoopCase Value is %d\n",LoopCase);
                    fp  = fopen(IN_ENTERY_FILE,"w");
                    fclose(fp);
                    fp = fopen(IN_TEXT_FILE,"w");
                    fclose(fp);            
                }
             break; 
		case 13:
			WELLTH_CHECK_TIMEOUT_SEC = CO_CHECK_TIMEOUT_SEC;
			printf(" loop case value is = %d", LoopCase);
			printf("\ngoing to operate CO-sensor\n");
			OperateCOSensor();
		break;
             default:
			 break;
		}
	}
}

void OperateCOSensor(void){
	FILE *fp, *fpUI;
	char co_sensor_val[32];
	char CharArr[512];
    char *ArrStr[4];
    char *ArrOfStr[32];
    char *StrPtr;
    char StrSerialPortName[64];
    int len, co_started, i, st_co_reading, prev_co_reading, cur_co_reading;
    int delta, blowing_started;
    char co_start, out_char;
    float ratio;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char timeBuf[64];
	char Line[512];
	FILE *fpt;

//	close(fd);
//	close(RFID_fd);
//	close(CO_fd);
//	OpenSerialPort();
//	openCO_port();
	printf("opening serial port is done\n");

	co_start = 0;
	prevSecVal = 0;
    co_started = 0;
    st_co_reading = 0;
    prev_co_reading = 0;
    cur_co_reading = 0;
    delta = 0; blowing_started = 0;
	printf("%cskjhdkjdshkfjhds\n--",co_start);
    while( 1 ){
		printf("%c--%d",co_start, LoopCase);
		if (LoopCase == 13){
			/*****************************
			check the time if it is more than 2 min then 
			1) clear the daemon input files
			2) give a msg to the UI that sensors are not responding
			3) change loopcase to 1
			*************************/
 			gettimeofday(&start, NULL);
			if(prevSecVal != start.tv_sec){
				prevSecVal = start.tv_sec;
				testStartTime++;
				printf("\n__%d__\n", testStartTime);
			}
		} else {
			testStartTime = 0;
		}
		if(testStartTime > WELLTH_CHECK_TIMEOUT_SEC){
			LoopCase = 1;
			ClearDemonInputFile();
			printf("/**********************/\nRetry Test..! No Responce From Sensor resetted the Daemon....!\n/**********************/\n");
			break;
		
		}
        if(co_started == 0){
            //while(read(CO_fd, &co_start, sizeof(co_start)) <= 0);
//			read(CO_fd, &co_start, sizeof(co_start));
			nu_SerialComPort_BlockRead(CO_fd, &co_start, sizeof(co_start));
            //printf("%d -- %c\n",CO_fd, co_start);
        }
        if(co_start == 'A' || co_start == '5'){
            for(i=0; i<32; i++){
                ArrOfStr[i] = NULL;
            }
            memset(CharArr, 0, sizeof(CharArr));
            printf("enterin in serial reads");
            prev_co_reading = cur_co_reading;
            nu_SerialComPort_BlockRead(CO_fd, CharArr, sizeof(CharArr));
            printf("entering in split");
            StringSplit(ArrOfStr , CharArr, " ");
            if(st_co_reading == 0) {
                printf("\nCo-sensor start value = %d\n", st_co_reading);
				if(ArrOfStr[18] != NULL) {
					st_co_reading = atoi(ArrOfStr[9]);
					cur_co_reading = st_co_reading;
					co_started = 1;
					printf("\nCo-sensor start value = %d\n", st_co_reading);
				}
            } else {
                cur_co_reading = atoi(ArrOfStr[9]);
                printf("\nCo-sensor  value = %d\n", cur_co_reading);
                //if(prev_co_reading < cur_co_reading)
                delta =    st_co_reading - cur_co_reading;
                if(delta > 1000)
                    blowing_started = 1;
                if(blowing_started == 1){
                    if(prev_co_reading < cur_co_reading) {
                        ratio = (float)st_co_reading / (float)cur_co_reading;
                        break;
                    }
                }
            }
            //co_started = 1;
			FreeArrayOfString(ArrOfStr);
        }
    }
    printf("%s\n",ArrOfStr[9]);
    //FreeArrayOfString(ArrOfStr);
    printf("the ratio is = %f\n", ratio);
	sprintf(co_sensor_val, "%f", ratio);
	/****************************/
    fp = fopen (OT_ENTERY_FILE, "w");
        if(fp<0){
            printf(" System is not able to open file \n");
        }
        else{
            fwrite(co_sensor_val, strlen(co_sensor_val), 1, fp);
            fclose(fp);
        }
	/****************************/
    fpUI = fopen (CO_SUMMARY_FILE, "w"); //open file in append mode now onwards
        if(fpUI<0){
            printf(" System is not able to open file \n");
        } else {
			fwrite(co_sensor_val, strlen(co_sensor_val), 1, fpUI);
			fclose(fpUI);
		}

	//----------------------database loging--------------------------------------------------
		system("/bin/date > /home/time.c");
		fpt = fopen("/home/time.c", "r");
		while( GetLine(fpt, Line) != 0){
			strcpy(timeBuf, Line);
			//printf("%s\n", Line);
		}
		sprintf(query, "insert into bf_t (co_val, log_time) values ('%s', '%s') ", co_sensor_val, timeBuf);

		if (sqlite3_open(DAEMON_DB_FILE, &db) != SQLITE_OK)
		{
			fprintf(stderr, "Can't open database: \n");
			sqlite3_close(db);
			exit(1);
		  }
		  rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
		  if( rc!=SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		  }
		  sqlite3_close(db);
	/****************************/
	out_char = '1';
	fp = fopen (OT_TEXT_FILE, "w");
        if(fp<0){
            printf(" System is not able to open file \n");
        }
        else{
            fwrite(&out_char,1,1,fp);
            fclose(fp);
        }
    printf(" Demon has completed the  Wellth CO Test\n");
	LoopCase = 1;
	/****************************/
    printf("LoopCase Value is %d\n",LoopCase);
    fp  = fopen(IN_ENTERY_FILE,"w");
    fclose(fp);
    fp = fopen(IN_TEXT_FILE,"w");
    fclose(fp);     
}

char MODBUS_USB[32];
char CO_PORT_USB[32];

char OpenSerialPort(void){
	FILE *rfid_fp;
	char PrevLine[512];
	char Line[512];
	char *ArrStr[4], cha = 0;
	char *StrPtr, co_start;
	char StrSerialPortName[64];
	int len, i, dum_fd;
	/********************Open RFID connected serial port*********************/
	#if 0
		system("/bin/dmesg | grep \"FTDI USB Serial Device converter now\" > /home/ab.c");
		rfid_fp = fopen("/home/ab.c", "r");
		while( GetLine(rfid_fp, Line) != 0){
			//StrPtr = strstr(Line, "FTDI USB Serial Device converter now attached to");
			//if(StrPtr != NULL){
			//	strcpy(PrevLine, Line);
			//}
			strcpy(PrevLine, Line);
			//printf("%s\n", Line);
		}
		printf("---\n %s\n", PrevLine);
		StrPtr = strstr(PrevLine, "FTDI USB Serial Device converter now attached to");
		if(StrPtr == NULL) {
			printf("\n******************************************\nError : The RFID Sensor is not connected\n******************************************\n");
		} else {
			StrPtr = strstr(PrevLine, "tty");
			strcpy(StrSerialPortName, "/dev/");
			strcat(StrSerialPortName, StrPtr);
			len = strlen(StrSerialPortName);
			printf(">>> %s -- %d\n", StrSerialPortName, len);
			StrSerialPortName[len] = 0;
			RFID_fd = ArgOpenSerialPort(StrSerialPortName, 9600);
		}
	#endif
	
	#ifdef RUN_DAEMON_WITHOT_SERIAL_PORT_CONNECTED
		SerialPortStatus = 1;
		return 0;
	#else

	/********************Open Co-Processor connected serial port*********************/
	memset(MODBUS_USB, 0, sizeof(MODBUS_USB));
	fd=-1;

//	system("/bin/dmesg | grep \"cp210x converter now\" > /home/ab.c");
	system("/bin/dmesg | grep \"pl2303 converter now\" > /home/ab.c");
	rfid_fp = fopen("/home/ab.c", "r");
	while( GetLine(rfid_fp, Line) != 0){
		strcpy(PrevLine, Line);
		//printf("%s\n", Line);
	}
	printf("---\n %s\n", PrevLine);
	StrPtr = strstr(PrevLine, "pl2303 converter now attached to");
//	StrPtr = strstr(PrevLine, "cp210x converter now attached to");
	if(StrPtr == NULL) {
		printf("\n******************************************\nError : The Co-Processor is not connected\n******************************************\n");
		return 0;
	} else {
		StrPtr = strstr(PrevLine, "tty");
		strcpy(StrSerialPortName, "/dev/");
		strcat(StrSerialPortName, StrPtr);
		len = strlen(StrSerialPortName);
		printf(">>>%s -- %d\n", StrSerialPortName, len);
		StrSerialPortName[len] = 0;
		strcpy(MODBUS_USB, StrSerialPortName);
		fd = ArgOpenSerialPort(StrSerialPortName, 9600);
		if (fd < 0){
			printf("\nAlert: reconnect the master co-processor port USB and restart the daemon");
			return 0;
		} else {
			SerialPortStatus = 1;
		}
	}
	if(fd < 0) {
		printf("__Error : The Co-Processor is not connected __");
		return 0;
	}

	/********************Open CO-sensor connected serial port*********************/
	prevSecVal= 100;
	testStartTime = 0;
    i=0;
	memset(CO_PORT_USB, 0, sizeof(CO_PORT_USB));
	CO_fd = -1;
    while(i<20){
		if (strcmp(SerialPortName[i], MODBUS_USB) ) {
			printf("\n-----%s----\n", SerialPortName[i]);
		  	dum_fd = ArgOpenSerialPort(SerialPortName[i], 9600);//nu_SerialComPort_Open(SerialPortName[i]);
			if(dum_fd < 0) {
				//printf("\nAlert: reconnect the CO sensor ( CO- sensor USB) and restart the daemon");
				//return 0;
				//break;
		    }
			else{
				testStartTime = 0;
				while( 1 ){
					//printf("%c--",co_start);
					/*****************************
					check the time if it is more than 2 min then 
					1) clear the daemon input files
					2) give a msg to the UI that sensors are not responding
					3) change loopcase to 1
					*************************/
					gettimeofday(&start, NULL);
					if(prevSecVal != start.tv_sec){
						prevSecVal = start.tv_sec;
						testStartTime++;
						//printf("\n__%d__\n", testStartTime);
					}

					if(testStartTime > CO_PORT_OPEN_TIMEOUT_SEC+1){
						//printf("/**********************/\nRetry Test..! No Responce From Sensor resetted the Daemon....!\n/**********************/\n");
						break;
					}
					read(dum_fd, &co_start, sizeof(co_start));
					//nu_SerialComPort_BlockRead(fd, &co_start, sizeof(co_start));
					//printf("-%d%c-",fd, co_start);
					if(co_start == 'A'){
						//this is the CO sensor port**************************
						printf("CO sensor is connected to %s discriptor=%d\n ",SerialPortName[i], dum_fd);
						strcpy(CO_PORT_USB, SerialPortName[i]);
						CO_fd = dum_fd;
						//SerialPortStatus = 1;
				        break;
					}
				}
				if(CO_fd > 0) {
					break; //break;
				}
			}
		}
	    i++;
    }


	printf("--%d--\n", CO_fd);
	/********************Open RFID connected serial port*********************/
	i = 0; RFID_fd = -1;
    while(i<20){
		printf("%s\n", SerialPortName[i]);
		if ( (! strcmp(SerialPortName[i], MODBUS_USB)) || (! strcmp(SerialPortName[i], CO_PORT_USB))) {
		} else {
			RFID_fd = ArgOpenSerialPort(SerialPortName[i], 9600);
			if(RFID_fd < 0) {
				printf("Alert: reconnect the RFID port USB and restart the daemon \n");
			//return 0;
			} else {
				break;
			}
		}
		i++;
	}
	if(RFID_fd > 0){
		printf("RFID is connected to %s discriptor=%d\n ",SerialPortName[i], RFID_fd);
	}
	//================**oo**oo**=============================================
	
	#endif

/*
		fd = ArgOpenSerialPort("/dev/ttyUSB0", 9600);
		if(fd < 0) {
			//printf("\nNOTE: reconnect the ttyUSB2 ( CO- sensor USB)----");
			printf("\n-- OR -- Connect co-processor USB 1st, RFID USB 2nd & CO-sensor 3rd..\n");
		} else {
			printf("\n---------------- The Co-Processor is connected--------------\n");
			SerialPortStatus = 1;
		}	


		RFID_fd = ArgOpenSerialPort("/dev/ttyUSB1", 9600);
		if(RFID_fd < 0) {
			//printf("\nNOTE: reconnect the ttyUSB2 ( CO- sensor USB)----");
			printf("\n-- OR -- Connect co-processor USB 1st, RFID USB 2nd & CO-sensor 3rd..\n");
		} else {
			printf("\n----------------RFID is connected to USB1--------------\n");
		}
    int i=0;
    while(i<20){
      	fd = nu_SerialComPort_Open(SerialPortName[i]);
	    if(fd < 0) {
        }
	    else{
 	        printf("Able to open the SerialPort %s \n ",SerialPortName[i]);
  	        SerialPortStatus = 1;
            break;
	    }
	    i++;
	    
    }

    if(SerialPortStatus != 0){
        baud = 9600;    //atoi(argv[2]);
    	if(nu_SerialComPort_Init(fd, baud) < 0 ){
    		printf("baud not set");
		    return 0;
    	} 
    }
    else{
            printf("Unable to open the SerialPort \n ");
   		    return ;
    }
    return 1;
*/
}

char openCO_port(){
	char co_start;
	int i;
/********************Open CO-sensor connected serial port*********************/
	prevSecVal= 100;
	testStartTime = 0;
    i=0;
	memset(CO_PORT_USB, 0, sizeof(CO_PORT_USB));
	CO_fd = -1;
    while(i<20){
		if (strcmp(SerialPortName[i], MODBUS_USB) ) {
			printf("\n-----%s----\n", SerialPortName[i]);
		  	fd = nu_SerialComPort_Open(SerialPortName[i]);
			if(fd < 0) {
//				printf("\nAlert: reconnect the CO sensor ( CO- sensor USB) and restart the daemon");
//				return 0;
		    }
			else{
				testStartTime = 0;
				while( 1 ){
					//printf("%c--",co_start);
					/*****************************
					check the time if it is more than 2 min then 
					1) clear the daemon input files
					2) give a msg to the UI that sensors are not responding
					3) change loopcase to 1
					*************************/
					gettimeofday(&start, NULL);
					if(prevSecVal != start.tv_sec){
						prevSecVal = start.tv_sec;
						testStartTime++;
						//printf("\n__%d__\n", testStartTime);
					}

					if(testStartTime > CO_PORT_OPEN_TIMEOUT_SEC+2){
						//printf("/**********************/\nRetry Test..! No Responce From Sensor resetted the Daemon....!\n/**********************/\n");
						break;
					}
					read(fd, &co_start, sizeof(co_start));
					//nu_SerialComPort_BlockRead(fd, &co_start, sizeof(co_start));
					//printf("-%d%c-",fd, co_start);
					if(co_start == 'A'){
						//this is the CO sensor port**************************
						printf("Able to open the SerialPort %s \n ",SerialPortName[i]);
						strcpy(CO_PORT_USB, SerialPortName[i]);
						CO_fd = fd;
						//SerialPortStatus = 1;
				        break;
					}
				}
			}
		}
		if(CO_fd > 0)
			break;
	    i++;
    }
	printf("--%d--", CO_fd);
}
void DisplayMessage(void){
 	printf("You are in Wellth System \n");
	printf("Enter 1 To Check Your BMI Level\n");
	printf("Enter 2 To Check Your Blood Pressure  \n");
	printf("Enter 3 To Check Your BLI Level\n");
}

float ConvertHexToFloat(void){
        float Result;
        signed int Bmi;
        long int Byte1=0,Byte2=0,Byte3=0,Byte4=0;
        
        if(BliTestEn){
          Byte1 = BmiBuf[1];
            Byte2  = BmiBuf[0];
            Byte2 = (Byte2<<8);
            Byte3 = BmiBuf[3];
            Byte3 = (Byte3<<16);
            Byte4= BmiBuf[2];
            Byte4  = (Byte4<<24);
        }        
        else
        {        
            Byte1 = BmiBuf[0];
            Byte2  = BmiBuf[1];
            Byte2 = (Byte2<<8);
            Byte3 = BmiBuf[2];
            Byte3 = (Byte3<<16);
            Byte4= BmiBuf[3];
            Byte4  = (Byte4<<24);
        }        
        Bmi = (Byte1| Byte2| Byte3|Byte4);    
        printf("The int value is %x \n",Bmi);    
        Result = *((float *)&Bmi);     
        return Result;
}

void CheckTest(void){
    if(TestNo == 1){
         LoopCase = 2;    
        printf("Demon Code has received the Hight = ");
        fp = fopen (IN_ENTERY_FILE, "r");
        if(fp<0){
            printf(" System is not able to open file \n");
        }
        else{
            fread(&Din,sizeof(Din),1,fp);
            fclose(fp);
        }
        sscanf(Din.Hight,"%d",&Hight);
        //memset(Din.Hight,0,10);
        printf("%d\n",Hight);
        printf("\n");     
        printf("BMI Test Starts Now \n");             
    }
    else if(TestNo == 2){
        printf("Bp Test Starts Now \n");
        LoopCase = 6;    
    }
    else if(TestNo == 3){
        printf("BLI Test Starts now \n");      
        printf("Demon Code has received the Hight = ");
        fp = fopen (IN_ENTERY_FILE, "r");
        if(fp<0){
            printf(" System is not able to open file \n");
        }
        else{
            fread(&Din,sizeof(Din),1,fp);
            fclose(fp);
        }
        sscanf(Din.Weight,"%d",&TempWeight);
        sscanf(Din.Hight,"%d",&Hight);
        memset(Din.Weight,0,10);
        memset(Din.Hight,0,10);
        printf("The Hight Value %d\n",Hight);
        printf("%d\n",Hight);
        printf("The Weight Value %d\n",TempWeight);
        printf("BLI Test Starts now \n");      
        LoopCase = 9;  
        printf("\n");     
    }
	else if(TestNo == 4){
		LoopCase = 13;   
		printf("Co Test Starts now \n");
	}
    else{
        LoopCase = 1;    
    }
}

char Line[512];
char *DemonArrOfCol[8];
char *ArrayOfCol[32];

void CheckUserInfo(void){
	sqlite3	*db;
	char *zErrMsg = 0;
	int rc;
	char CharArr[12];
	char *endptr;
	long val;
	char timeBuf[64];
	FILE *fpt;

	memset(CurRFIDScaned, 0, 16);
	if(UserMode==1){
    	printf("Demon Code has received the User Login information\n ");
	    fp = fopen (IN_ENTERY_FILE, "r");
	    if(fp<0){
	        printf(" System is not able to open file \n");
	    }
	    else{
			if(GetLine(fp,Line)){
				//printf("size of array of string %d", sizeof(DemonArrOfCol));
				//printf("size of array of column %d", sizeof(ArrayOfCol));
				CreateArrayOfString(DemonArrOfCol,Line);					
			}	
	        fclose(fp);
	    }
	   	Uin.Name = DemonArrOfCol[0];
		Uin.Password  = DemonArrOfCol[1];
		Uin.Gender  = DemonArrOfCol[2];
		printf("The User Name %s\n",Uin.Name);
		printf("The User Password %s\n",Uin.Password);
		printf("The User Gender %s\n",Uin.Gender);

		val = strtol(Uin.Name,&endptr,10);
		Current_MobileNo = val;

		val = strtol(Uin.Password,&endptr,10);
		Current_Age = val;

		if ( strcmp(Uin.Gender,"Female") == 0)
			Current_Gender = 1;	//Female Gender
		else
			Current_Gender = 0;	//Male Gender
		
		// Database Logging of Login Information

		system("/bin/date > /home/time.c");
		fpt = fopen("/home/time.c", "r");
		while( GetLine(fpt, Line) != 0){
			strcpy(timeBuf, Line);
			//printf("%s\n", Line);
		}

		sprintf(query, "insert into login_t (mobilenumber, age, gender, log_time) values ('%s', '%s', '%s', '%s') ", Uin.Name, Uin.Password, Uin.Gender, timeBuf);
		// sprintf(query, "insert into login_t (mobilenumber, age, gender, log_time) values ('%l', '%d', '%s', '%s') ", Current_MobileNo, Current_Age, Uin.Gender, timeBuf);

		if (sqlite3_open(DAEMON_DB_FILE, &db) != SQLITE_OK)
		{
			fprintf(stderr, "Can't open database: \n");
			sqlite3_close(db);
			exit(1);
		}
		rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		sqlite3_close(db);

		printf("DataBase Test Starts now \n");          
		CompareUserInfo();
	}
	else if (UserMode == 2){
		//if(GetLine(fp,Line)){
			//memset(CurRFIDScaned, 0, sizeof(CurRFIDScaned));
			read(RFID_fd, CurRFIDScaned, sizeof (CurRFIDScaned));
			if(CurRFIDScaned[0] != 0) {
				nu_SerialComPort_BlockRead(RFID_fd, CurRFIDScaned, 16);
				printf("%s",CurRFIDScaned);
				//printf("%s",CurRFIDScaned);
				////printf("%s",CurRFIDScaned);
				if(CurRFIDScaned[0] != 0){
					CompareUserInfo();
				}
			}
		//}
	}
}

void CompareUserInfo(void){
		FILE* fp1, *fpUI;
        char Buf,NoOfCommas =0,Index=0;
        char TempUserName[20], ch;
		unsigned char UNMatched = 0, PWDMatched = 0, RFIDMatched = 0;
		unsigned char UNColNum = 11;
		unsigned char PWDColNum = 12;  
		unsigned char NameColNum = 8;
		unsigned char AgeColNum = 13;
		unsigned char RFIDColNum = 14;
		unsigned char GenderColNum = 4;
		char *ptr_CurRFIDScaned, *ptr_nxtline;
		printf("compairing user info");
		if(CurRFIDScaned[0] != 0){
			ptr_CurRFIDScaned = strstr(CurRFIDScaned,"280");
			ptr_nxtline = strchr(CurRFIDScaned, '\n');
			*ptr_nxtline = '\0';
			printf("\n-------------%s", CurRFIDScaned);
		}
        printf("\nDemon Code has started the comparison of  Login information\n ");
        UNMatched = 0; PWDMatched = 0; RFIDMatched = 0;
        fp1 = fopen (USER_INFO_FILE, "r");
        if(fp1<0){
            printf(" System is not able to open file \n");
        }
        else{
			while(1){
				if(GetLine(fp1,Line)){
					printf("----> Line: %s\n", Line);
					CreateArrayOfString(ArrayOfCol,Line);
					if(UserMode == 1){
						if (strcmp(ArrayOfCol[UNColNum-1],Uin.Name) == 0) {
							UNMatched = 1;
							if(strcmp(ArrayOfCol[PWDColNum-1],Uin.Password) == 0){
								PWDMatched = 1;
							}
						}
						if(UNMatched == 1 && PWDMatched == 1) {
							strcpy(login.LoginReply, "LOGIN_SUCCESS");
							strcpy(login.Name, ArrayOfCol[NameColNum-1]);
							strcpy(login.Age, ArrayOfCol[AgeColNum-1]);
							strcpy(login.Gender, ArrayOfCol[GenderColNum-1]);
							break;
						} else {
							//printf("--wrong user name / PWD---- \n");
							strcpy(login.LoginReply, "LOGIN_FAILED");
							strcpy(login.Name, "NULL");
							strcpy(login.Age, "NULL");
							strcpy(login.Gender, "NULL");
						}
					}
					if(UserMode == 2){
					printf("\ncompairing\n");
						//if(strcmp(ArrayOfCol[RFIDColNum-1], CurRFIDScaned) == 0){
						printf("\n--%s-- == --%s--\n", ArrayOfCol[RFIDColNum-1], ptr_CurRFIDScaned);
						if(strcmp(ArrayOfCol[RFIDColNum-1], ptr_CurRFIDScaned) == 0) {
							RFIDMatched = 1;
						}
						printf("\nafter compairing\n");
						if (RFIDMatched == 1){ 
							strcpy(login.LoginReply, "LOGIN_SUCCESS");
							strcpy(login.Name, ArrayOfCol[NameColNum-1]);
							strcpy(login.Age, ArrayOfCol[AgeColNum-1]);
							strcpy(login.Gender, ArrayOfCol[GenderColNum-1]);
							break;
						}
					}

				} else {
					break;
				}
			}
			fclose(fp1); // close USER_INFO_FILE
			
			if(UserMode == 2){
				if( RFIDMatched == 0 ) {
					memset(CurRFIDScaned, 0, sizeof(CurRFIDScaned));
					return;
				}
			}
			
            fp1 = fopen (OT_ENTERY_FILE, "w");
            if(fp1<0){
                printf(" System is not able to open file \n");
            }
            else{
                fwrite(login.LoginReply, strlen(login.LoginReply), 1, fp1);
				ch = ',';
				fwrite(&ch, 1, 1, fp1);
				fwrite(login.Name, strlen(login.Name), 1, fp1);
				fwrite(&ch, 1, 1, fp1);
				fwrite(login.Age, strlen(login.Age), 1, fp1);
				fwrite(&ch, 1, 1, fp1);
				fwrite(login.Gender, strlen(login.Gender), 1, fp1);
				
				fpUI = fopen(LOGIN_SUMMARY_FILE, "w");
				if(fp1<0){
                	printf(" System is not able to open file \n");
	            } else {
					fwrite(login.Name, strlen(login.Name), 1, fpUI);
					fwrite(&ch, 1, 1, fpUI);
					fwrite(login.Age, strlen(login.Age), 1, fpUI);
					fwrite(&ch, 1, 1, fpUI);
					fwrite(login.Gender, strlen(login.Gender), 1, fpUI);
					fclose(fpUI);
				}
				
				//ch = '\0';
				//fwrite(&ch, 1, 1, fp1);
                //fclose(fp);
            }
            fclose(fp1); // close OT_ENTERY_FILE

			UserMode= 0;
			Buf = '1';
            fp1 = fopen (OT_TEXT_FILE, "w");
            if(fp1<0){
                printf(" System is not able to open file \n");
            }
            else{
                fwrite(&Buf,1,1,fp1);
            }
			fclose(fp1); // close OT_TEXT_FILE

			FreeArrayOfString(DemonArrOfCol);
			FreeArrayOfString(ArrayOfCol);
			ClearDemonInputFile();
        }
}

void ClearDemonInputFile(){
	FILE *fp1;
	fp1 = fopen (IN_TEXT_FILE, "w");
	fclose(fp1);
	fp1 = fopen (IN_ENTERY_FILE, "w");
	fclose(fp1);
}
