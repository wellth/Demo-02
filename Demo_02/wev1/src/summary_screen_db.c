#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include <string.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"
#include <sqlite3.h>

/*****************************************************************************************************************
                                                        Variable Declaration
******************************************************************************************************************/

GtkWidget *label_sys_bp, *label_dia_bp, *label_pulse_rate;
extern GtkWidget *window,*Bliwindow,*Bpwindow,*Bmiwindow;

static int init_user_summary_window = 0;
static int init_health_summary_window = 0;

GtkBuilder *UserSummaryBuilder, *HealthSummaryBuilder;
/**************************Variable Declaration******************************/


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
/*****************************************************************************************************************
                                                       Button click message handler
******************************************************************************************************************/
void user_summary_home_btnclicked(GtkWidget *widget, gpointer user_data)
{
	int arg_val;
	arg_val = user_data;
	if(arg_val == 0) {
		hide_window_user_summary();
	} else {
		hide_window_health_summary ();
	}
	show_window_home();
	start_timer = 0;
}

void health_summary_home_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_user_summary();
	//hide_window_health_summary ();
	show_window_home();
	start_timer = 0;
}

void display_user_summary_btnclicked (GtkWidget *widget, gpointer user_data)
{
	hide_window_user_summary();
	show_window_health_summary();
	start_timer = 0;
}
/***********************Button click message handler**********************************************************************/


//************************************************************************************************************************
//						UserSummary
//************************************************************************************************************************
static GtkWidget*
create_user_summary_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5, *button6; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG, *imageTestStat;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (UserSummaryBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, USER_SUMMARY_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				USER_SUMMARY_WINDOW,
				USER_SUMMARY_UI_FILE);
        }

	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);

	button1 = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (user_summary_home_btnclicked),
                    0);
	
	button2 = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    0);

	button6 = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "display_on_hs"));
 	 g_signal_connect (G_OBJECT (button6), "clicked",
                    G_CALLBACK (display_user_summary_btnclicked),
                    NULL);
/*
	button2 = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);
*/
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_user_summary()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_user_summary_window) {
		   GError *error=NULL;
		UserSummaryBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (UserSummaryBuilder, USER_SUMMARY_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_user_summary_window();
		init_user_summary_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, USER_SUMMARY_WINDOW));
	gtk_widget_show_all(window);
	cur_active_screen = user_summary_scrn;
}

void hide_window_user_summary()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, USER_SUMMARY_WINDOW));
	gtk_widget_hide(window);
}

//************************************************************************************************************************
//						health Summary (health summary graphs are shown here)
//************************************************************************************************************************
static GtkWidget*
create_health_summary_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG, *imageTestStat;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (UserSummaryBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, HEALTH_SUMMARY_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				HEALTH_SUMMARY_WINDOW,
				HEALTH_SUMMARY_UI_FILE);
        }

	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);

	button1 = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (user_summary_home_btnclicked),
                    1);
	
	button2 = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    1);

	
/*
	button2 = GTK_WIDGET (gtk_builder_get_object (UserSummaryBuilder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);
*/
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_health_summary()
{
	GtkWidget *window, *label;
	GtkImage *imageBMI, *imageDBP, *imageSBP, *imageBF;
	char graph_wt_buf[64], graph_bf_buf[32], graph_sbp_buf[32], graph_dbp_buf[32];
	char BMI_img_path[128];
	char SBP_img_path[128];
	char DBP_img_path[128];
	char BF_img_path[128];
	GError** error = NULL;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char timeBuf[64];
	char Line[512], query[512];
	FILE *fpt;
	
	char lbl_buf[64];

	float Bmi_Val,Bli_Value;
	int Current_Age,Current_Gender;
	int Bmi_Health_Score,Bp_Health_Score,Bf_Health_Score,Co_Health_Score;

	if(!init_health_summary_window) {
		   GError *error=NULL;
		HealthSummaryBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (HealthSummaryBuilder, HEALTH_SUMMARY_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_health_summary_window();
		init_health_summary_window = 1;
	}
	//g_print("showing the health summary");
	label = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	g_print("showing the health summary--");
	ReadBMIForSummary();
	g_print("showing the health summary--->>>1");
	ReadLoginForSummary();
	g_print("showing the health summary--->>>2");
	ReadBPForSummary ();
	g_print("showing the health summary--->>>3");
	ReadBFForSummary ();
	
	get_blood_pressure_summary_graph_name(UserSummaryData.SysBP, UserSummaryData.DiaBp, UserSummaryData.Age, UserSummaryData.Gender, SBP_img_path, DBP_img_path);
	get_bmi_summary_graph_name(UserSummaryData.BMI_val, UserSummaryData.Age, UserSummaryData.Gender, BMI_img_path);
	get_bf_summary_graph_name(UserSummaryData.BMI_val, UserSummaryData.BF_val, UserSummaryData.Age, UserSummaryData.Gender, BF_img_path);
	//strcpy(BF_img_path, "GraphsNValues/bf/female/bff60-79_18.5-24.9t.png");
	g_print("BF path %s - bmi=%f - bf.val = %f age= %d \n gender= %s \n", BF_img_path, UserSummaryData.BMI_val, UserSummaryData.BF_val, UserSummaryData.Age, UserSummaryData.Gender);

	GtkWidget *grph_wt_label = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "label7"));
	sprintf(graph_wt_buf, "Weight= %03.03f Kgs, BMI = %03.03f", UserSummaryData.Weight, UserSummaryData.BMI_val);
	gtk_label_set_label (grph_wt_label, graph_wt_buf);

	GtkWidget *grph_bf_label = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "label10"));
	sprintf(graph_bf_buf, "Body Fat = %03.03f %", UserSummaryData.BF_val);
	gtk_label_set_label (grph_bf_label, graph_bf_buf);

	GtkWidget *grph_sbp_label = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "label9"));
	sprintf(graph_sbp_buf, "Systolic BP = %d", (int)UserSummaryData.SysBP);
	gtk_label_set_label (grph_sbp_label, graph_sbp_buf);

	GtkWidget *grph_dbp_label = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "label8"));
	sprintf(graph_dbp_buf, "Diastolic BP = %d", (int)UserSummaryData.DiaBp);
	gtk_label_set_label (grph_dbp_label, graph_dbp_buf);
		
	GdkPixbufAnimation * pixbufBMI = gdk_pixbuf_animation_new_from_file(BMI_img_path, error);
	GdkPixbufAnimation * pixbufSBP = gdk_pixbuf_animation_new_from_file(SBP_img_path, error);
	GdkPixbufAnimation * pixbufDBP = gdk_pixbuf_animation_new_from_file(DBP_img_path, error);
	GdkPixbufAnimation * pixbufBF = gdk_pixbuf_animation_new_from_file(BF_img_path, error);
	
	imageBMI = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "image2"));
	imageSBP = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "image4"));
	imageDBP = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "image5"));
	imageBF = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, "image3"));
		
	gtk_image_set_from_animation(GTK_IMAGE(imageBMI), pixbufBMI);
	gtk_image_set_from_animation(GTK_IMAGE(imageSBP), pixbufSBP);
	gtk_image_set_from_animation(GTK_IMAGE(imageDBP), pixbufDBP);
	gtk_image_set_from_animation(GTK_IMAGE(imageBF), pixbufBF);
	
	window = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, HEALTH_SUMMARY_WINDOW));
	gtk_widget_show_all(window);
	cur_active_screen = summary_display_scrn;

	system("/bin/date > /home/time_app.c");
	fpt = fopen("/home/time_app.c", "r");
	while( GetLine(fpt, Line) != 0){
		strcpy(timeBuf, Line);
		//printf("%s\n", Line);
	}
// -------------Calculate BMI Health_Score---------------------------------

                         Bmi_Val = UserSummaryData.BMI_val;
                        // Bmi_Val = atof(Bmi.BmiValue);
			Current_Age = UserSummaryData.Age;
			if (strcmp(UserSummaryData.Gender,"Female") == 0)
				Current_Gender = 1;
			else
				Current_Gender = 0;
		if ( Bmi_Val != 0.0)
		{

                        if (Current_Gender == 1)  // Female
                        {

                                if (Current_Age <= 20)
                                {
                                        if ( (Bmi_Val >= 15.05) && (Bmi_Val <= 25.25))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 21) && (Current_Age <= 25))
                                {
                                        if ( (Bmi_Val >= 15.24) && (Bmi_Val <= 27.05))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 26) && (Current_Age <= 30))
                                {
                                        if ( (Bmi_Val >= 15.20) && (Bmi_Val <= 28.43))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 31) && (Current_Age <= 35))
                                {
                                        if ( (Bmi_Val >= 16.36) && (Bmi_Val <= 30.83))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 36) && (Current_Age <= 40))
                                {
                                        if ( (Bmi_Val >= 17.0) && (Bmi_Val <= 33.75))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 41) && (Current_Age <= 45))
                                {
                                        if ( (Bmi_Val >= 16.76) && (Bmi_Val <= 34.01))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 46) && (Current_Age <= 50))
                                {
                                        if ( (Bmi_Val >= 16.08) && (Bmi_Val <= 32.50))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 51) && (Current_Age <= 55))
                                {
                                        if ( (Bmi_Val >= 18.50) && (Bmi_Val <= 34.51))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 56) && (Current_Age <= 60))
                                {
                                        if ( (Bmi_Val >= 16.39) && (Bmi_Val <= 33.44))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else  // Age Greater than 60
                                {
                                        if ( (Bmi_Val >= 16.15) && (Bmi_Val <= 31.16))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }




                        }
                        else   // Male
                        {

                                if (Current_Age <= 20)
                                {
                                        if ( (Bmi_Val >= 15.57) && (Bmi_Val <= 25.03))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 21) && (Current_Age <= 25))
                                {
                                        if ( (Bmi_Val >= 15.87) && (Bmi_Val <= 26.51))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 26) && (Current_Age <= 30))
                                {
                                        if ( (Bmi_Val >= 16.43) && (Bmi_Val <= 27.54))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 31) && (Current_Age <= 35))
                                {
                                        if ( (Bmi_Val >= 16.33) && (Bmi_Val <= 28.67))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 36) && (Current_Age <= 40))
                                {
                                        if ( (Bmi_Val >= 17.21) && (Bmi_Val <= 29.89))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 41) && (Current_Age <= 45))
                                {
                                        if ( (Bmi_Val >= 17.51) && (Bmi_Val <= 30.11))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 46) && (Current_Age <= 50))
                                {
                                        if ( (Bmi_Val >= 17.99) && (Bmi_Val <= 30.20))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 51) && (Current_Age <= 55))
                                {
                                        if ( (Bmi_Val >= 18.17) && (Bmi_Val <= 35.76))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else if ((Current_Age >= 56) && (Current_Age <= 60))
                                {
                                        if ( (Bmi_Val >= 16.99) && (Bmi_Val <= 32.24))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }
                                else  // Age Greater than 60
                                {
                                        if ( (Bmi_Val >= 17.53) && (Bmi_Val <= 30.82))
                                                Bmi_Health_Score = 1;
                                        else
                                                Bmi_Health_Score = 0;
                                }



                        }
		}
		else
			Bmi_Health_Score = 0;


// -----------------BP Health_Score Calculation -----------------------

		if ( UserSummaryData.SysBP != 0.0)
		{
                	if ((UserSummaryData.SysBP < 140.0) && (UserSummaryData.DiaBp < 90.0))
                        	Bp_Health_Score = 1;
                	else
                        	Bp_Health_Score = 0;
		}
		else
			Bp_Health_Score = 0;

//-----------------BF Health_Score Calculation -------------------------


                Bli_Value = UserSummaryData.BF_val;

		if ( Bli_Value != 0.0)
		{
                if (Current_Gender == 1)  // Female
                {
                        // if (Current_Age < 20)
                        //        Bf_Health_Score = 1;
                        //else if ((Current_Age >=20) && (Current_Age <=39))
                        if (Current_Age <=39)
                        {
                                if ((Bli_Value > 21.0) && (Bli_Value < 39.0))
                                        Bf_Health_Score = 1;
                                else
                                        Bf_Health_Score = 0;
                        }
                        else if ((Current_Age >=40) && (Current_Age <=59))
                        {
                                if ((Bli_Value > 23.0) && (Bli_Value < 40.0))
                                        Bf_Health_Score = 1;
                                else
                                        Bf_Health_Score = 0;
                        }
                        else if ((Current_Age >=60) && (Current_Age <=79))
                        {
                                if ((Bli_Value > 24.0) && (Bli_Value < 42.0))
                                        Bf_Health_Score = 1;
                                else
                                        Bf_Health_Score = 0;
                        }
                        else
                                Bf_Health_Score = 1;

                }
                else
                {

                        // if (Current_Age < 20)
                        //        Bf_Health_Score = 1;
                        // else if ((Current_Age >=20) && (Current_Age <=39))

                        if (Current_Age <=39)
                        {
                                if ((Bli_Value > 8.0) && (Bli_Value < 25.0))
                                        Bf_Health_Score = 1;
                                else
                                        Bf_Health_Score = 0;
                        }
                        else if ((Current_Age >=40) && (Current_Age <=59))
                        {
                                if ((Bli_Value > 11.0) && (Bli_Value < 28.0))
                                        Bf_Health_Score = 1;
                                else
                                        Bf_Health_Score = 0;
                        }
                        else if ((Current_Age >=60) && (Current_Age <=79))
                        {
                                if ((Bli_Value > 13.0) && (Bli_Value < 30.0))
                                        Bf_Health_Score = 1;
                                else
                                        Bf_Health_Score = 0;
                        }
                        else
                                Bf_Health_Score = 1;

                }
		else
			Bf_Health_Score = 0;


//---------------CO Health_Score Calculation --------------------------

		if (UserSummaryData.CO_val != 0.0)
		{
			if (UserSummaryData.CO_val < 1.1 )
				Co_Health_Score = 1;
			else
				Co_Health_Score = 0;
		}
		else
			Co_Health_Score = 0;
			


//-------------Database Logging ---------------------------------------
	g_print("current date and time=\n%s\n",timeBuf);
	sprintf(query, "insert into app_data (name, age, gender,weight,BMI_val, height, BF_val, CO_val, sys_bp, dia_bp, pulse_rate,Bmi_Sc,Bf_Sc,Co_Sc,Bp_Sc) values ('%s', %d, '%s', '%f', '%f','%f', '%f', '%f','%f', '%f', '%f',%d, %d, %d, %d) ", UserSummaryData.Login_name,UserSummaryData.Age, UserSummaryData.Gender, UserSummaryData.Weight, UserSummaryData.BMI_val, UserSummaryData.Height, UserSummaryData.BF_val, UserSummaryData.CO_val,\
	        UserSummaryData.SysBP, UserSummaryData.DiaBp, UserSummaryData.PulseRate,Bmi_Health_Score,Bf_Health_Score,Co_Health_Score,Bp_Health_Score);
	g_print("query is=\n%s\n",query);
	if (sqlite3_open(APP_DATABASE_PATH, &db) != SQLITE_OK)
	{
		g_print("Can't open database: \n");
		sqlite3_close(db);
		exit(1);
	  }
	  rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
	  if( rc!=SQLITE_OK ){
		g_print("SQL error: %s\n", zErrMsg);
	  }
	  sqlite3_close(db);
	
}

void hide_window_health_summary()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (HealthSummaryBuilder, HEALTH_SUMMARY_WINDOW));
	gtk_widget_hide(window);
}



