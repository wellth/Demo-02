## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = wev1.ui \
	weMandMsg.ui \
	weSignUp.ui \
	weInit.ui \
	weLogin.ui \
	wel22.ui \
	../uiScreens/home.ui \
	../uiScreens/Test1.ui \
	../uiScreens/Test2.ui \
	../uiScreens/Test3.ui \
	../uiScreens/Test4.ui \
	../uiScreens/prev_we_check_stats.ui \
	../uiScreens/check_symptoms.ui \
	../uiScreens/email_colleague.ui \
	../uiScreens/quick_we_check.ui \
	../uiScreens/first_time_profile.ui \
	../uiScreens/client_admin.ui \
	../uiScreens/dependent_or_guest.ui \
	../uiScreens/profile.ui \
	../uiScreens/system_admin.ui \
	../uiScreens/system_admin_diagnostic.ui \
	../uiScreens/system_admin_theme.ui \
	../uiScreens/key_pad.ui \
	../uiScreens/health_summary.ui \
	../uiScreens/user_summary.ui \
	../uiScreens/q_n_a.ui \
	../uiScreens/exit.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(WEV1_CFLAGS) \
	$(sqlite3_CFLAGS) \
	$(SQLITE3_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = wev1

wev1_SOURCES = \
	main.c \
	windows.c \
	admin.c \
	admin.h \
	window_defs_n_config.h \
	profile.c \
	ticker.c \
	blood_pressure.c \
	body_fat.c \
	smoking_detector.c \
	weight_n_bmi.c \
	main_test_backend.h \
	keypad.c \
	Timer.c \
	demon_file_ops.c \
	DemonRetuns.h \
	summary_screen.c \
	ProcessCSV.c \
	ProcessCSV.h \
	choose_graph_image.c \
	qn_and_ans.c \
	exit.c

wev1_LDFLAGS = \
	-Wl,--export-dynamic

wev1_LDADD = $(WEV1_LIBS) \
	$(sqlite3_LIBS) \
	$(SQLITE3_LIBS)


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

