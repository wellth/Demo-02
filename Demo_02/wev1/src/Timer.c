#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include <string.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"

extern void QuitBmiWindow(void);
extern void GetBmiOutput(void);
extern void QuitBliWindow(void);
extern void GetBliOutput(void);
extern void QuitBpWindow(void);
extern void GetBpOutput(void);
extern void GetCoOutput();
void TimerInit(void);
gboolean UpdateTime(void);

gboolean isWeCheckTestScreen();
static gboolean _update_time();
static void _go_timer_start();


volatile char StartTimer = 0;
int SessionExpired, Seconds, Minutes;
extern unsigned char BLITestEn,BPTestEn,BMITestEn, COTestEn;
extern GtkWidget *window,*Bliwindow,*Bpwindow,*Bmiwindow;
extern GtkBuilder *BliBuilder, *BmiBuilder, *BpBuilder;
extern GtkWidget *MovingButton;
GtkWidget *TimerLabel , *login_status, *login_usrnm, *login_password;

/************************************************************************************************************************************
*************************************************************************************************************************************/
char TimeBuf[64];
gboolean UpdateTime(void){
    //char buf[256];

	printf("star timer %d, bp=%d, bli=%d, bmi=%d\n", StartTimer, BPTestEn, BLITestEn, BMITestEn);
	if(StartTimer){

		if (isWeCheckTestScreen())
		{
			Seconds++;
			//memset(&buf, 0x0, 256);
			//snprintf(buf, 255, "Time %d", ++SessionExpired);    
			Seconds = ++SessionExpired;
			if(Seconds%59==0){
				Seconds = 0;
				SessionExpired = 0;
				Minutes++;
			}
			sprintf(TimeBuf, "%d M : %d S",Minutes,Seconds);
			//gtk_entry_set_text(entry4,&TimeBuf);
			gtk_label_set_label (TimerLabel, &TimeBuf);
			//printf(buf);
			printf("\n");
			if(SessionExpired%5 == 0){
				printf("Check The Response\n");
			    if(BMITestEn == 1){
					GetBmiOutput();
				}
				else if(BPTestEn == 1){
					printf("get output --its a wellth check\n");
					GetBpOutput();
				}
				else if(BLITestEn == 1){
					if(wait_for_5sec){
						send_bf_measure_request();
						wait_for_5sec = 0;
						StartTimer = 1;
					} else {
						GetBliOutput();
					}
				}
				else if(COTestEn == 1){
					GetCoOutput();
				}
			}
			if (Minutes >=3){
				SessionExpired = 0;
				Minutes = 0;
				StartTimer = 0;
				if(BMITestEn == 1){
					hide_measure_btn_show_repeate_btn();
					test1_button_set_state(TRUE);
					BMITestEn = 0;
					//QuitBmiWindow();
					return 0;
				}
				else if(BPTestEn == 1){
					hide_measure_btn_show_repeate_btn();
					test4_button_set_state(TRUE);
					enable_test4_repeate();
					BPTestEn = 0;
					//QuitBpWindow();
					return 0;
				}
				else if(BLITestEn == 1){
					hide_measure_btn_show_repeate_btn();
					test2_button_set_state(TRUE);
					BLITestEn = 0;
					//QuitBliWindow();
					return 0;
				}
				else if(COTestEn == 1){
					hide_measure_btn_show_repeate_btn();
					test3_button_set_state(TRUE);
					enable_test3_repeate();
					COTestEn = 0;
					//QuitBliWindow();
					return 0;
				}
			}
		} else {
			if( isOutputFlagSet() )
			{
				switch (cur_active_screen){
					case initial_scrn : 
					case login_scrn : 
							g_print("entered in the timer login");
							demon_return_status = ReadDemonLoginStatus();
							ClearOutPutFile ();
							if(demon_return_status == LOGIN_SUCCESS){
								hide_window_login();
								hide_window_second ();
								show_window_mand_msg ();
								_go_timer_start();
								sec_expired = 0;
								start_timer = 1;
								continue_timer = TRUE;
								login_password = GTK_WIDGET (gtk_builder_get_object(LoginBuilder, "password"));
								gtk_entry_set_text(login_password, "");
								login_usrnm = GTK_WIDGET (gtk_builder_get_object(LoginBuilder, "company_mail"));
								gtk_entry_set_text(login_usrnm,"");
								login_status = GTK_WIDGET (gtk_builder_get_object(LoginBuilder, "login_status"));
								gtk_label_set_label (login_status, "");
							} else {
								SessionExpired = 0;
								Minutes = 0;
								Seconds = 0;
								StartTimer = 0;
								login_status = GTK_WIDGET (gtk_builder_get_object(LoginBuilder, "login_status"));
								gtk_label_set_label (login_status, "Invalid User name or Password");
								login_password = GTK_WIDGET (gtk_builder_get_object(LoginBuilder, "password"));
								gtk_entry_set_text(login_password,"");
							}
						break;
					default :
						break;
				}
			}
		}
	}
	else{
		SessionExpired = 0;
		Minutes = 0;
		Seconds = 0;
		StartTimer = 0;
		printf("Timer is off\n");
		sprintf(TimeBuf, "%d M : %d S",Minutes,Seconds);
		switch (cur_active_screen){
			case test1_scrn :
				TimerLabel = GTK_WIDGET (gtk_builder_get_object(Test1Builder, "timer_label"));
				gtk_label_set_label (TimerLabel, TimeBuf);
				BMITestEn = 0;
				test1_button_set_state(TRUE);
				hide_measure_btn_show_repeate_btn();
				break;
			case test2_scrn :
				TimerLabel = GTK_WIDGET (gtk_builder_get_object(Test2Builder, "timer_label"));
				gtk_label_set_label (TimerLabel, TimeBuf);
				test2_button_set_state(TRUE);
				hide_measure_btn_show_repeate_btn();
				BLITestEn= 0; 
				break;
			case test3_scrn :
				TimerLabel = GTK_WIDGET (gtk_builder_get_object(Test3Builder, "timer_label"));
				gtk_label_set_label (TimerLabel, TimeBuf);
				test3_button_set_state(TRUE);
				hide_measure_btn_show_repeate_btn();
				COTestEn = 0;
				break;
			case test4_scrn :
				TimerLabel = GTK_WIDGET (gtk_builder_get_object(Test4Builder, "timer_label"));
				gtk_label_set_label (TimerLabel, TimeBuf);
				test4_button_set_state(TRUE);
				hide_measure_btn_show_repeate_btn();
				BPTestEn = 0;
				break;
			default:
				break;
		}
	}
	return StartTimer;			
}

void TimerInit(void){
	//StartTimer = 1;
	printf("timer wnter\n");
	g_timeout_add_seconds(1, UpdateTime, NULL);
	printf("timer intris\n");
	switch (cur_active_screen){
		case test1_scrn :
			TimerLabel = GTK_WIDGET (gtk_builder_get_object(Test1Builder, "timer_label"));
			break;
		case test2_scrn :
			TimerLabel = GTK_WIDGET (gtk_builder_get_object(Test2Builder, "timer_label"));
			break;
		case test3_scrn :
			TimerLabel = GTK_WIDGET (gtk_builder_get_object(Test3Builder, "timer_label"));
			break;
		case test4_scrn :
			TimerLabel = GTK_WIDGET (gtk_builder_get_object(Test4Builder, "timer_label"));
			break;
	}
	printf("timer intris\n");
	SessionExpired = 0;
}

gboolean isWeCheckTestScreen(){
	gboolean ret;
	switch (cur_active_screen){
		case test1_scrn :
		case test2_scrn :
		case test3_scrn :
		case test4_scrn :
			ret = TRUE;
			break;
		default:
			ret = FALSE;
			break;
	}
	return ret;
}


static gboolean _update_time(){
	if(start_timer) { 
		++sec_expired;
	}
	if (sec_expired > 2) {
		sec_expired = 0;
		start_timer = 0;
		hide_window_mand_msg();
		show_window_home();
		continue_timer = FALSE;
		StartTimer = 0;
	}
	return continue_timer;
}

static void _go_timer_start()
{
	
   //if(!start_timer)
    {
        //g_timeout_add_seconds(1, _label_update, label);
		g_timeout_add_seconds(1, _update_time, NULL);
        start_timer = TRUE;
        continue_timer = TRUE;
    }
}
static gboolean _count_smoke_test_timeout()
{
	enable_test3_repeate ();
	return FALSE;
}

void smoke_test_wait_start()
{
	g_timeout_add_seconds (30 , _count_smoke_test_timeout, NULL);
	//smoke_test_wait_timer_start = 1;
}

static gboolean _count_bp_test_timeout()
{
	enable_test4_repeate ();
	return FALSE;
}

void bp_test_wait_start()
{
	g_timeout_add_seconds (2, _count_bp_test_timeout, NULL);
	//smoke_test_wait_timer_start = 1;
}

