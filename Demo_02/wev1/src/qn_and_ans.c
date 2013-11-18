#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include <string.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"

static int init_qn_and_ans_window = 0;
GtkBuilder *QnAndAnsBuilder;


void qna_next_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_qna();
	show_window_health_summary ();
	//show_window_user_summary();
}


static GtkWidget*
create_qn_and_ans_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5; 
	GtkImage *imageClient, *imageWellth, *imageBG;
	GtkImage *imageQ1O1, *imageQ1O2, *imageQ1O3, *imageQ2O1, *imageQ2O2, *imageQ2O3; 

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (QnAndAnsBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, QN_AND_ANS_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				QN_AND_ANS_WINDOW,
				QN_AND_ANS_UI_FILE);
        }

	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

#if 1
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);

	/***********questions images************/
	GdkPixbufAnimation * q1o1_img = gdk_pixbuf_animation_new_from_file(Q1O1_IMG, error);
	GdkPixbufAnimation * q1o2_img = gdk_pixbuf_animation_new_from_file(Q1O2_IMG, error);
	GdkPixbufAnimation * q1o3_img = gdk_pixbuf_animation_new_from_file(Q1O3_IMG, error);
	GdkPixbufAnimation * q2o1_img = gdk_pixbuf_animation_new_from_file(Q2O1_IMG, error);
	GdkPixbufAnimation * q2o2_img = gdk_pixbuf_animation_new_from_file(Q2O2_IMG, error);
	GdkPixbufAnimation * q2o3_img = gdk_pixbuf_animation_new_from_file(Q2O3_IMG, error);

	imageQ1O1 = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "image2"));
	imageQ1O2 = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "image3"));
	imageQ1O3 = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "image4"));
	imageQ2O1 = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "image5"));
	imageQ2O2 = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "image6"));
	imageQ2O3 = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "image7"));

	gtk_image_set_from_animation(GTK_IMAGE(imageQ1O1), q1o1_img);
	/*gtk_image_set_from_animation(GTK_IMAGE(imageQ1O2), q1o1_img);
	gtk_image_set_from_animation(GTK_IMAGE(imageQ1O3), q1o1_img);
	gtk_image_set_from_animation(GTK_IMAGE(imageQ2O1), q1o1_img);
	gtk_image_set_from_animation(GTK_IMAGE(imageQ2O2), q1o1_img);
	gtk_image_set_from_animation(GTK_IMAGE(imageQ2O3), q1o1_img);

*/
	gtk_image_set_from_animation(GTK_IMAGE(imageQ1O2), q1o2_img);
	gtk_image_set_from_animation(GTK_IMAGE(imageQ1O3), q1o3_img);
	gtk_image_set_from_animation(GTK_IMAGE(imageQ2O1), q2o1_img);
	gtk_image_set_from_animation(GTK_IMAGE(imageQ2O2), q2o2_img);
	gtk_image_set_from_animation(GTK_IMAGE(imageQ2O3), q2o3_img);
	
	GdkPixbufAnimation * pixbuf1 = gdk_pixbuf_animation_new_from_file(HEALTH_STAT, error);
	GtkImage * grf = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "image8"));
	gtk_image_set_from_animation(GTK_IMAGE(grf), pixbuf1);
	
	/*GdkPixbufAnimation * pixbuf2 = gdk_pixbuf_animation_new_from_file(T3_NEXT_ICON, error);
	GtkImage * nxt_icon = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "next_icon"));
	gtk_image_set_from_animation(GTK_IMAGE(nxt_icon), pixbuf2);
*/

	button5 = GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "next"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (qna_next_btnclicked),
                    NULL);
	#endif
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_qna()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_qn_and_ans_window) {
		   GError *error=NULL;
		QnAndAnsBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (QnAndAnsBuilder, QN_AND_ANS_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		g_print("\n#########entering into creating qna screen\n");
		create_qn_and_ans_window();
		g_print("\n#########created qna screen\n");
		init_qn_and_ans_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	g_print("\n#########label set\n");
	gtk_label_set_label (label, lbl_buf);
	g_print("\n#########goin g to show the window\n");
	//window = GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, QN_AND_ANS_WINDOW));
	//window = GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, QN_AND_ANS_WINDOW));
	//window = GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, QN_AND_ANS_WINDOW));
	window = GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, QN_AND_ANS_WINDOW));
	g_print("\n#########window shown\n");
	gtk_widget_show_all(window);
	//cur_active_screen = qn_and_ans_scrn;
}

void hide_window_qna()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (QnAndAnsBuilder, QN_AND_ANS_WINDOW));
	gtk_widget_hide(window);
}

