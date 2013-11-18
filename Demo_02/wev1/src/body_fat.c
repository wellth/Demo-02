#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include <string.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"


/*****************************************************************************************************************
                                                        Variable Declaration
******************************************************************************************************************/
_BLI_IN_FILE Indata;
_BLI_TEST Bli;
_BLI_INPUT_DATA Din;

GtkWidget *bodyFatLabel,*entry1,*entry2, *test2_msg;
static GtkWidget *button1, *button2, *button3, *button4, *button5;
extern GtkWidget *window,*Bliwindow,*Bpwindow,*Bmiwindow;
extern GtkBuilder *BliBuilder, *BmiBuilder, *BpBuilder;
unsigned char BLITestEn = 0;

static int init_test2_window = 0, init_test2_b_window = 0;
GtkBuilder *Test2Builder, *Test2bBuilder;

void reset_test2_results() {
	gtk_label_set_label (bodyFatLabel, "**%");
	//gtk_label_set_label (test2_msg, "System message");
}

void test2_button_set_state(int bool_val){
	gtk_widget_set_sensitive(button1, bool_val);
	gtk_widget_set_sensitive(button2, bool_val);
	gtk_widget_set_sensitive(button3, bool_val);
	gtk_widget_set_sensitive(button4, bool_val);
	gtk_widget_set_sensitive(button5, bool_val);

	GtkWidget *feed_back;
	if(bool_val == TRUE){
		feed_back = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "label9"));
		gtk_label_set_label (feed_back, "Click to measure");
	}
}

void test2_b_next_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_test2_b();
	start_timer = 0;
	show_window_test3();
	set_cur_test = CUR_TEST3;
	show_measure_btn_hide_repeate_btn();
	
}

void test2_next_btnclicked(GtkWidget *widget, gpointer user_data)
{
	reset_test2_results();
	hide_window_test2();
	start_timer = 0;
	show_window_test2_b();
	//set_cur_test = CUR_TEST3;
	//show_measure_btn_hide_repeate_btn();
	
}
void test2_click_to_measure_btnclicked(GtkWidget *widget, gpointer user_data)
{
	
	long int Delay = 0xFFFFFFFF;
	start_timer = 1;
	wait_for_5sec = 1;

		GtkWidget *feed_back;
	feed_back = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "label9"));
	gtk_label_set_label (feed_back, "Please hold the handles");
	
	/*
	while(BLITestEn==1){
		while(Delay>0){
			Delay--;
		}
		GetBliOutput();
		Delay = 0xFFFFFFFF;
	}
	*/
	BLITestEn=1;
	test2_button_set_state(FALSE);
	Minutes = 0;
	Seconds = 0;
	StartTimer = 1;
	TimerInit();
	return;
}

void send_bf_measure_request(){
	FILE *fp;
	GtkWidget *feed_back;
	feed_back = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "label9"));
	gtk_label_set_label (feed_back, "Please wait..! Getting your body fat");
	
	fp = fopen (IN_TEXT_FILE , "w");
	if(fp<0){
		printf(" System is not able to create file \n");
	}
	else{
		printf("ip is ready for body fat. \n");
		ClearOutPutFile();
		GetWindowEntry();
		strcpy(Indata.InBuf,"BliTest");
		fwrite(&Indata,sizeof(Indata),1,fp);
		printf(" File has been Updated \n");
		fclose(fp);			
	}
}

void test2_set_btnclicked(GtkWidget *widget, gpointer user_data)
{
}
void test2_hide_next_button_n_image(){
	GtkWidget *button;GtkImage *nextImage;
	button = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "next"));
	nextImage = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "next_icon"));
	gtk_widget_hide(button);
	gtk_widget_hide(nextImage);
}

static GtkWidget*
create_test2_window (void)
{
	GtkWidget *window;
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;
	GtkWidget *timer_label;
		
	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (Test2Builder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (Test2Builder, TEST2_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TEST2_WINDOW,
				TEST2_UI_FILE);
        }

	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2Builder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2Builder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2Builder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

	GdkPixbufAnimation * pixbufTestanm = gdk_pixbuf_animation_new_from_file(TEST2, error);
	imageTest = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2Builder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTest), pixbufTestanm);

	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2Builder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);

	//GdkPixbufAnimation * pixbuf1 = gdk_pixbuf_animation_new_from_file(GRAPH, error);
	//GtkImage * grf = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2Builder, "graph"));
	//gtk_image_set_from_animation(GTK_IMAGE(grf), pixbuf1);
	
	
	button1 = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);

	profileBtnClickedFrom = test2_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);
	bodyFatLabel = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "label5"));
	//test2_msg = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "label7"));
	
	button3 = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "click_to_measure"));

	button4 = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "repeate"));
		g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (test2_click_to_measure_btnclicked),
                    NULL);
	
	timer_label = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "timer_label"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (test2_click_to_measure_btnclicked),
                    NULL);
	//g_object_set_property (G_OBJECT(button3), "label", "Repeate");
	continue_timer = TRUE;
    start_timer = TRUE;

	//g_signal_connect(G_OBJECT(button3), "clicked", G_CALLBACK(_start_timer), timer_label);
	//g_signal_connect(G_OBJECT(button31), "clicked", G_CALLBACK(_start_timer), timer_label);
	/*
	button4 = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "set"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (test2_set_btnclicked),
                    NULL);
*/
	button5 = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "next"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test2_next_btnclicked),
                    NULL);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_test2()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_test2_window) {
		   GError *error=NULL;
		Test2Builder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (Test2Builder, TEST2_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_test2_window();
		init_test2_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (Test2Builder, TEST2_WINDOW));
	gtk_widget_show_all(window);
	cur_active_screen = test2_scrn;
}

void hide_window_test2()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (Test2Builder, TEST2_WINDOW));
	gtk_widget_hide(window);
}

static GtkWidget*
create_test2_b_window (void)
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
	gtk_builder_connect_signals (Test2bBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, TEST2_b_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TEST2_b_WINDOW,
				TEST2B_UI_FILE);
        }
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
//	GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
/*
	GdkPixbufAnimation * pixbufTestanm = gdk_pixbuf_animation_new_from_file(TEST1, error);
	imageTest = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTest), pixbufTestanm);
*/
	
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
	
//#endif
	
	//GdkPixbufAnimation * pixbuf1 = gdk_pixbuf_animation_new_from_file(GRAPH, error);
	//GtkImage * grf = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "graph"));
	//gtk_image_set_from_animation(GTK_IMAGE(grf), pixbuf1);
	
	/*GdkPixbufAnimation * pixbuf2 = gdk_pixbuf_animation_new_from_file(T1_NEXT_ICON, error);
	GtkImage * nxt_icon = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, "next_icon"));
	gtk_image_set_from_animation(GTK_IMAGE(nxt_icon), pixbuf2);
	*/
		GdkPixbufAnimation * pixbuf2 = gdk_pixbuf_animation_new_from_file(T2_NEXT_ICON, error);
	GtkImage * nxt_icon = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, "next_icon"));
	gtk_image_set_from_animation(GTK_IMAGE(nxt_icon), pixbuf2);
	
	button1 = GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);
	
	button5 = GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, "next"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test2_b_next_btnclicked),
                    NULL);

	
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_test2_b()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_test2_b_window) {
		   GError *error=NULL;
		Test2bBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (Test2bBuilder, TEST2B_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_test2_b_window();
		init_test2_b_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, TEST2_b_WINDOW));
	gtk_widget_show_all(window);
	cur_active_screen = test2_b_scrn;
}

void hide_window_test2_b()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (Test2bBuilder, TEST2_b_WINDOW));
	gtk_widget_hide(window);
}

void GetWindowEntry(void){
	FILE *fp;
	char *HightBuffer;
    char *WeightBuffer;  
		 
	ReadBMIForSummary();

	sprintf(Din.Weight, "%03.3f", UserSummaryData.Weight);
	sprintf(Din.Hight, "%d",(int)  UserSummaryData.Height);
	//HightBuffer = "168";//gtk_entry_get_text(entry1);
    //WeightBuffer = "60";//gtk_entry_get_text(entry2);	
	
	//strcpy(Din.Weight,WeightBuffer);
	//strcpy(Din.Hight,HightBuffer);
	Din.Comma = ',';

	fp = fopen (IN_ENTERY_FILE, "w");
	if(fp<0){
		printf(" System is not able to open file \n");
	}
	else{
		fwrite(&Din,sizeof(Din),1,fp);
		fclose(fp);
		printf(" System has updated the Hight Buf file %s \n",Din.Hight);
		printf(" System has updated the Hight Buf file %s \n",Din.Weight);
	}
}


void GetBliOutput(void){
	GtkWidget * window, * feed_back;
	FILE *fp;
	char Buf[2];
	char BF_sys_msg[64];
	char BF_percent[16];

	if(BLITestEn==1){
		fp = fopen (OT_TEXT_FILE , "r+");
		if(fp<0){
			printf(" System is not able to create file \n");
		}
		else{
			fread(&Buf,1,1,fp);
			fclose(fp);	
			if(Buf[0] == '1'){

				test2_button_set_state(TRUE);
				printf(" String info %d \n",Buf[0]);
				printf("System Got Data Updated \n");
				fp = fopen (OT_ENTERY_FILE, "r");
				fread(&Bli,sizeof(Bli),1,fp);
				fclose(fp);
				printf("Bmi Value is %s\n",&Bli.BliValue);
				//bodyFatLabel = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "label5"));
				if( ( strcmp(Bli.BliValue, "-inf") == 0)  || ( strcmp(Bli.BliValue, "-1") == 0) ){
					gtk_label_set_label(bodyFatLabel, "0.00");
					feed_back = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "label9"));
					gtk_label_set_label (feed_back, "Please grab hadle properly");
					//strcpy(BF_sys_msg, "Your Body fat test indicates:--");
					//gtk_label_set_label (test2_msg, BF_sys_msg);
				} else {
					strcpy(BF_percent, &Bli.BliValue);
					strcat(BF_percent, " %");
					gtk_label_set_label(bodyFatLabel, BF_percent);
					//strcpy(BF_sys_msg, "Your Body fat test indicates:normal");
					//gtk_label_set_label (test2_msg, BF_sys_msg);
				}
				BLITestEn=0;
				//SessionExpired = 0;
				Minutes = 0;
				Seconds = 0;
				//StartTimer = 0;
				ClearOutPutFile();
				
				ReadBFForSummary ();
				//blood_pressure_system_msg(UserSummaryData.SysBP, UserSummaryData.DiaBp, UserSummaryData.PulseRate, BP_test_msg);
				
				window = GTK_WIDGET (gtk_builder_get_object (Test2Builder, TEST2_WINDOW));
				gtk_widget_show_all(window);
				if(isQuickWellthCheck)
					test2_hide_next_button_n_image();
			}			
		}
	}
}
