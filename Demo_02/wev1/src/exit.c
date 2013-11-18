#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include <string.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"

static int init_exit_window = 0;
GtkBuilder *ExitBuilder;
GtkWidget *button1, *button2, *button3, *button4, *button5; 
GtkWidget * CoLevelLabel, *test3_msg;
GtkCheckButton *cb1_1, *cb1_2, *cb1_3;
GtkCheckButton *cb2_1, *cb2_2, *cb2_3;
GtkCheckButton *cb3_1, *cb3_2, *cb3_3;

void exit_exit_btnclicked (GtkWidget **widget, gpointer user_data) {
	hide_window_exit ();
	//show_window_login();
	show_window_second ();
}

void exit_set_priority(GtkWidget *widget, gpointer user_data) {
	int value;
	gboolean v1_1,v1_2,v1_3,v2_1,v2_2,v2_3,v3_1,v3_2,v3_3;

	value = (int)user_data;
	v1_1 = gtk_toggle_button_get_active ((GtkToggleButton*)cb1_1); //all 1 labeled check button
	v1_2 = gtk_toggle_button_get_active ((GtkToggleButton*)cb1_2);
	v1_3 = gtk_toggle_button_get_active ((GtkToggleButton*)cb1_3);

	v2_1 = gtk_toggle_button_get_active ((GtkToggleButton*)cb2_1); //all 2 labels check button
	v2_2 = gtk_toggle_button_get_active ((GtkToggleButton*)cb2_2);
	v2_3 = gtk_toggle_button_get_active ((GtkToggleButton*)cb2_3);

	v3_1 = gtk_toggle_button_get_active ((GtkToggleButton*)cb3_1); //all 3 labels check button
	v3_2 = gtk_toggle_button_get_active ((GtkToggleButton*)cb3_2);
	v3_3 = gtk_toggle_button_get_active ((GtkToggleButton*)cb3_3);
 
	g_print("~~~~~~%d~~~~~%d##%d##%d~~~~~~~~~~~~\n",value, (int)v1_1,(int) v1_2,(int) v1_3);
	
	if (value == 1){
		if(v1_1) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_1, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_3, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_1, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_1, FALSE);
		}
	}

	if (value == 2){
		if(v1_2) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_3, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_2, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, FALSE);
		}
	}
	if (value == 3){
		if(v1_3) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_3, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_3, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_3, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_3, FALSE);
		}
	}
	if (value == 4){
		if(v2_1) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_1, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_3, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_1, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_1, FALSE);
		}
	}

	if (value == 5){
		if(v2_2) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_2, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_3, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_2, FALSE);
		}
	}
	if (value == 6){
		if(v2_3) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_3, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_3, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_3, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_3, FALSE);
		}
	}

	if (value == 7){
		if(v3_1) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_1, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_3, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_1, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_1, FALSE);
		}
	}

	if (value == 8){
		if(v3_2) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_2, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_3, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_2, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, FALSE);
		}
	}
	if (value == 9){
		if(v3_3) {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_3, TRUE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_1, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_2, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb1_3, FALSE);
			gtk_toggle_button_set_active ((GtkToggleButton*)cb2_3, FALSE);
		} else {
			gtk_toggle_button_set_active ((GtkToggleButton*)cb3_3, FALSE);
		}
	}
	/*
	if(!v1_1){
		 gtk_toggle_button_set_active ((GtkToggleButton*)cb1_1, TRUE);
	} else {
		gtk_toggle_button_set_active ((GtkToggleButton*)cb3_1, FALSE);
	}
	
	if(!v1_2){
		 gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, TRUE);
	} else {
		gtk_toggle_button_set_active ((GtkToggleButton*)cb1_2, FALSE);
	}
	
	if(!v1_3){
		 gtk_toggle_button_set_active ((GtkToggleButton*)cb1_3, TRUE);
	} else {
		gtk_toggle_button_set_active ((GtkToggleButton*)cb1_3, FALSE);
	}
*/
	return;
}

static GtkWidget*
create_exit_window (void)
{
	GtkWidget *window;
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (ExitBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (ExitBuilder, EXIT_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				EXIT_WINDOW,
				EXIT_UI_FILE);
        }

	//

	//GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	//imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (ExitBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (ExitBuilder, "wellth_logo"));
	//gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (ExitBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
	
	cb1_1 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton1"));
	cb1_2 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton2"));
	cb1_3 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton3"));

	cb2_1 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton4"));
	cb2_2 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton5"));
	cb2_3 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton6"));

	cb3_1 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton7"));
	cb3_2 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton8"));
	cb3_3 = GTK_WIDGET (gtk_builder_get_object(ExitBuilder, "checkbutton9"));
	
	g_signal_connect (G_OBJECT (cb1_1), "released", G_CALLBACK (exit_set_priority),
                    1);
	g_signal_connect (G_OBJECT (cb1_2), "released", G_CALLBACK (exit_set_priority),
                    2);
	g_signal_connect (G_OBJECT (cb1_3), "released", G_CALLBACK (exit_set_priority),
                    3);
	
	g_signal_connect (G_OBJECT (cb2_1), "released", G_CALLBACK (exit_set_priority),
                    4);
	g_signal_connect (G_OBJECT (cb2_2), "released", G_CALLBACK (exit_set_priority),
                    5);
	g_signal_connect (G_OBJECT (cb2_3), "released", G_CALLBACK (exit_set_priority),
                    6);
	
	g_signal_connect (G_OBJECT (cb3_1), "released", G_CALLBACK (exit_set_priority),
                    7);
	g_signal_connect (G_OBJECT (cb3_2), "released", G_CALLBACK (exit_set_priority),
                    8);
	g_signal_connect (G_OBJECT (cb3_3), "released", G_CALLBACK (exit_set_priority),
                    9);
/*
	GdkPixbufAnimation * pixbufTestanm = gdk_pixbuf_animation_new_from_file(TEST3, error);
	imageTest = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test3Builder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTest), pixbufTestanm);

	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test3Builder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);


	//GdkPixbufAnimation * pixbuf1 = gdk_pixbuf_animation_new_from_file(GRAPH, error);
	//GtkImage * grf = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test3Builder, "graph"));
	//gtk_image_set_from_animation(GTK_IMAGE(grf), pixbuf1);
	
	GdkPixbufAnimation * pixbuf2 = gdk_pixbuf_animation_new_from_file(T3_NEXT_ICON, error);
	GtkImage * nxt_icon = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (Test3Builder, "next_icon"));
	gtk_image_set_from_animation(GTK_IMAGE(nxt_icon), pixbuf2);
	*/
	
	button1 = GTK_WIDGET (gtk_builder_get_object (ExitBuilder, "exit_btn"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (exit_exit_btnclicked),
                    NULL);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_exit()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_exit_window) {
		   GError *error=NULL;
		ExitBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (ExitBuilder, EXIT_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_exit_window();
		init_exit_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (ExitBuilder, "label2"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);


	label = GTK_WIDGET (gtk_builder_get_object (ExitBuilder, "label1"));
	sprintf(lbl_buf, "Thanks %s for using the wellth station.", GlobalUserName );
	//strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (ExitBuilder, EXIT_WINDOW));
	gtk_widget_show_all(window);
	cur_active_screen = exit_scrn;
}

void hide_window_exit()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (ExitBuilder, EXIT_WINDOW));
	gtk_widget_hide(window);
}
