#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include "window_defs_n_config.h"


static int init_profile_window = 0;
GtkBuilder *ProfileBuilder;

void profile_add_dependent_btnclicked(GtkWidget *widget, gpointer user_data)
{
	
}
void profile_change_pwd_btnclicked(GtkWidget *widget, gpointer user_data){
}

void profile_update_pro_btnclicked(GtkWidget *widget, gpointer user_data){
}

void profile_home_btnclicked(GtkWidget *widget, gpointer user_data){
	hide_window_profile ();
	show_window_home ();
}
void profile_cancel_btnclicked(GtkWidget *widget, gpointer user_data){
}
void profile_exit_btnclicked(GtkWidget *widget, gpointer user_data){
	hide_window_profile ();
	show_window_exit ();
	/*
	switch(profileBtnClickedFrom){
		case home_pro_btn:
			show_window_home ();
			break;
		case test1_pro_btn:
			show_window_test1 ();
			break;
		case test2_pro_btn:
			show_window_test2 ();
			break;
		case test3_pro_btn:
			show_window_test3 ();
			break;
		case test4_pro_btn:
			show_window_test4 ();
			break;
	
		case client_admin_pro_btn:
			//show_window_c ();
			break;
		case check_symptoms_pro_btn:
			show_window_check_symptoms();
			break;
		case email_colleague_pro_btn:
			show_window_email_colleague();
			break;
		case last_wellth_check_pro_btn:
			show_window_prev_wellth_check();
			break;
		case quick_wellth_check_pro_btn:
			show_window_quick_wellth_check();
			break;
		case start_test_pro_btn:
			show_window_first_time_profile();
			break;
		case summary_display_pro_btn:
			break;
		case user_summary_pro_btn:
			break;
	}
	*/
}

static GtkWidget*
create_profile_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5, *button6; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (ProfileBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, PROFILE_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				PROFILE_WINDOW,
				TEST2_UI_FILE);
        }

	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);


	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);
	
	
	button1 = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (profile_exit_btnclicked),
                    NULL);

	//profileBtnClickedFrom = test2_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "cancel"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (profile_cancel_btnclicked),
                    NULL);
	
	button3 = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (profile_home_btnclicked),
                    NULL);

	button4 = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "update_profile"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (profile_update_pro_btnclicked),
                    NULL);

	button5 = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "add_dependent"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (profile_add_dependent_btnclicked),
                    NULL);

	button6 = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, "change_pwd"));
 	 g_signal_connect (G_OBJECT (button6), "clicked",
                    G_CALLBACK (profile_change_pwd_btnclicked),
                    NULL);
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_profile()
{
	GtkWidget *window;
	if(!init_profile_window) {
		   GError *error=NULL;
		ProfileBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (ProfileBuilder, PROFILE_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_profile_window();
		init_profile_window = 1;
	}
	window = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, PROFILE_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = profile_scrn;
}

void hide_window_profile()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (ProfileBuilder, PROFILE_WINDOW));
	gtk_widget_hide(window);
}
