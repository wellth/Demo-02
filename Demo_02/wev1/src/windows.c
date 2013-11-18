#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"



GtkBuilder *HomeBuilder;
GtkBuilder *FirstTimeProfileBuilder, *PrevWellthCheckBuilder, *CheckSymptomsBuilder;
GtkBuilder *EmailColleagueBuilder, *QuickWeCheckBuilder, *DependentGuestBuilder;


static int init_home_window = 0;
static int init_first_time_profile_window = 0;
static int init_prev_wellth_check_window = 0;
static int init_check_symptoms_window = 0;
static int init_email_colleague_window = 0;
static int init_quick_wellth_check_window = 0;
static int init_dependent_guest_window = 0;

static int isFirstTimeProfile = 0;
int isQuickWellthCheck = 0;
int profileBtnClickedFrom = 0;

const char ticker_text[] = "WeV1 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE     ";

gboolean continue_timer = FALSE;

/* Determines if the timer has started */
gboolean start_timer = FALSE;

/* Display seconds expired */
int sec_expired = 0;


//PROFILE_BTN_CLK profileBtnClickedFrom;


/* This function gets called when currently selected item changes */
static void
cb_changed( GtkComboBoxText *combo,
            gpointer     data )
{
    /* Obtain currently selected string from combo box */
    gchar *string = gtk_combo_box_text_get_active_text( combo );

    /* Print it to the console - if nothing is selected, print NULL */
    g_print( "Selected (simple): >> %s <<\n", ( string ? string : "NULL" ) );

    /* Free string */
    g_free( string );
}

/* This function deletes currently selected item from combo box */
static void
cb_delete( GtkButton   *button,
           GtkComboBoxText *combo )
{
    gint index;

    /* Get currently selected item's index */
    index = gtk_combo_box_text_get_active_text( combo );

    /* Remove this item from combo */
    gtk_combo_box_text_remove( combo, index );
}

void d_or_g_check_for_guest_btnclicked(GtkWidget *widget, gpointer user_data){
	hide_window_dependent_guest();
	show_window_test1();
	show_measure_btn_hide_repeate_btn();
}

void d_or_g_check_for_dependent_btnclicked (GtkWidget *widget, gpointer user_data){
	hide_window_dependent_guest();
	show_window_test1();
	show_measure_btn_hide_repeate_btn();
}


void home_exit_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_home ();
	show_window_login();
}

void home_complete_wellth_check_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_home ();
	isQuickWellthCheck = 0;
	//isFirstTimeProfile = 1;
	if(isFirstTimeProfile == 1){
		set_cur_test = CUR_TEST1;
		//start_timer = 0;
		show_window_test1();
		show_measure_btn_hide_repeate_btn();
	} else {
		show_window_first_time_profile ();
		isFirstTimeProfile = 1;
	}
}

void test_exit_btnclicked(GtkWidget *widget, gpointer user_data)
{
	switch(cur_active_screen){
		case test1_scrn:
			reset_test1_results();
			hide_window_test1();
			break;
		case test2_scrn:
			reset_test2_results();
			hide_window_test2();
			break;
		case test3_scrn:
			reset_test3_results();
			hide_window_test3();
			break;
		case test4_scrn:
			reset_test4_results();
			hide_window_test4();
			break;
		case test1_b_scrn:
			hide_window_test1_b();
			break;
		case test2_b_scrn:
			hide_window_test2_b();
			break;
		case test3_b_scrn:
			hide_window_test3_b();
			break;
		case test4_b_scrn:
			hide_window_test4_b();
			break;
		case check_symptoms_scrn:
			hide_window_check_symptoms();
			break;
		case first_time_profile_scrn:
			hide_window_first_time_profile ();
			break;
		case last_wellth_check_scrn:
			hide_window_prev_wellth_check ();
			break;
		case email_colleague_scrn:
			hide_window_email_colleague ();
			break;
		case quick_wellth_check_scrn:
			hide_window_quick_wellth_check ();
			break;
		case depenednt_or_guest_scrn:
			hide_window_dependent_guest ();
			break;
		case user_summary_scrn:
			hide_window_user_summary ();
			break;
		case summary_display_scrn:
			hide_window_health_summary ();
			break;
		default:
			break;
	}

	start_timer = 0;
	show_window_exit();
	//show_window_home();
	/*
	if (isQuickWellthCheck == 0) {
		show_window_home();
	} else {
		show_window_quick_wellth_check ();
	}
*/
}

void quick_wellth_check_exit_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_quick_wellth_check ();
	//show_window_home();
	isQuickWellthCheck = 0;
	show_window_exit();
}

void test_profile_btnclicked(GtkWidget *widget, gpointer user_data)
{
	int val;
	val = user_data;
	hide_window_test1();
	hide_window_test3();
	hide_window_test2();
	hide_window_test4();
	hide_window_check_symptoms();
	hide_window_first_time_profile ();
	hide_window_prev_wellth_check ();
	hide_window_email_colleague ();
	hide_window_quick_wellth_check ();
	hide_window_dependent_guest ();
	hide_window_home ();
	profileBtnClickedFrom = user_data;
	show_window_profile ();
	printf("%s %d \n", "The value of cs is: ", user_data);
	if( user_data == home_pro_btn){
		printf("%s %d \n", "The value of cs is: ", user_data);
	}
}

void home_quick_wellth_check_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_home ();
	show_window_quick_wellth_check ();
}
void home_access_last_wellth_check_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_home ();
	show_window_prev_wellth_check ();
}
void home_email_colleage_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_home ();
	show_window_email_colleague();
}
void home_check_symptoms_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_home ();
	show_window_check_symptoms ();
}

void home_dependent_guest_btnclicked(GtkWidget *widget, gpointer user_data){
	hide_window_home ();
	show_window_dependent_guest ();
}


void fst_time_profile_proceed_btnclicked (GtkWidget *widget, gpointer user_data)
{
	hide_window_first_time_profile ();
	show_window_test1();
	start_timer = 0;
	set_cur_test = CUR_TEST1;
	show_measure_btn_hide_repeate_btn();
}

void quick_wellth_check_test1_btnclicked (GtkWidget *widget, gpointer user_data)
{
	isQuickWellthCheck = 1;
	hide_window_quick_wellth_check ();
	show_window_test1();
	set_cur_test = CUR_TEST1;
	test1_hide_next_button_n_image();
	show_measure_btn_hide_repeate_btn();
}

void quick_wellth_check_test2_btnclicked (GtkWidget *widget, gpointer user_data)
{
	isQuickWellthCheck = 1;
	hide_window_quick_wellth_check ();
	show_window_test2();
	set_cur_test = CUR_TEST2;
	test2_hide_next_button_n_image();
	show_measure_btn_hide_repeate_btn();
}

void quick_wellth_check_test3_btnclicked (GtkWidget *widget, gpointer user_data)
{
	isQuickWellthCheck = 1;
	hide_window_quick_wellth_check ();
	show_window_test3();
	set_cur_test = CUR_TEST3;
	test3_hide_next_button_n_image();
	show_measure_btn_hide_repeate_btn();
}

void quick_wellth_check_test4_btnclicked (GtkWidget *widget, gpointer user_data)
{
	isQuickWellthCheck = 1;
	hide_window_quick_wellth_check ();
	show_window_test4();
	set_cur_test = CUR_TEST4;
	test4_hide_next_button_n_image();
	show_measure_btn_hide_repeate_btn();
}


void common_home_btnclicked(GtkWidget *widget, gpointer user_data)
{
	switch(cur_active_screen){
		case test1_scrn:
			reset_test1_results();
			hide_window_test1();
			break;
		case test2_scrn:
			reset_test2_results();
			hide_window_test2();
			break;
		case test3_scrn:
			reset_test3_results();
			hide_window_test3();
			break;
		case test4_scrn:
			reset_test4_results();
			hide_window_test4();
			break;
		case check_symptoms_scrn:
			hide_window_check_symptoms();
			break;
		case first_time_profile_scrn:
			hide_window_first_time_profile ();
			break;
		case last_wellth_check_scrn:
			hide_window_prev_wellth_check ();
			break;
		case email_colleague_scrn:
			hide_window_email_colleague ();
			break;
		case quick_wellth_check_scrn:
			hide_window_quick_wellth_check ();
			break;
		case depenednt_or_guest_scrn:
			hide_window_dependent_guest ();
			break;
		default:
			break;
	}

	show_window_home ();
}

static GtkWidget*
create_home_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5, *button6, *button7, *button8; 
	GtkImage *imageClient, *imageWeellth, *imageBG, *imageTest;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (HomeBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, HOME_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				HOME_WINDOW,
				HOME_UI_FILE);
        }
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
#endif
	
	button1 = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (home_exit_btnclicked),
                    NULL);

	profileBtnClickedFrom = home_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);

	button3 = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "quick_wellth_check"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (home_quick_wellth_check_btnclicked),
                    NULL);

	button4 = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "access_last_wellth_check"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (home_access_last_wellth_check_btnclicked),
                    NULL);
/*
	button5 = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "email_colleague"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (home_email_colleage_btnclicked),
                    NULL);

	button7 = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "check_symptoms"));
 	 g_signal_connect (G_OBJECT (button7), "clicked",
                    G_CALLBACK (home_check_symptoms_btnclicked),
                    NULL);
*/
			button6 = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "complete_wellth_check"));
 	 g_signal_connect (G_OBJECT (button6), "clicked",
                    G_CALLBACK (home_complete_wellth_check_btnclicked),
                    NULL);
	
	button8 = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "dependent_guest"));
 	 g_signal_connect (G_OBJECT (button8), "clicked",
                    G_CALLBACK (home_dependent_guest_btnclicked),
                    NULL);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_home()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_home_window) {
		   GError *error=NULL;
		HomeBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (HomeBuilder, HOME_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		printf("entering in creating creatre home window");
		create_home_window();
		init_home_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);

	StartTimer = 0;
	window = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, HOME_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = home_scrn;
}

void hide_window_home()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (HomeBuilder, HOME_WINDOW));
	gtk_widget_hide(window);
}



void hide_measure_btn_show_repeate_btn(){
	//GtkWidget *
	GtkWidget *measure_btn, *repeate_btn;
	switch(set_cur_test) {
		case CUR_TEST1:
			measure_btn = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "click_to_measure"));
			repeate_btn = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "repeate"));
			break;
		case CUR_TEST2:
			measure_btn = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "click_to_measure"));
			repeate_btn = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "repeate"));
			break;
		case CUR_TEST3:
			measure_btn = GTK_WIDGET (gtk_builder_get_object (Test3Builder, "click_to_measure"));
			repeate_btn = GTK_WIDGET (gtk_builder_get_object (Test3Builder, "repeate"));
			break;
		case CUR_TEST4:
			measure_btn = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "click_to_measure"));
			repeate_btn = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "repeate"));
			break;
	}
	gtk_widget_hide(measure_btn);
	gtk_widget_show(repeate_btn);
}

void show_measure_btn_hide_repeate_btn(){
	//GtkWidget *
	GtkWidget *measure_btn, *repeate_btn;
	switch(set_cur_test) {
		case CUR_TEST1:
			measure_btn = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "click_to_measure"));
			repeate_btn = GTK_WIDGET (gtk_builder_get_object (Test1Builder, "repeate"));
			break;
		case CUR_TEST2:
			measure_btn = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "click_to_measure"));
			repeate_btn = GTK_WIDGET (gtk_builder_get_object (Test2Builder, "repeate"));
			break;
		case CUR_TEST3:
			measure_btn = GTK_WIDGET (gtk_builder_get_object (Test3Builder, "click_to_measure"));
			repeate_btn = GTK_WIDGET (gtk_builder_get_object (Test3Builder, "repeate"));
			break;
		case CUR_TEST4:
			measure_btn = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "click_to_measure"));
			repeate_btn = GTK_WIDGET (gtk_builder_get_object (Test4Builder, "repeate"));
			break;
	}
	gtk_widget_hide(repeate_btn);
	gtk_widget_show(measure_btn);
}

typedef struct adjustment_tag {
	float value;
	float lower;
	float upper;
	float step_increment;
	float page_increment;
	float page_size;
}MyGtkAdjustment;

static GtkWidget*
create_first_time_profile_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5, *box, *q2_box; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;
//	GtkWidget *radiob1, *radiob2, *radiob3, *radiob4, *radiob5, *radiob6, *radiob7, *radiob8;
	GtkWidget *lout1, *lbl5;
	GError** error = NULL;
	//box = gtk_vbox_new (TRUE, 3);

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (FirstTimeProfileBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, FST_TIME_PROFILE_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				FST_TIME_PROFILE_WINDOW,
				FST_TP_UI_FILE);
        }
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);
#endif
	GdkPixbufAnimation * pixbuf2 = gdk_pixbuf_animation_new_from_file(HM_NEXT_ICON, error);
	GtkImage * nxt_icon = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "next_icon"));
	gtk_image_set_from_animation(GTK_IMAGE(nxt_icon), pixbuf2);
	
	button1 = GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);
	profileBtnClickedFrom = start_test_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);
	
	button3 = GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "proceed"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (fst_time_profile_proceed_btnclicked),
					NULL);
	//"Q.1: Do any of your parent(s) have diabetes"
			
	//lout1 = GTK_WIDGET(gtk_builder_get_object (FirstTimeProfileBuilder, "layout2"));
	GtkWidget *vbox1 = gtk_vbox_new( FALSE, 6 );
	gtk_container_add( GTK_CONTAINER(window), vbox1 );
		GtkLayout *qnlayout = (GtkLayout *)GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder,"layout1"));
	gtk_box_pack_start( GTK_BOX( vbox1 ), qnlayout, FALSE, FALSE, 0 );
	
	GtkLayout *glayout = (GtkLayout *)GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder,"layout2"));
	gtk_box_pack_start( GTK_BOX( vbox1 ), glayout, FALSE, FALSE, 0 );

	lbl5 = gtk_label_new ("");
	//lbl5 = GTK_WIDGET(gtk_builder_get_object (FirstTimeProfileBuilder, "qn1"));
	gtk_label_set_text ( lbl5, "Q.1: Do any of your parent(s) have diabetes");
	gtk_container_add( GTK_CONTAINER(qnlayout ), lbl5 );

	 box = gtk_vbox_new (TRUE, 1);
	gtk_container_set_border_width (GTK_CONTAINER (box), 5);

	
   /* Create a radio button with a GtkEntry widget */
   GtkWidget * radio1 = gtk_radio_button_new (NULL);
   GtkWidget * lblss = gtk_label_new ("Mother");
   gtk_container_add (GTK_CONTAINER (radio1), lblss);
   
   
   /* Create a radio button with a label */
   GtkWidget * radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1),
							"Father");

	GtkWidget * radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1),
							"Both Parents");
	/*GtkWidget * radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1),
							"I'm the second radio button.");
	GtkWidget * radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1),
							"I'm the second radio button.");
   */
   /* Pack them into a box, then show all the widgets */
   gtk_box_pack_start (GTK_BOX (box), radio1, FALSE, FALSE, 1);
   gtk_box_pack_start (GTK_BOX (box), radio2, FALSE, FALSE, 1);
	gtk_box_pack_start (GTK_BOX (box), radio3, TRUE, TRUE, 1);
  // gtk_box_pack_start (GTK_BOX (box), radio4, TRUE, TRUE, 1);
	//gtk_box_pack_start (GTK_BOX (box), radio5, TRUE, TRUE, 1);
   gtk_container_add (GTK_CONTAINER (glayout), box);

#if 0
//	Adj = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW(scrollbar));
	GtkAdjustment*  adjustment = gtk_adjustment_new (0, 0,6,1,1,3);
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW (gtk_builder_get_object (FirstTimeProfileBuilder, "scrolledwindow1"));
	//MyGtkAdjustment*  /*adjustment*/Adj =  GTK_ADJUSTMENT (gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW(scrolled_window)));

//	gtk_widget_set_usize(scrolled_window, 250, 150);
	gtk_scrolled_window_set_vadjustment(scrolled_window, adjustment);
	gtk_container_add(GTK_CONTAINER(box), scrolled_window);
	gtk_widget_show(scrolled_window);
#endif

	//"Q.1: Do you smoke ?"
			
	//lout1 = GTK_WIDGET(gtk_builder_get_object (FirstTimeProfileBuilder, "layout2"));
	GtkWidget *vbox2 = gtk_vbox_new( FALSE, 6 );
	gtk_container_add( GTK_CONTAINER(window), vbox2 );
	GtkLayout *q2_qnlayout = (GtkLayout *)GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder,"layout3"));
	gtk_box_pack_start( GTK_BOX( vbox2 ), q2_qnlayout, FALSE, FALSE, 0 );
	
	GtkLayout *q2_glayout = (GtkLayout *)GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder,"layout4"));
	gtk_box_pack_start( GTK_BOX( vbox2 ), q2_glayout, FALSE, FALSE, 0 );

	lbl5 = gtk_label_new ("");
	//lbl5 = GTK_WIDGET(gtk_builder_get_object (FirstTimeProfileBuilder, "qn1"));
	gtk_label_set_text ( lbl5, "Q.2: Do you smoke ?");
	gtk_container_add( GTK_CONTAINER(q2_qnlayout ), lbl5 );

	 q2_box = gtk_vbox_new (TRUE, 1);
	gtk_container_set_border_width (GTK_CONTAINER (q2_box), 5);

	
   /* Create a radio button with a GtkEntry widget */
   GtkWidget * q2_radio1 = gtk_radio_button_new (NULL);
   GtkWidget * q2_lblss = gtk_label_new ("<5 cigarettes / day");
   gtk_container_add (GTK_CONTAINER (q2_radio1), q2_lblss);
   
   
   /* Create a radio button with a label */
   GtkWidget * q2_radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (q2_radio1),
							"5-10 cigs / days");

	GtkWidget * q2_radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (q2_radio1),
							">10 cigs dailly");
	/*GtkWidget * radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1),
							"I'm the second radio button.");
	GtkWidget * radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1),
							"I'm the second radio button.");
   */
   /* Pack them into a box, then show all the widgets */
   gtk_box_pack_start (GTK_BOX (q2_box), q2_radio1, FALSE, FALSE, 1);
   gtk_box_pack_start (GTK_BOX (q2_box), q2_radio2, FALSE, FALSE, 1);
	gtk_box_pack_start (GTK_BOX (q2_box), q2_radio3, TRUE, TRUE, 1);
  // gtk_box_pack_start (GTK_BOX (box), radio4, TRUE, TRUE, 1);
	//gtk_box_pack_start (GTK_BOX (box), radio5, TRUE, TRUE, 1);
   gtk_container_add (GTK_CONTAINER (q2_glayout), q2_box);

	
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_first_time_profile()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_first_time_profile_window) {
		GError *error=NULL;
		FirstTimeProfileBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (FirstTimeProfileBuilder, FST_TP_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_first_time_profile_window();
		init_first_time_profile_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, FST_TIME_PROFILE_WINDOW));
	gtk_widget_show_all(window);
	cur_active_screen = first_time_profile_scrn;
}

void hide_window_first_time_profile()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (FirstTimeProfileBuilder, FST_TIME_PROFILE_WINDOW));
	gtk_widget_hide(window);
}

static GtkWidget*
create_prev_wellth_check_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (PrevWellthCheckBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, PREV_WE_CHECK_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				PREV_WE_CHECK_WINDOW,
				PREV_WE_CHECK_UI_FILE);
        }
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);
#endif
	
	button1 = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);

	profileBtnClickedFrom = last_wellth_check_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);
	
	button3 = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (common_home_btnclicked),
                    NULL);

#if 0
	button4 = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "set"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (test4_set_btnclicked),
                    NULL);

	button5 = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test4_home_btnclicked),
                    NULL);
#endif
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_prev_wellth_check()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_prev_wellth_check_window) {
		   GError *error=NULL;
		PrevWellthCheckBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (PrevWellthCheckBuilder, PREV_WE_CHECK_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_prev_wellth_check_window();
		init_prev_wellth_check_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, PREV_WE_CHECK_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = last_wellth_check_scrn;
	
}

void hide_window_prev_wellth_check()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (PrevWellthCheckBuilder, PREV_WE_CHECK_WINDOW));
	gtk_widget_hide(window);
}

static GtkWidget*
create_check_symptoms_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (CheckSymptomsBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, CHECK_SYMPTOMS_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				CHECK_SYMPTOMS_WINDOW,
				CHECK_SYMPTOMS_UI_FILE);
        }
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

	GdkPixbufAnimation * pixbufTestanm = gdk_pixbuf_animation_new_from_file(SYMPTOMS, error);
	imageTest = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(imageTest), pixbufTestanm);
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);
#endif
	
	button1 = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);
	profileBtnClickedFrom = check_symptoms_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);
	
	button3 = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (common_home_btnclicked),
                    NULL);

#if 0
	button4 = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "set"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (test4_set_btnclicked),
                    NULL);

	button5 = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test4_home_btnclicked),
                    NULL);
#endif
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_check_symptoms()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_check_symptoms_window) {
		   GError *error=NULL;
		CheckSymptomsBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (CheckSymptomsBuilder, CHECK_SYMPTOMS_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_check_symptoms_window();
		init_check_symptoms_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, CHECK_SYMPTOMS_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = check_symptoms_scrn;
}

void hide_window_check_symptoms()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (CheckSymptomsBuilder, CHECK_SYMPTOMS_WINDOW));
	gtk_widget_hide(window);
}

static GtkWidget*
create_email_colleague_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (EmailColleagueBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, EMAIL_COLLEAGUE_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				EMAIL_COLLEAGUE_WINDOW,
				EMAIL_COLLEAGUE_UI_FILE);
        }
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);
#endif
	
	button1 = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);
	
	button2 = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    NULL);
	profileBtnClickedFrom = email_colleague_pro_btn;
	button3 = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (common_home_btnclicked),
                    profileBtnClickedFrom);

#if 0
	button4 = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "set"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (test4_set_btnclicked),
                    NULL);

	button5 = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (test4_home_btnclicked),
                    NULL);
#endif
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_email_colleague()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_email_colleague_window) {
		   GError *error=NULL;
		EmailColleagueBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (EmailColleagueBuilder, EMAIL_COLLEAGUE_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_email_colleague_window();
		init_email_colleague_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, EMAIL_COLLEAGUE_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = email_colleague_scrn;
}

void hide_window_email_colleague()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (EmailColleagueBuilder, EMAIL_COLLEAGUE_WINDOW));
	gtk_widget_hide(window);
}

static GtkWidget*
create_quick_wellth_check_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5, *button6, *button7; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (QuickWeCheckBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, QUICK_WE_CHECK_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				QUICK_WE_CHECK_WINDOW,
				QUICK_WE_CHECK_UI_FILE);
        }
/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);
#endif
	
	button1 = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (quick_wellth_check_exit_btnclicked),
                    NULL);

	profileBtnClickedFrom = quick_wellth_check_pro_btn;
	button2 = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "profile"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (test_profile_btnclicked),
                    profileBtnClickedFrom);
	
	button3 = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (common_home_btnclicked),
                    NULL);

	button4 = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "weight_bmi"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (quick_wellth_check_test1_btnclicked),
                    NULL);

	button5 = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "blood_pressure"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (quick_wellth_check_test4_btnclicked),
                    NULL);

	button6 = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "body_fat_composition"));
 	 g_signal_connect (G_OBJECT (button6), "clicked",
                    G_CALLBACK (quick_wellth_check_test2_btnclicked),
                    NULL);

	button7 = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "smoking_status"));
 	 g_signal_connect (G_OBJECT (button7), "clicked",
                    G_CALLBACK (quick_wellth_check_test3_btnclicked),
                    NULL);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_quick_wellth_check()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_quick_wellth_check_window) {
		   GError *error=NULL;
		QuickWeCheckBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (QuickWeCheckBuilder, QUICK_WE_CHECK_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_quick_wellth_check_window();
		init_quick_wellth_check_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, QUICK_WE_CHECK_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = quick_wellth_check_scrn;
}

void hide_window_quick_wellth_check()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (QuickWeCheckBuilder, QUICK_WE_CHECK_WINDOW));
	gtk_widget_hide(window);  
}


static GtkWidget*
create_dependent_guest_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4; 
	GtkImage *imageClient, *imageWellth, *imageTest, *imageBG;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (DependentGuestBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, DEPENDENT_GUEST_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				DEPENDENT_GUEST_WINDOW,
				DEPENDENT_GUEST_UI_FILE);
        }

/*
	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
*/
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *adds = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(adds), pixbufaddanm);
#endif
	
	button1 = GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (test_exit_btnclicked),
                    NULL);
	
	button2 = GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "check_for_dependent"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (d_or_g_check_for_dependent_btnclicked),
                    NULL);
	
	button3 = GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "check_for_guest"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (d_or_g_check_for_guest_btnclicked),
                    NULL);

	button4 = GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "home"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (common_home_btnclicked),
                    NULL);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_dependent_guest ()
{
	GtkWidget *window, *label;
	char lbl_buf[64];
	if(!init_dependent_guest_window) {
		   GError *error=NULL;
		DependentGuestBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (DependentGuestBuilder, DEPENDENT_GUEST_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_dependent_guest_window();
		init_dependent_guest_window = 1;
	}
	label = GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	window = GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, DEPENDENT_GUEST_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = depenednt_or_guest_scrn;
}

void hide_window_dependent_guest()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (DependentGuestBuilder, DEPENDENT_GUEST_WINDOW));
	gtk_widget_hide(window);  
}

