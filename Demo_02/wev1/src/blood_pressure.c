#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include <string.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"

/*****************************************************************************************************************
                                                        Variable Declaration
******************************************************************************************************************/
_BP_IN_FILE Indata2;
_BP_TEST Bp;

GtkWidget *label_sys_bp, *label_dia_bp, *label_pulse_rate, *label_sys_msg;
static GtkWidget *button1, *button2, *button3, *button4, *button5;
GtkWidget *t4_normal_smsg1, *t4_normal_smsg2, *t4_normal_smsg3, *t4_normal_smsg4;
GtkWidget *t4_result_smsg1, *t4_result_smsg2, *t4_result_smsg3, *t4_result_smsg4;
GtkWidget *t4_normal_smsg5, *t4_normal_smsg6, *t4_normal_smsg7, *t4_normal_smsg8;
GtkWidget *t4_result_smsg5, *t4_result_smsg6, *t4_result_smsg7, *t4_result_smsg8;
unsigned char BPTestEn = 0;
extern GtkWidget *window,*Bliwindow,*Bpwindow,*Bmiwindow;

static int init_test4_window = 0, init_test4_b_window = 0;
GtkBuilder *Test4Builder, *Test4bBuilder;
/**************************Variable Declaration******************************/


/*****************************************************************************************************************
                                                       Button click message handler
******************************************************************************************************************/
void test4_home_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_test4();
	show_window_home();
	start_timer = 0;
}

void test4_button_set_state(int bool_val){
	gtk_widget_set_sensitive(button1, bool_val);
	gtk_widget_set_sensitive(button2, bool_val);
	gtk_widget_set_sensitive(button3, bool_val);
	//gtk_widget_set_sensitive(button4, bool_val);
	if(bool_val == FALSE)
		disable_test4_repeate();
	
	gtk_widget_set_sensitive(button5, bool_val);

	GtkWidget *feed_back;
	if(bool_val == TRUE){
		feed_back = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "label9"));
		gtk_label_set_label (feed_back, "Please wait upto sensor is ready");
	}
}

void enable_test4_repeate(){
	GtkWidget *feed_back;
	gtk_widget_set_sensitive(button4, TRUE);
	feed_back = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "label9"));
	gtk_label_set_label (feed_back, "Click to measure");
}
void disable_test4_repeate(){
	GtkWidget *feed_back;
	gtk_widget_set_sensitive(button4, FALSE);
	//feed_back = GTK_WIDGET (gtk_builder_get_object (Test3Builder, "label9"));
	//gtk_label_set_label (feed_back, "Click to measure");
}

void reset_test4_results() {
	gtk_label_set_label (label_sys_bp, "**mmHg");
	gtk_label_set_label (label_dia_bp, "**mmHg");
	gtk_label_set_label (label_pulse_rate, "**");
//	gtk_label_set_label (label_sys_msg, "System message");

	gtk_widget_show (t4_normal_smsg1);
	gtk_widget_show (t4_normal_smsg2);
	gtk_widget_show (t4_normal_smsg3);
	gtk_widget_show (t4_normal_smsg4);
	gtk_widget_hide (t4_result_smsg1);
	gtk_widget_hide (t4_result_smsg2);
	gtk_widget_hide (t4_result_smsg3);
	gtk_widget_hide (t4_result_smsg4);

	gtk_widget_show (t4_normal_smsg5);
	gtk_widget_show (t4_normal_smsg6);
	gtk_widget_show (t4_normal_smsg7);
	gtk_widget_show (t4_normal_smsg8);
	gtk_widget_hide (t4_result_smsg5);
	gtk_widget_hide (t4_result_smsg6);
	gtk_widget_hide (t4_result_smsg7);
	gtk_widget_hide (t4_result_smsg8);
}
void test4_b_next_btnclicked (GtkWidget *widget, gpointer user_data) {
	reset_test4_results();
	hide_window_test4_b();
	//show_window_test1();
	show_window_qna();
}

void test4_next_btnclicked(GtkWidget *widget, gpointer user_data) {
	reset_test4_results();
	hide_window_test4();
	show_window_test4_b();
	//show_window_test1();
	//show_window_qna();
	//show_window_user_summary();
	start_timer = 0;
	BPTestEn = 0;
	//printf("---------------------completed---------------");
	//set_cur_test = CUR_TEST2;
	//show_measure_btn_hide_repeate_btn();
}

void ClearOutPutFile(void){
	FILE *fp;
	fp = fopen (OT_ENTERY_FILE, "w");
	fclose(fp);
	fp = fopen (OT_TEXT_FILE, "w");
	fclose(fp);	
	StartTimer = FALSE;
}

void test4_click_to_measure_btnclicked(GtkWidget *widget, gpointer user_data)
{
	FILE *fp;
	long int Delay = 0xFFFFFFFF;
	//start_timer = 1;
	GtkWidget *feed_back;
	reset_test4_results();
	
	feed_back = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "label9"));
	gtk_label_set_label (feed_back, "Please wait..! Getting SBP, DBP & PR");

	
	fp = fopen (IN_TEXT_FILE , "w");
	if(fp<0){
		printf(" System is not able to create file \n");
	}
	else{
		printf("ip is ready \n");
		ClearOutPutFile();
		strcpy(Indata2.InBuf,"BpTest");
		fwrite(&Indata2,sizeof(Indata2),1,fp);
		printf(" File has been Updated \n");
		fclose(fp);		
		BPTestEn=1;	
	}
	/*
	while(BPTestEn==1){
		while(Delay>0){
			Delay--;
		}
		GetBpOutput();
		Delay = 0xFFFFFFFF;
	}
*/
	test4_button_set_state(FALSE);
	Minutes = 0;
	Seconds = 0;
	BPTestEn=1;	
	StartTimer = 1;
	TimerInit();
	return;
}
void test4_set_btnclicked(GtkWidget *widget, gpointer user_data)
{

}
void test4_hide_next_button_n_image(){
	GtkWidget *button;GtkImage *nextImage;
	button = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "next"));
	nextImage = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "image3"));
	gtk_widget_hide(button);
	gtk_widget_hide(nextImage);
}
/***********************Button click message handler**********************************************************************/

static GtkWidget*
create_test4_window (void)
{
	GtkWidget *window; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG, *imageTestStat;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (Test4Builder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (Test4Builder, TEST4_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TEST4_WINDOW,
				TEST4_UI_FILE);
        }

	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4Builder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4Builder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

//	GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4Builder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

	GdkPixbufAnimation * pixbufTestanm = gdk_pixbuf_animation_new_from_file(TEST4, error);
	imageTest = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4Builder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTest), pixbufTestanm);

	
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4Builder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);
	
	GdkPixbufAnimation * pixbuf1 = gdk_pixbuf_animation_new_from_file(GRAPH, error);
	//GtkImage * grf = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4Builder, "graph"));
	//gtk_image_set_from_animation(GTK_IMAGE(grf), pixbuf1);
	
	button1 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);

	profileBtnClickedFrom = test4_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);

	button4 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "repeate"));
	g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (test4_click_to_measure_btnclicked),
                    NULL);
	
	GtkWidget *timer_label = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "timer_label"));
	button3 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "click_to_measure"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (test4_click_to_measure_btnclicked),
                    NULL);
	continue_timer = TRUE;
    start_timer = TRUE;
	//g_signal_connect(G_OBJECT(button3), "clicked", G_CALLBACK(_start_timer), timer_label);
	//g_signal_connect(G_OBJECT(button31), "clicked", G_CALLBACK(_start_timer), timer_label);

	label_sys_bp = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "label10"));
	label_dia_bp = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "label11"));
	label_pulse_rate = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "label5"));
	//label_sys_msg = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "label6"));

	t4_normal_smsg1 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_normal_smsg1"));
	t4_normal_smsg2 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_normal_smsg2"));
	t4_normal_smsg3 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_normal_smsg3"));
	t4_normal_smsg4 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_normal_smsg4"));
	t4_normal_smsg5 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_normal_smsg5"));
	t4_normal_smsg6 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_normal_smsg6"));
	t4_normal_smsg7 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_normal_smsg7"));
	t4_normal_smsg8 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_normal_smsg8"));
	t4_result_smsg1 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_result_smsg1"));
	t4_result_smsg2 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_result_smsg2"));
	t4_result_smsg3 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_result_smsg3"));
	t4_result_smsg4 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_result_smsg4"));
	t4_result_smsg5 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_result_smsg5"));
	t4_result_smsg6 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_result_smsg6"));
	t4_result_smsg7 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_result_smsg7"));
	t4_result_smsg8 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "t4_result_smsg8"));
	
	/*
	button4 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "set"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (test4_set_btnclicked),
                    NULL);
	*/
	button5 = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "next"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test4_next_btnclicked),
                    NULL);
	/* 
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test4_home_btnclicked),
                    NULL);
	*/
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_test4()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_test4_window) {
		   GError *error=NULL;
		Test4Builder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (Test4Builder, TEST4_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_test4_window();
		init_test4_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (Test4Builder, TEST4_WINDOW));
	reset_test4_results();
	gtk_widget_show_all(window);
	cur_active_screen = test4_scrn;
}

void hide_window_test4()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (Test4Builder, TEST4_WINDOW));
	gtk_widget_hide(window);
}

static GtkWidget*
create_test4_b_window (void)
{
	GtkWidget *window; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG, *add;
	GtkWidget *timer_label;

	GtkWidget *vbox1;
	GError** error = NULL;
	int i=0;
	char buf[8];
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (Test4bBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, TEST4_b_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TEST4_b_WINDOW,
				TEST4B_UI_FILE);
        }
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
/*
	GdkPixbufAnimation * pixbufTestanm = gdk_pixbuf_animation_new_from_file(TEST1, error);
	imageTest = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTest), pixbufTestanm);
*/
	
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
	
//#endif
	
	//GdkPixbufAnimation * pixbuf1 = gdk_pixbuf_animation_new_from_file(GRAPH, error);
	//GtkImage * grf = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "graph"));
	//gtk_image_set_from_animation(GTK_IMAGE(grf), pixbuf1);

	GdkPixbufAnimation * pixbufStat1anm = gdk_pixbuf_animation_new_from_file(QNA_ICON, error);
	GtkImage* imageTestStat = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, "next_icon"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTestStat), pixbufStat1anm);
	/*
	GdkPixbufAnimation * pixbuf2 = gdk_pixbuf_animation_new_from_file(T_NEXT_ICON, error);
	GtkImage * nxt_icon = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, "next_icon"));
	gtk_image_set_from_animation(GTK_IMAGE(nxt_icon), pixbuf2);
	*/
	button1 = GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);
	
	button5 = GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, "next"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test4_b_next_btnclicked),
                    NULL);

	
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_test4_b()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_test4_b_window) {
		   GError *error=NULL;
		Test4bBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (Test4bBuilder, TEST4B_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_test4_b_window();
		init_test4_b_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, TEST4_b_WINDOW));
	gtk_widget_show_all(window);
	cur_active_screen = test4_b_scrn;
}

void hide_window_test4_b()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (Test4bBuilder, TEST4_b_WINDOW));
	gtk_widget_hide(window);
}



void GetBpOutput(void){
	GtkWidget *window;
	FILE *fp;
	char Buf[2];
	char buf1[40], buf2[40], buf3[40];
	char BP_test_msg[128];
	
	if(BPTestEn==1){
		fp = fopen (OT_TEXT_FILE, "r+");
		if(fp<0){
			printf(" System is not able to create file \n");
		}
		else{
			fread(&Buf,1,1,fp);
			fclose(fp);	
			
			if(Buf[0] == '1')
			{
				printf("Check the output\n");
				printf(" String info %d \n",Buf[0]);
				printf("System Got Data Updated \n");
				fp = fopen (OT_ENTERY_FILE, "r");
				fread(&Bp,sizeof(Bp),1,fp);
				fclose(fp);
				printf("BP Systolic BP  is %d\n",Bp.SysP[0]);
				printf("BP Diastolic BP is %d\n",Bp.DiaP[0]);
    			printf("BP Pulse Rate  is %d\n",Bp.PulseRate[0]);

				sprintf(buf1, "%s mmHg", &Bp.SysP);
				sprintf(buf2, "%s mmHg", &Bp.DiaP);
				sprintf(buf3, "%s", &Bp.PulseRate);

				gtk_label_set_label (label_sys_bp, buf1);
				gtk_label_set_label (label_dia_bp, buf2);
				gtk_label_set_label (label_pulse_rate, buf3);
				BPTestEn=0;
				Minutes = 0;
				Seconds = 0;
				ClearOutPutFile();
				
				ReadBPForSummary ();
				g_print("reading message-----\n");
				//UserSummaryData.SysBP = 140.0;
				//UserSummaryData.DiaBp = 90.0;
				blood_pressure_system_msg(UserSummaryData.SysBP, UserSummaryData.DiaBp, UserSummaryData.PulseRate, BP_test_msg);
				if( strcmp(BP_test_msg, "Normal") == 0 ){
					gtk_widget_show (t4_result_smsg1);
					gtk_widget_show (t4_result_smsg5);
					gtk_widget_hide (t4_normal_smsg1);					
					gtk_widget_hide (t4_normal_smsg5);
				}
				if( strcmp(BP_test_msg, "Pre-hypertension") == 0 ){
					gtk_widget_show (t4_result_smsg2);
					gtk_widget_show (t4_result_smsg6);
					gtk_widget_hide (t4_normal_smsg2);					
					gtk_widget_hide (t4_normal_smsg6);
				}
				if( strcmp(BP_test_msg, "Hypertension") == 0 ){
					gtk_widget_show (t4_result_smsg3);
					gtk_widget_show (t4_result_smsg7);
					gtk_widget_hide (t4_normal_smsg3);					
					gtk_widget_hide (t4_normal_smsg7);
				}
				if( strcmp(BP_test_msg, "Emergency Care required") == 0 ){
					gtk_widget_show (t4_result_smsg4);
					gtk_widget_show (t4_result_smsg8);
					gtk_widget_hide (t4_normal_smsg4);					
					gtk_widget_hide (t4_normal_smsg8);
				}
				g_print("msg has been read\n");
				//gtk_label_set_label (label_sys_msg, BP_test_msg);

				test4_button_set_state(TRUE);
				bp_test_wait_start();
				
				window = GTK_WIDGET (gtk_builder_get_object (Test4Builder, TEST4_WINDOW));
				gtk_widget_show_all(window);
				if(isQuickWellthCheck)
					test4_hide_next_button_n_image();
			}			
		}
	}
}

