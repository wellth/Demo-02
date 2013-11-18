/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) 2012 kiran <kiran@Kiran>
 * 
 * WeV1 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * WeV1 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
//#include <stdio.h>
//#include <stdlib.h>
#include <sqlite3.h>

#include <config.h>
#include <gtk/gtk.h>

#include <glib/gi18n.h>
#include "window_defs_n_config.h"
#include "main_test_backend.h"


static int init_second_window = 0;
static int init_login_window = 0;
static int init_signup_window = 0;
static int init_mand_msg_window = 0;


char GlobalUserName[64];
ACTIVE_SCREEN cur_active_screen;
DEMON_RETURN_STATUS demon_return_status;
_USER_SUMMARY_DATA UserSummaryData;
static GtkComboBoxText *gender;

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/WindowMain.ui" */
#define UI_FILE				"src/wev1.ui"
#define INITIAL_UI_FILE		"src/weInit.ui"
#define LOGIN_UI_FILE		"uiScreens/demo_login.ui"
#define SIGNUP_UI_FILE		"src/weSignUp.ui"
#define MAND_MSG_UI_FILE	"src/weMandMsg.ui"

//#define CLIENT_LOGO "/home/kiran/Downloads/logo.gif"
//#define WELLTH_LOGO "/home/kiran/Downloads/WeLogoFin.png"

//#define CLIENT_LOGO "/Images/logo.gif"
//#define WELLTH_LOGO "/Images/logo.gif"


#define TOP_WINDOW		"window_walkin"
#define INITIAL_WINDOW  "window_init"
#define	LOGIN_WINDOW	"window_login"
#define SIGNUP_WINDOW   "window_signup"
#define MAND_MSG_WINDOW "window_mand_msg"

GtkBuilder *MainBuilder, *InitialBuilder, *LoginBuilder, *SignupBuilder, *MandMsgBuilder;
GtkBuilder *DummyBuilder, *KeyPadBuilder;
GtkWidget *KeyPadFrameWidget;
int focused_text_entry = 0;
char ph_no_str[16],age_str[16];
/* Signal handlers */
/* Note: These may not be declared static because signal autoconnection
 * only works with non-static methods
 */

/* Called when the window is closed */


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

static void
cb_changed( GtkComboBox *combo,
            gpointer     data )
{
	int i = 0;
    /* Obtain currently selected string from combo box */
    gchar *string = gtk_combo_box_text_get_active_text( gender );
	gtk_combo_box_set_active_id(gender, string);
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

void
destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit ();
}

void hide_window_main()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (MainBuilder, TOP_WINDOW));
	gtk_widget_hide(window);
}

void hide_window_second()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (InitialBuilder, INITIAL_WINDOW));
	gtk_widget_hide(window);
}

void hide_window_login()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, LOGIN_WINDOW));
	gtk_widget_hide(window);
}

void hide_window_mand_msg()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, MAND_MSG_WINDOW));
	gtk_widget_hide(window);
}

void hide_window_signup()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (SignupBuilder, SIGNUP_WINDOW));
	gtk_widget_hide(window);
}

void login_cancel_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_login();
	show_window_second();
}

void key_pad_callback(GtkWidget *widget, gpointer user_data){
	int number = (int)user_data;
	char numstr[10];
	int len;
	
	g_print("\ncallback number is = %d focus = %d\n", number,focused_text_entry);
	if (focused_text_entry == 1)
	{
		if ((number <= 9) && (number >= 0))
		{
			numstr[0] = number + 48;
			numstr[1] = '\0';
			strcat (ph_no_str,numstr);
			ph_no_str[10] = '\0';
		}
		else
		{
			len = strlen(ph_no_str);
			if (len != 0)
				ph_no_str[len - 1] = '\0';
		}
		// Display phone no str
		GtkWidget *ph_no_obj = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "ph_no"));	
		gtk_entry_set_text(ph_no_obj, ph_no_str);
	}
	else if (focused_text_entry == 2)
	{
		
		if ((number <= 10) && (number >= 0))
		{
			numstr[0] = number + 48;
			numstr[1] = '\0';
			strcat (age_str,numstr);
			age_str[2] = '\0';
		}
		else
		{
			len = strlen(age_str);
			if (len != 0)
				age_str[len - 1] = '\0';
		}
		// Display Age string
		GtkWidget *age_obj = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "age"));	
		gtk_entry_set_text(age_obj, age_str);
	}
}

void set_ph_no_active(GtkWidget *widget, gpointer user_data){
	focused_text_entry = 1; //1=> phone number text entry
	g_print("Ph_no activated \n");
	GtkWidget *ph_no_obj = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "ph_no"));	
//	gtk_widget_set_can_focus (ph_no_obj, TRUE)	gtk_entry_set_icon_from_pixbuf ()
}
void set_age_active(GtkWidget *widget, gpointer user_data){
	focused_text_entry = 2; //2=> age text entry
	g_print("Age activated \n");
}

void show_soft_keypad(){
	GtkWidget * key_pad;
	key_pad = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "frame1"));
	gtk_widget_show(key_pad);
}
void signup_cancel_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_signup();
	show_window_second();
}


void login_button_clicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_second();
	show_window_login ();
}
void signup_button_clicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_second();
	show_window_signup();
}

void signup_login_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_signup();
	show_window_login ();
}

void signup_go_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_signup();
	show_window_mand_msg ();
}

void _start_ticker_timer()
{
	
   //if(!start_timer)
    {
        //g_timeout_add_seconds(1, _label_update, label);
		//g_timeout_add(150, ScrollTickerText, NULL);
		g_timeout_add (150, playAudioFiles, NULL);
        start_timer = TRUE;
        continue_timer = TRUE;
    }
}

void login_go_btnclicked(GtkWidget *widget, gpointer user_data)
{

	gchar *buf, *string;
	GtkWidget *UsrName, *PWD, *login_status;
	char *UserName, *pwd;
	char WriteBuf[MAX_BYTE_DATA_IN_ENTRY_FILE];
	FILE *fp;
	_WT_IN_FILE InText;

	UserName = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "ph_no"));
	PWD = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "age"));
	UsrName = gtk_entry_get_text (UserName);
	pwd = gtk_entry_get_text (PWD);
	string = gtk_combo_box_text_get_active_text( gender );

	if( (strcmp(UsrName, "")  == 0) || (strcmp(pwd, "")  == 0) || (strlen(UsrName) < 10) ){
	//if(UsrName == NULL || pwd == NULL) {
		login_status = GTK_WIDGET (gtk_builder_get_object(LoginBuilder, "login_status"));
		gtk_label_set_label (login_status, "please input all the information");
		return;
	}
	login_status = GTK_WIDGET (gtk_builder_get_object(LoginBuilder, "login_status"));
	gtk_label_set_label (login_status, "");
	
	printf("ip is ready \n");
	fp = fopen (IN_TEXT_FILE, "w");
	if(fp<0){
		printf(" System is not able to create file \n");
	}
	else{
		printf("ip is ready \n");
		ClearOutPutFile();
		//WriteEntryFile();
		strcpy(InText.InBuf,"Login");
		
		strcpy(WriteBuf, UsrName);
		strcat(WriteBuf, ",");
		strcat(WriteBuf, pwd);
		strcat(WriteBuf, ",");
		if (string)	 
			strcat(WriteBuf, string);
		else
			strcat(WriteBuf, "UnSpec");
		WriteDataFromUI(WriteBuf);
		
		fwrite(&InText,sizeof(InText),1,fp);
		printf("File has been Updated for login \n");
		fclose(fp);	
		StartTimer = 1;
		TimerInit();
	}
}



void login_signup_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_login();
	show_window_signup();
}

void login_clnt_admin_btnclicked(GtkWidget *widget, gpointer user_data){
	hide_window_login();
	show_window_client_admin();
}

void login_sys_admin_btnclicked(GtkWidget *widget, gpointer user_data){
	hide_window_login();
	show_window_system_admin();
}

void mand_msg_exit_btnclicked(GtkWidget *widget, gpointer user_data)
{
	hide_window_mand_msg();
	show_window_home();
	sec_expired = 0;
	start_timer = 0;
	continue_timer = TRUE;
	//show_window_test1();
	//create_dummy_window();
}

typedef struct
{       
        GtkWidget               *entry;
        GtkWidget               *textview;
} MyWidgets;
/*
void start_button_clicked (GtkWidget *widget, GdkEventButton * event, gpointer user_data)
{
	if(event->type == GDK_BUTTON_PRESS && event->button == 3){
		hide_window_main();
		show_window_second();
	}
}
*/

void start_button_clicked(GtkWidget *widget, gpointer user_data){
	hide_window_main();
		show_window_second();
}

void back_button_clicked (GtkWidget *widget, gpointer user_data)
{	
	show_window_main();
	hide_window_second();
}

void show_window_main()
{
	GtkWidget *window;
	window = GTK_WIDGET (gtk_builder_get_object (MainBuilder, TOP_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = wev1_scrn;
}

static GtkWidget*
create_window2 (void)
{
	GtkWidget *window, *button1, *button2, *button3, *ticker; 
	GtkImage *imageClient, *imageWellth, *imageBG;
	

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (InitialBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (InitialBuilder, INITIAL_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				INITIAL_WINDOW,
				INITIAL_UI_FILE);
        }

	//GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	//imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (InitialBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (InitialBuilder, "wellth_logo"));
	//gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG2, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (InitialBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

	GdkPixbufAnimation * pixbufswipecard = gdk_pixbuf_animation_new_from_file(SMART_CARD, error);
	GtkImage *sc = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (InitialBuilder, "image2"));
	gtk_image_set_from_animation(GTK_IMAGE(sc), pixbufswipecard);
	
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (InitialBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
#endif
//	button1 = GTK_WIDGET (gtk_builder_get_object (InitialBuilder, "back"));
// 	 g_signal_connect (G_OBJECT (button1), "clicked",
//                    G_CALLBACK (back_button_clicked),
//                   NULL);


GdkColor color;

gdk_color_parse ("#FF0000", &color);

gtk_widget_modify_bg ( GTK_WIDGET(button1), GTK_STATE_NORMAL, &color);

	
	button2 = GTK_WIDGET (gtk_builder_get_object (InitialBuilder, "login"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (login_button_clicked),
                    NULL);
	//button3 = GTK_WIDGET (gtk_builder_get_object (InitialBuilder, "sign_up"));
 	// g_signal_connect (G_OBJECT (button3), "clicked",
                  // G_CALLBACK (signup_button_clicked),
                  //  NULL);
	//ticker = GTK_WIDGET (gtk_builder_get_object(InitialBuilder, "entry1"));
	//gtk_entry_set_text(ticker, ticker_text);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

static GtkWidget*
create_login_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4, *button5, *button6, *phone_number, *age; 
	GtkImage *imageClient, *imageWellth, *imageBG;
	GtkWidget *vbox1;

	GtkWidget *butno1, *butno2,*butno3,*butno4,*butno5,*butno6,*butno7,*butno8,*butno9,*butno0,*butno_bs; 
	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (LoginBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, LOGIN_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				LOGIN_WINDOW,
				LOGIN_UI_FILE);
        }

	//GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	//imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "wellth_logo"));
	//gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG1, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);

#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
#endif
/*		
	button1 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "back"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (login_cancel_btnclicked),
                    NULL);
	*/
	
	button2 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "cancel"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (login_cancel_btnclicked),
                    NULL);

	phone_number = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "ph_no"));
	 g_signal_connect (G_OBJECT (phone_number), "icon-press",
                    G_CALLBACK (set_ph_no_active),
                    NULL);

	age = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "age"));
 	 g_signal_connect (G_OBJECT (age), "icon-press",
                    G_CALLBACK (set_age_active),
                    NULL);

	vbox1 = gtk_vbox_new( FALSE, 6 );
    gtk_container_add( GTK_CONTAINER( window ), vbox1 );
	
	GtkFrame *gvp = (GtkFrame *)GTK_WIDGET (gtk_builder_get_object (LoginBuilder,"frame1"));
	
	//gtk_box_pack_start( GTK_BOX( vbox1 ), KeyPadFrameWidget, FALSE, FALSE, 0 );
	gtk_box_pack_start( GTK_BOX( vbox1 ), gvp, FALSE, FALSE, 0 );
	//GtkComboBoxText *height_list = gtk_combo_box_text_new();
   

	gender = gtk_combo_box_text_new();
    gtk_container_add( GTK_CONTAINER( gvp ), gender );
	gtk_combo_box_text_append_text((GtkComboBoxText *)(gender), (const gchar *)"Male");
	gtk_combo_box_text_append_text((GtkComboBoxText *)(gender), (const gchar *)"Female");

	g_signal_connect( G_OBJECT( gender ), "changed", G_CALLBACK( cb_changed ), NULL );
	
	/*
	button3 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "sign_up"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (login_signup_btnclicked),
                    NULL);
*/
	button4 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "go"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (login_go_btnclicked),
                    NULL);

		button5 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "clnt_admin"));
 	 g_signal_connect (G_OBJECT (button5), "clicked",
                    G_CALLBACK (login_clnt_admin_btnclicked),
                    NULL);

	button6 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "sys_admin"));
 	 g_signal_connect (G_OBJECT (button6), "clicked",
                    G_CALLBACK (login_sys_admin_btnclicked),
                    NULL);
	butno1 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button1"));
	g_signal_connect (G_OBJECT (butno1), "clicked", G_CALLBACK (key_pad_callback), 1);
	butno2 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button2"));
	g_signal_connect (G_OBJECT (butno2), "clicked", G_CALLBACK (key_pad_callback), 2);
	butno3 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button3"));
	g_signal_connect (G_OBJECT (butno3), "clicked", G_CALLBACK (key_pad_callback), 3);
	butno4 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button4"));
	g_signal_connect (G_OBJECT (butno4), "clicked", G_CALLBACK (key_pad_callback), 4);
	butno5 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button5"));
	g_signal_connect (G_OBJECT (butno5), "clicked", G_CALLBACK (key_pad_callback), 5);
	butno6 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button6"));
	g_signal_connect (G_OBJECT (butno6), "clicked", G_CALLBACK (key_pad_callback), 6);
	butno7 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button7"));
	g_signal_connect (G_OBJECT (butno7), "clicked", G_CALLBACK (key_pad_callback), 7);
	butno8 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button8"));
	g_signal_connect (G_OBJECT (butno8), "clicked", G_CALLBACK (key_pad_callback), 8);
	butno9 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button9"));
	g_signal_connect (G_OBJECT (butno9), "clicked", G_CALLBACK (key_pad_callback), 9);
	butno0 = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button10"));
	g_signal_connect (G_OBJECT (butno0), "clicked", G_CALLBACK (key_pad_callback), 0);
	butno_bs = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "button11")); 
	g_signal_connect (G_OBJECT (butno_bs), "clicked", G_CALLBACK (key_pad_callback), 100);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

static GtkWidget*
create_mand_msg_window (void)
{
	GtkWidget *window, *button1, *label;// *button2, *button3; 
	GtkImage *imageClient, *imageWellth, *imageBG;
	char lbl_buf[64];

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (MandMsgBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, MAND_MSG_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				MAND_MSG_WINDOW,
				MAND_MSG_UI_FILE);
        }

	//GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	//imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, "wellth_logo"));
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, "image1"));
	//gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageBG), pixbufBGanm);
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
#endif

	button1 = GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, "exit"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (mand_msg_exit_btnclicked),
                    NULL);
	
	label = GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, "user_name"));
	strcpy(lbl_buf, "Hi ");
	strcat(lbl_buf, GlobalUserName);
	gtk_label_set_label (label, lbl_buf);
	
	//button2 = GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, "back"));
 	// g_signal_connect (G_OBJECT (button2), "clicked",
      //              G_CALLBACK (mand_msg_exit_btnclicked),
        //            NULL);
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

static GtkWidget*
create_signup_window (void)
{
	GtkWidget *window, *button1, *button2, *button3, *button4; 
	GtkImage *imageClient, *imageWellth, *imageBG;

	GError** error = NULL;
	

	/* Load UI from file */

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (SignupBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (SignupBuilder, SIGNUP_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				SIGNUP_WINDOW,
				SIGNUP_UI_FILE);
        }

	GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (SignupBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (SignupBuilder, "wellth_logo"));
	gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG1, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (SignupBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
	
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (SignupBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
#endif
	
/*	button1 = GTK_WIDGET (gtk_builder_get_object (SignupBuilder, "back"));
 	 g_signal_connect (G_OBJECT (button1), "clicked",
                    G_CALLBACK (signup_cancel_btnclicked),
                    NULL);
*/	
	button2 = GTK_WIDGET (gtk_builder_get_object (SignupBuilder, "cancel"));
 	 g_signal_connect (G_OBJECT (button2), "clicked",
                    G_CALLBACK (signup_cancel_btnclicked),
                    NULL);
	
	button3 = GTK_WIDGET (gtk_builder_get_object (SignupBuilder, "login"));
 	 g_signal_connect (G_OBJECT (button3), "clicked",
                    G_CALLBACK (signup_login_btnclicked),
                    NULL);
	button4 = GTK_WIDGET (gtk_builder_get_object (SignupBuilder, "go"));
 	 g_signal_connect (G_OBJECT (button4), "clicked",
                    G_CALLBACK (signup_go_btnclicked),
                    NULL);
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (InitialBuilder);
	
	return window;
}

void show_window_login()
{
	GtkWidget *window, *key_pad;

	FILE*fp;
	_WT_IN_FILE InText;
	
	fp = fopen (IN_TEXT_FILE, "w");
	if(fp<0){
		printf(" System is not able to create file \n");
	}
	else{
		printf("swipe is erased \n");
		ClearOutPutFile();
		//WriteEntryFile();
		//strcpy(InText.InBuf,"Swipe");
		
		//fwrite(&InText,sizeof(InText),1,fp);
		//printf("File has been Updated for login \n");
		fclose(fp);	
		//TimerInit();
	}
	
	if(!init_login_window) {
		   GError *error=NULL;
		LoginBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (LoginBuilder, LOGIN_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_login_window();
		init_login_window = 1;
	}
	window = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, LOGIN_WINDOW));
	gtk_widget_show_all(window);
	memset(ph_no_str,0, sizeof(ph_no_str));
	memset(age_str,0, sizeof(age_str));
	GtkWidget *ph_no_obj = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "ph_no"));	
	gtk_entry_set_text(ph_no_obj, "");
	//key_pad = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "frame1"));
	GtkWidget *age_obj = GTK_WIDGET (gtk_builder_get_object (LoginBuilder, "age"));	
	gtk_entry_set_text(age_obj, "");
	//gtk_widget_hide (key_pad);
	cur_active_screen = login_scrn;
	StartTimer = 0;
}

void show_window_signup()
{
	GtkWidget *window;
	if(!init_signup_window) {
		   GError *error=NULL;
		SignupBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (SignupBuilder, SIGNUP_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_signup_window ();
		init_signup_window = 1;
	}
	window = GTK_WIDGET (gtk_builder_get_object (SignupBuilder, SIGNUP_WINDOW));
	gtk_widget_show(window);
}

void show_window_mand_msg()
{
	GtkWidget *window;
	if(!init_mand_msg_window) {
		   GError *error=NULL;
		MandMsgBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (MandMsgBuilder, MAND_MSG_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_mand_msg_window ();
		init_mand_msg_window = 1;
	}
	
	window = GTK_WIDGET (gtk_builder_get_object (MandMsgBuilder, MAND_MSG_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = mand_msg_scrn;
}
#ifndef BG_DEFINED
#define BG_DEFINED
GdkPixbufAnimation * pixbufBGanm;
#endif

void show_window_second()
{
	GtkWidget *window;

	FILE*fp;
	_WT_IN_FILE InText;
	
	fp = fopen (IN_TEXT_FILE, "w");
	if(fp<0){
		printf(" System is not able to create file \n");
	}
	else{
		printf("ip is ready \n");
		ClearOutPutFile();
		//WriteEntryFile();
		strcpy(InText.InBuf,"Swipe");
		
		fwrite(&InText,sizeof(InText),1,fp);
		printf("File has been Updated for login \n");
		fclose(fp);
		StartTimer = 1;
		TimerInit();

	}
	
	if(!init_second_window) {
		   GError *error=NULL;
		InitialBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (InitialBuilder, INITIAL_UI_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		create_window2();
		init_second_window = 1;
	}
	window = GTK_WIDGET (gtk_builder_get_object (InitialBuilder, INITIAL_WINDOW));
	gtk_widget_show(window);
	cur_active_screen = initial_scrn;
	//cur_active_screen = login_scrn;
}



static GtkWidget*
create_window1 (void)
{
	GtkWidget *window, *button1; 
	GtkImage *imageClient, *imageWellth, *imageBG;	
	GError** error = NULL;
	GTimeVal  time;
	GDate    *date_heap;
	
	GDate     date_stack;
	gchar     tmp_buffer[256];

	g_get_current_time( &time );
	date_heap = g_date_new();

	GDate* mod_date = g_date_new ();
	
	g_date_set_time_val( date_heap, &time );
	g_date_strftime( tmp_buffer, 256, "%x", date_heap );
	g_print( "Current date (heap):  %s\n", tmp_buffer );

	//g_date_set_year (mod_date, atoi (parts[2]));
	
	g_date_free( date_heap );
	
	/* Load UI from file */
	MainBuilder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (MainBuilder, UI_FILE, error))
	{
		g_critical ("Couldn't load builder file: %s", (*error)->message);
		g_error_free (*error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (MainBuilder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (MainBuilder, TOP_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TOP_WINDOW,
				UI_FILE);
        }

	//GdkPixbufAnimation * pixbufclntanm = gdk_pixbuf_animation_new_from_file(CLIENT_LOGO, error);
	GdkPixbufAnimation * pixbufwlthanm = gdk_pixbuf_animation_new_from_file(WELLTH_LOGO, error);
	//imageClient = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (MainBuilder, "clnt_logo"));
	imageWellth = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (MainBuilder, "wellth_logo"));
	//gtk_image_set_from_animation(GTK_IMAGE(imageClient), pixbufclntanm);
	gtk_image_set_from_animation (GTK_IMAGE(imageWellth), pixbufwlthanm);

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG1, error);
	imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (MainBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
#ifdef DISPLAY_ADD
	GdkPixbufAnimation * pixbufaddanm = gdk_pixbuf_animation_new_from_file(ADDS2, error);
	GtkImage *add = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (MainBuilder, "adds"));
	gtk_image_set_from_animation(GTK_IMAGE(add), pixbufaddanm);
#endif
	button1 = GTK_WIDGET (gtk_builder_get_object (MainBuilder, "main_start_btn"));
 	 g_signal_connect (G_OBJECT (button1), "clicked", G_CALLBACK (start_button_clicked), NULL);

		
	
	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for WindowMain.ui - DO NOT REMOVE */

	//g_object_unref (MainBuilder);
	
	return window;
}
#if 1
int
main (int argc, char *argv[])
{
 	GtkWidget *window;
	GError** error = NULL;
	GtkImage *imageKeypadBG;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	
	gtk_init (&argc, &argv);
	create_dummy_window();
	pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG, error);

	window = create_window1 ();

	KeyPadBuilder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (KeyPadBuilder, KEY_PAD_UI_FILE, error))
	{
		g_critical ("Couldn't load builder file: %s", (*error)->message);
		g_error_free (*error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (MainBuilder, NULL);

	/* Get the window object from the ui file */
	KeyPadFrameWidget = GTK_WIDGET (gtk_builder_get_object (KeyPadBuilder, KEY_PAD_FRAME));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				KEY_PAD_FRAME,
				KEY_PAD_UI_FILE);
        }

	//gtk_widget_show (KeyPadFrameWidget);
	GdkPixbufAnimation * pixbufKeypadBGanm = gdk_pixbuf_animation_new_from_file(KEYPAD_BG, error);
	imageKeypadBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (KeyPadBuilder, "keypad_bg"));
	gtk_image_set_from_animation(GTK_IMAGE(imageKeypadBG), pixbufKeypadBGanm);
	
	gtk_widget_show (window);
	cur_active_screen = wev1_scrn;
	_start_ticker_timer();
	gtk_main();
	g_free (priv);
	return 0;
}
#endif

#if 0
int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  if( argc!=3 ){
    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    exit(1);
  }
  if (sqlite3_open(argv[1], &db) != SQLITE_OK)
    {
    fprintf(stderr, "Can't open database: \n");
    sqlite3_close(db);
    exit(1);
  }
  rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}
#endif
#if 0
static gboolean mdk_text_scroller_expose(GtkWidget *textScroller, GdkEventExpose *event)
{

   MdkTextScroller *ts = MDK_TEXT_SCROLLER(textScroller);


   cairo_t *cr;

   cr = gdk_cairo_create(textScroller->window);
                                                                                                                               

    gdk_cairo_region(cr,
event->region);                                                                                                                              

cairo_clip(cr);                                                                                                                                                  


   cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
CAIRO_FONT_WEIGHT_NORMAL);

   cairo_set_font_size(cr, ts->size);
   cairo_move_to(cr, ts->x+ts->xOffset, ts->y+ts->yOffset);
   cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
   cairo_show_text(cr, ts->text);

   if (ts->width == 0 || ts->height == 0)
   {
      cairo_text_extents_t ext;
      cairo_text_extents(cr, ts->text, &ext);
      ts->width = ext.width;
      ts->height = ext.height;
   }

   if (ts->width+ts->xOffset+100 < 0)
   {
      cairo_move_to(cr, ts->x+ts->xOffset+ts->width+100, ts->y
+ts->yOffset);
      cairo_show_text(cr, ts->text);
   }

   if (ts->x+ts->xOffset+ts->width < 0)
   {
      ts->xOffset=ts->xOffset+ts->width+100;
   }


   cairo_stroke(cr);

   cairo_destroy(cr);
} 
#endif


void create_dummy_window(){
	GtkWidget *window;
	
		   GError *error=NULL;
		DummyBuilder = gtk_builder_new ();
		if (!gtk_builder_add_from_file (DummyBuilder, BACK_GND_FILE, &error))
		{
		  g_critical ("Couldn't load builder file: %s", error->message);
		  g_error_free (error);
		}
		//create_window2();

	window = GTK_WIDGET (gtk_builder_get_object (DummyBuilder, BACK_GND_WINDOW));

	//GdkPixbufAnimation * pixbufBGanm = gdk_pixbuf_animation_new_from_file(BG1, error);
	GtkImage *imageBG = (GtkImage *) GTK_WIDGET (gtk_builder_get_object (DummyBuilder, "image1"));
	gtk_image_set_from_animation(GTK_IMAGE(imageBG), pixbufBGanm);
	gtk_widget_show(window);
}
