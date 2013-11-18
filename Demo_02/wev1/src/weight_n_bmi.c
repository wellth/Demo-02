#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include <string.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"


_WT_IN_FILE InText;
_BMI_TEST Bmi;
_WT_INPUT_DATA Din;

static float ideal_weight1, ideal_weight2;

unsigned char BMITestEn = 0;
//GtkWidget *entry3,*entry1,*entry2,*entry4;
GtkWidget *weight_label, *BMI_label, *test1_message, *ideal_weight_label;
GtkLabel *t1_normal_smsg1, *t1_normal_smsg2, *t1_normal_smsg3, *t1_normal_smsg4;
GtkLabel *t1_result_smsg1, *t1_result_smsg2, *t1_result_smsg3, *t1_result_smsg4;
extern GtkBuilder *BliBuilder, *BmiBuilder, *BpBuilder;
extern GtkWidget *window,*Bliwindow,*Bpwindow,*Bmiwindow;
GtkWidget *label;

static GtkWidget *button1, *button2, *button3, *button4, *button5;

static int init_test1_b_window = 0, init_test1_window = 0;
GtkBuilder *Test1Builder, *Test1bBuilder;
static GtkComboBoxText *height_list;
/*
gboolean _label_update(gpointer data)
{
    GtkLabel *label = (GtkLabel*)data;
	char buf[256];
	if(sec_expired == 0){
		memset(&buf, 0x0, 256);
		snprintf(buf, 255, "%d", sec_expired);
		gtk_label_set_label(label, buf);WriteDataFromUI
	}
	if(start_timer) { 
		memset(&buf, 0x0, 256);
		snprintf(buf, 255, "%d", ++sec_expired);
		gtk_label_set_label(label, buf);
		if(sec_expired > 2500){
			sec_expired = 0;
			start_timer = 0;
			//hide_measure_btn_show_repeate_btn();
		}unsigned char BLITestEn,BPTestEn,BMITestEn;
	} else {
		sec_expired = 0;
	}
	
	return continue_timer;
}
void _start_timer (GtkWidget *button, gpointer data)
{
    (void)button;///Avoid compiler warnings
    GtkWidget *label = data;
   //if(!start_timer)
    {
        //g_timeout_add_seconds(1, _label_update, label);
		g_timeout_add(1, _label_update, label);
        start_timer = TRUE;
        continue_timer = TRUE;
    }
}
*/
void reset_test1_results(){
	gtk_label_set_label (weight_label, "**Kg");
	gtk_label_set_label (BMI_label, "**");
	//gtk_label_set_label (test1_message, "System message");

	gtk_widget_show (t1_normal_smsg1);
	gtk_widget_show (t1_normal_smsg2);
	gtk_widget_show (t1_normal_smsg3);
	gtk_widget_show (t1_normal_smsg4);
	gtk_widget_hide (t1_result_smsg1);
	gtk_widget_hide (t1_result_smsg2);
	gtk_widget_hide (t1_result_smsg3);
	gtk_widget_hide (t1_result_smsg4);
}

void test1_b_next_btnclicked(GtkWidget *widget, gpointer user_data)
{

	hide_window_test1_b();
	show_window_test2();
	set_cur_test = CUR_TEST2;
	show_measure_btn_hide_repeate_btn();
}

void test1_next_btnclicked(GtkWidget *widget, gpointer user_data)
{
	reset_test1_results();	
	hide_window_test1();
	show_window_test1_b();
	//show_window_test2();
	//set_cur_test = CUR_TEST2;
	//show_measure_btn_hide_repeate_btn();
}

void test1_hide_next_button_n_image(){
	GtkWidget *button;GtkImage *nextImage;
	button = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "next"));
	nextImage = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "next_icon"));
	gtk_widget_hide(button);
	gtk_widget_hide(nextImage);
}
/********************************************************************************************************************************************************
										Button click handler
 ********************************************************************************************************************************************************/

void test1_button_set_state(int bool_val){
	gtk_widget_set_sensitive(button1, bool_val);
	gtk_widget_set_sensitive(button2, bool_val);
	gtk_widget_set_sensitive(button3, bool_val);
	gtk_widget_set_sensitive(button4, bool_val);
	gtk_widget_set_sensitive(button5, bool_val);
	gtk_widget_set_sensitive(height_list, bool_val);

	GtkWidget *feed_back;
	if(bool_val == TRUE){
		feed_back = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "label9"));
		gtk_label_set_label (feed_back, "Click to measure");
	}
}

void test1_click_to_measure_btnclicked(GtkWidget *widget, gpointer user_data)
{
	gchar *buf;

	FILE *fp;
	start_timer = 1;
	char HightBuffer[8], *HtPtr;
	GtkWidget * feed_back;

	reset_test1_results();
	strcpy(HightBuffer, "");
	HtPtr = gtk_combo_box_text_get_active_text (height_list);
	if(HtPtr != NULL)
		strcpy(HightBuffer, HtPtr);
	feed_back = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "label9"));
	if( strcmp(HightBuffer, "")  == 0){
		gtk_label_set_label (feed_back, "Please select the height first");
		return;
	}
	/*
	if(StartTimer != 1) {
		StartTimer = 1;
		TimerInit();
	} else {
		gtk_label_set_label (feed_back, "Please wait!\nGetting your weight and BMI");
		return;
	}*/
	
	gtk_label_set_label (feed_back, "Please wait..! Getting your weight and BMI");
	//buf	= gtk_combo_box_text_get_active_text (height_list);
	g_print("The value of cs is: %s", HightBuffer);
	//if height is not selected show dialog window
	
	printf("ip is ready \n");
	fp = fopen (IN_TEXT_FILE, "w");
	if(fp<0){
		printf(" System is not able to create file \n");
	}
	else{
		printf("ip is ready \n");
		ClearOutPutFile();
		WriteEntryFile();
		strcpy(InText.InBuf,"WeTest");
		fwrite(&InText,sizeof(InText),1,fp);
		printf(" File has been Updated \n");
		fclose(fp);			
	}
	Minutes = 0;
	Seconds = 0;
	StartTimer = 1;
	TimerInit();
	test1_button_set_state(FALSE);
	return;
} 

void WriteEntryFile(void){
	FILE *fp;
	char *HightBuffer;

	HightBuffer = gtk_combo_box_text_get_active_text (height_list);
	//HightBuffer = gtk_entry_get_text(entry1);	
	strcpy(Din.Weight,"123");
	strcpy(Din.Hight,HightBuffer);
	Din.Comma = ',';

	fp = fopen (IN_ENTERY_FILE, "w");
	if(fp<0){
		printf(" System is not able to open file \n");
	}
	else{
		fwrite(&Din,sizeof(Din),1,fp);
		fclose(fp);
		printf(" System has updated the Hight Buf file %s \n",Din.Hight);
		BMITestEn=1;
	}
}


void GetBmiOutput(void){
	GtkWidget *window;
	FILE *fp;
	char Buf[2];
	char wt_buf[32], bmi_buf[32];
	char BMI_msg_buf[128];
   
	if(BMITestEn==1){
		fp = fopen (OT_TEXT_FILE, "r+");
		if(fp<0){
			printf(" System is not able to create file \n");
		}
		else{
			fread(&Buf,1,1,fp);
			fclose(fp);	
			if(Buf[0] == '1'){
				printf(" String info %d \n",Buf[0]);
				printf("System Got Data Updated \n");
				fp = fopen (OT_ENTERY_FILE, "r");
				fread(&Bmi,sizeof(Bmi),1,fp);
				fclose(fp);
				sprintf(wt_buf, "%s", Bmi.Weight);
				sprintf(bmi_buf, "%s", Bmi.BmiValue);
				
				printf("Bmi Value is %s\n",Bmi.Weight);
				printf("Bmi Value is %s\n",Bmi.BmiValue);			

				
					//gtk_entry_set_text(entry2,&Bmi.Weight);
				//gtk_entry_set_text(entry3,&Bmi.BmiValue);			
				gtk_label_set_label (weight_label, wt_buf);
				gtk_label_set_label (BMI_label, bmi_buf);
				BMITestEn=0;
				Minutes = 0;
				Seconds = 0;
				ClearOutPutFile();
				
				ReadBMIForSummary();
				g_print("\nCreate BMI message--%f\n", UserSummaryData.BMI_val);
				BMI_system_msg(UserSummaryData.BMI_val, BMI_msg_buf);
				
				//gtk_label_set_label (test1_message, BMI_msg_buf);
				if(strcmp(BMI_msg_buf, "Underweight") == 0){
					
					gtk_widget_hide (t1_normal_smsg1);
					gtk_widget_show (t1_result_smsg1);
				}
				if(strcmp(BMI_msg_buf, "Normal weight") == 0){
					gtk_widget_hide (t1_normal_smsg2);
					gtk_widget_show (t1_result_smsg2);
				}
				if(strcmp(BMI_msg_buf, "Overweight") == 0){
					gtk_widget_hide (t1_normal_smsg3);
					gtk_widget_show (t1_result_smsg3);
				}
				if(strcmp(BMI_msg_buf, "Obese") == 0){
					gtk_widget_hide (t1_normal_smsg4);
					gtk_widget_show (t1_result_smsg4);
				}

				test1_button_set_state(TRUE);

				window = GTK_WIDGET (gtk_builder_get_object (Test1Builder, TEST1_WINDOW));
				gtk_widget_show_all(window);
				if(isQuickWellthCheck)
					test1_hide_next_button_n_image();
			}			
		}
	}
}

/**********************************************************************************************************************************************
 ********************************************************************************************************************************************************/

#define START_HT	135
#define END_HT		210

char ht_str[75][6];

static void
cb_changed( GtkComboBox *combo,
            gpointer     data )
{
	int i = 0;
    /* Obtain currently selected string from combo box */
    gchar *string = gtk_combo_box_text_get_active_text( height_list );
	gtk_combo_box_set_active_id(height_list, string);
	/*
	if( strcmp((char *)string, ht_str[i]));
	for(){
		gtk_combo_box_set_active_id(
	}
*/
    /* Print it to the console - if nothing is selected, print NULL */
    g_print( "Selected (simple): >> %s <<\n", ( string ? string : "NULL" ) );

    /* Free string */
    g_free( string );
}

void test1_set_btnclicked(GtkWidget *widget, gpointer user_data)
{
}


static GtkWidget*
create_test1_window (void)
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
	gtk_builder_connect_signals (Test1Builder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (Test1Builder, TEST1_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TEST1_WINDOW,
				TEST1_UI_FILE);
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
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

	GdkPixbufAnimation * pixbufTestanm = gdk_pixbuf_animation_new_from_file(TEST1, error);
	imageTest = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTest), pixbufTestanm);
	
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
#endif
	
	//GdkPixbufAnimation * pixbuf1 = gdk_pixbuf_animation_new_from_file(GRAPH, error);
	//GtkImage * grf = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "graph"));
	//gtk_image_set_from_animation(GTK_IMAGE(grf), pixbuf1);
	

	
	button1 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);
	vbox1 = gtk_vbox_new( FALSE, 6 );
    gtk_container_add( GTK_CONTAINER( window ), vbox1 );
	
	GtkFrame *gvp = (GtkFrame *)GTK_WIDGET (gtk_builder_get_object (Test1Builder,"viewport1"));
	gtk_box_pack_start( GTK_BOX( vbox1 ), gvp, FALSE, FALSE, 0 );
	//GtkComboBoxText *height_list = gtk_combo_box_text_new();
	height_list = gtk_combo_box_text_new();
    gtk_container_add( GTK_CONTAINER( gvp ), height_list );

	//GtkComboBoxText *height_list = (GtkComboBoxText *)GTK_WIDGET (gtk_builder_get_object (Test1Builder,"combobox1"));
	
	for(i=START_HT; i<=END_HT;i++){
		sprintf(buf,"%d",i);
		gtk_combo_box_text_append_text((GtkComboBoxText *)(height_list), (const gchar *)buf);
		strcpy(ht_str, (char *)buf);
	}


	//selected_height = gtk_combo_box_text_get_active_text (height_list);
	
	profileBtnClickedFrom = test1_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);

	button4 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "repeate"));
	g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (test1_click_to_measure_btnclicked),
                    NULL);
	
	button3 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "click_to_measure"));
	timer_label = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "timer_label"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (test1_click_to_measure_btnclicked),
                    NULL);
	continue_timer = TRUE;
    start_timer = TRUE;

	//g_signal_connect(G_OBJECT(button3), "clicked", G_CALLBACK(_start_timer), timer_label);
	//g_signal_connect(G_OBJECT(button31), "clicked", G_CALLBACK(_start_timer), timer_label);
	//button4 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "set"));
 	 //g_signal_connect (G_OBJECT (button4), "clicked",
       //             G_CALLBACK (test1_set_btnclicked),
         //           NULL);
	 g_signal_connect( G_OBJECT( height_list ), "changed", G_CALLBACK( cb_changed ), NULL );
	
	button5 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "next"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test1_next_btnclicked),
                    NULL);

	weight_label = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "label10"));
	BMI_label = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "label11"));
	//test1_message = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "label7"));
	t1_normal_smsg1 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "t1_normal_smsg1"));
	t1_normal_smsg2 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "t1_normal_smsg2"));
	t1_normal_smsg3 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "t1_normal_smsg3"));
	t1_normal_smsg4 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "t1_normal_smsg4"));
	t1_result_smsg1 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "t1_result_smsg1"));
	t1_result_smsg2 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "t1_result_smsg2"));
	t1_result_smsg3 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "t1_result_smsg3"));
	t1_result_smsg4 = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "t1_result_smsg4"));
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}



void show_window_test1()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_test1_window) {
		   GError *error=NULL;
		Test1Builder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (Test1Builder, TEST1_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_test1_window();
		init_test1_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (Test1Builder, TEST1_WINDOW));
	reset_test1_results();
	gtk_widget_show_all(window);
	cur_active_screen = test1_scrn;
}

void hide_window_test1()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (Test1Builder, TEST1_WINDOW));
	gtk_widget_hide(window);
}

static GtkWidget*
create_test1_b_window (void)
{
	GtkWidget *window; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG, *add;
	GtkWidget *timer_label;

	GtkWidget *vbox1;
	GError** error = NULL;
	int i=0;
	char buf[8];
	char idl_wt_buf[64];
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (Test1bBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, TEST1_b_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TEST1_b_WINDOW,
				TEST1B_UI_FILE);
        }

	ideal_weight1 = (float) (UserSummaryData.Height * UserSummaryData.Height * 18.5) / 10000;
	ideal_weight2 = (float) (UserSummaryData.Height * UserSummaryData.Height * 24.9) / 10000;
		
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

/*
	GdkPixbufAnimation * pixbufTestanm = gdk_pixbuf_animation_new_from_file(TEST1, error);
	imageTest = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTest), pixbufTestanm);
*/
	
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
	
//#endif
	
	//GdkPixbufAnimation * pixbuf1 = gdk_pixbuf_animation_new_from_file(GRAPH, error);
	//GtkImage * grf = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1Builder, "graph"));
	//gtk_image_set_from_animation(GTK_IMAGE(grf), pixbuf1);
	
	GdkPixbufAnimation * pixbuf2 = gdk_pixbuf_animation_new_from_file(T1_NEXT_ICON, error);
	GtkImage * nxt_icon = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, "next_icon"));
	gtk_image_set_from_animation(GTK_IMAGE(nxt_icon), pixbuf2);
	
	button1 = GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);
	
	 g_signal_connect( G_OBJECT( height_list ), "changed", G_CALLBACK( cb_changed ), NULL );
	
	button5 = GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, "next"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test1_b_next_btnclicked),
                    NULL);

	ideal_weight_label = GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, "ideal_weight"));

	sprintf(idl_wt_buf, "Your ideal weight range is %03.03f - %03.03f kgs", ideal_weight1, ideal_weight2);
	gtk_label_set_label (ideal_weight_label, idl_wt_buf);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_test1_b()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_test1_b_window) {
		   GError *error=NULL;
		Test1bBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (Test1bBuilder, TEST1B_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_test1_b_window();
		init_test1_b_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	GdkColor color;
	//color.red = 
	color.red = 65535;
	color.green = 0;
	color.blue = 0;
    gdk_color_parse ("red", &color);
   // gtk_widget_draw ( GTK_WIDGET(label), GTK_STATE_NORMAL, &color);
	
	window = GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, TEST1_b_WINDOW));
	gtk_widget_show_all(window);
	cur_active_screen = test1_b_scrn;
}

void hide_window_test1_b()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (Test1bBuilder, TEST1_b_WINDOW));
	gtk_widget_hide(window);
}
