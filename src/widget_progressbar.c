/*
 * widget_progressbar.c: 
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2003-2007  László Pere <pipas@linux.pte.hu>
 * Copyright (C) 2011-2012  Thunor <thunorsif@hotmail.com>
 * Copyright (C) 2026       Artur Bednarek <artur@unix.org.pl>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* Includes */
#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <errno.h>
#ifndef G_OS_WIN32
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "signals.h"
#include "tag_attributes.h"

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS
#define PROGRESSBAR_TERMINATE_GRACE_MILLISECONDS 500
#define PROGRESSBAR_EXIT_GRACE_MICROSECONDS 100000

/* Local variables */
struct _progr_descr {
	GtkWidget    *widget;
	gdouble       fraction;
	gchar        *shell_command;
	GPid          child_pid;
	GIOChannel   *channel;
	guint         watch_id;
	guint         child_watch_id;
	guint         force_kill_id;
	gulong        destroy_handler_id;
	gboolean      actions_performed;
	gboolean      io_finished;
	gboolean      child_finished;
	gboolean      terminating;
	AttributeSet *Attr;
};
typedef struct _progr_descr progr_descr;

extern gchar *option_include_file;

#ifndef G_OS_WIN32
static GSList *progressbar_child_pids;
static gboolean progressbar_exit_handler_registered;
#endif

/* Local function prototypes, located at file bottom */
static void widget_progressbar_input_by_command(variable *var, char *command);
static void widget_progressbar_input_by_file(variable *var, char *filename);
static void widget_progressbar_input_by_items(variable *var);
static void widget_progressbar_perform_actions(progr_descr *descr);
static void widget_progressbar_process_line(progr_descr *descr, gchar *line);
static gboolean widget_progressbar_io_callback(GIOChannel *channel,
	GIOCondition condition, gpointer data);
static void widget_progressbar_io_destroy_notify(gpointer data);
static void widget_progressbar_child_exited(GPid pid, gint status,
	gpointer data);
static void widget_progressbar_widget_destroyed(GtkWidget *widget,
	gpointer data);
#ifndef G_OS_WIN32
static gint widget_progressbar_signal_child(GPid pid, gint signal_number);
static void widget_progressbar_begin_termination(progr_descr *descr);
#endif
void widget_progressbar_realized_callback(GtkWidget *widget, AttributeSet *Attr);

/* Notes:
 * This widget is interesting! The "realized" signal is being used to
 * execute the contents of the input directive and if that terminates
 * then it's game over for this widget because you can't refresh it */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_progressbar_clear(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Clear not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/

GtkWidget *widget_progressbar_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList            *element;
	GtkWidget        *widget;
	gchar            *text;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	widget = gtk_progress_bar_new();

	/* Thunor: This is all original code moved across when refactoring */
	if (attributeset_is_avail(Attr, ATTR_LABEL)) {
		text = attributeset_get_first(&element, Attr, ATTR_LABEL);
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widget), text);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_progressbar_envvar_construct(GtkWidget *widget)
{
	gchar            *string;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	string = g_strdup("");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_progressbar_fileselect(
	variable *var, const char *name, const char *value)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Refresh                                                             *
 ***********************************************************************/

void widget_progressbar_refresh(variable *var)
{
	GList            *element;
	gchar            *act;
	gint              initialised = FALSE;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Get initialised state of widget */
	if (g_object_get_data(G_OBJECT(var->Widget), "_initialised") != NULL)
		initialised = GPOINTER_TO_INT(g_object_get_data(
			G_OBJECT(var->Widget), "_initialised"));

	/* The <input> tag... */
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act) {
		if (input_is_shell_command(act))
			widget_progressbar_input_by_command(var, act + 8);
		/* input file stock = "File:", input file = "File:/path/to/file" */
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			widget_progressbar_input_by_file(var, act + 5);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_progressbar_input_by_items(var);

	/* Initialise these only once at start-up */
	if (!initialised) {
		/* Apply directives */
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
			fprintf(stderr, "%s(): <default> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT))
			fprintf(stderr, "%s(): <height> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_WIDTH))
			fprintf(stderr, "%s(): <width> not implemented for this widget.\n",
				__func__);
		if ((attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled")) ||	/* Deprecated */
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
			gtk_widget_set_sensitive(var->Widget, FALSE);

		/* Connect signals */
		/* Thunor: This is all original code moved across when refactoring */
		/* We start the input command in a separate thread when the
		 * widget gets realized */
		g_signal_connect(G_OBJECT(var->Widget), "realize",
			G_CALLBACK(widget_progressbar_realized_callback),
			(gpointer)var->Attributes);

	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_progressbar_removeselected(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Save                                                                *
 ***********************************************************************/

void widget_progressbar_save(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Save not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Command                                                    *
 ***********************************************************************/

static void widget_progressbar_input_by_command(variable *var, char *command)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* <input> is handled elsewhere for this widget */

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by File                                                       *
 ***********************************************************************/

static void widget_progressbar_input_by_file(variable *var, char *filename)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): <input file> not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Items                                                      *
 ***********************************************************************/

static void widget_progressbar_input_by_items(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): <item> not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Some stuff to handle progress bars                                  *
 ***********************************************************************/
/* Thunor: This simply executed all actions when the progressbar reached
 * 100 so I've fixed that by routing it through gtkdialog's normal signal
 * handling system and the default signal is now "time-out" */

static void widget_progressbar_perform_actions(progr_descr *descr)
{

	widget_signal_executor(descr->widget, descr->Attr, "time-out");
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
static void widget_progressbar_process_line(progr_descr *descr, gchar *line)
{
	gchar *end;
	gsize length;
	gint64 value;
	gboolean numeric;

	length = strlen(line);
	while (length > 0 &&
		(line[length - 1] == '\n' || line[length - 1] == '\r'))
		line[--length] = '\0';

	errno = 0;
	value = g_ascii_strtoll(line, &end, 0);
	numeric = end != line;
	while (g_ascii_isspace(*end))
		++end;
	numeric = numeric && errno == 0 && *end == '\0';

	if (descr->widget != NULL) {
		if (numeric) {
			descr->fraction = CLAMP(value / 100.0, 0.0, 1.0);
			gtk_progress_bar_set_fraction(
				GTK_PROGRESS_BAR(descr->widget), descr->fraction);
		} else {
			gtk_progress_bar_set_text(
				GTK_PROGRESS_BAR(descr->widget), line);
		}
	}

	if (!numeric || value != 100)
		descr->actions_performed = FALSE;
	if (numeric && value == 100 && !descr->actions_performed &&
		descr->widget != NULL) {
		descr->actions_performed = TRUE;
		widget_progressbar_perform_actions(descr);
	}
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
static gboolean widget_progressbar_io_callback(GIOChannel *channel,
	GIOCondition condition, gpointer data)
{
	progr_descr *descr = data;
	GError *error = NULL;
	GIOStatus status;
	gchar *line;
	gint lines_processed = 0;

	do {
		line = NULL;
		status = g_io_channel_read_line(channel, &line, NULL, NULL, &error);
		if (status == G_IO_STATUS_NORMAL) {
			widget_progressbar_process_line(descr, line);
			g_free(line);
			lines_processed++;
		}
	} while (status == G_IO_STATUS_NORMAL && lines_processed < 128);

	if (error != NULL) {
		gtkdialog_warning("%s(): Couldn't read progress input: %s",
			__func__, error->message);
		g_error_free(error);
		return FALSE;
	}

	if (status == G_IO_STATUS_EOF ||
		(condition & (G_IO_ERR | G_IO_NVAL)) != 0)
		return FALSE;

	/* A hang-up can arrive together with the final readable data. Keep the
	 * source only while there may still be buffered complete lines. */
	if ((condition & G_IO_HUP) != 0 && status != G_IO_STATUS_NORMAL)
		return FALSE;

	return TRUE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
static void widget_progressbar_maybe_free(progr_descr *descr)
{
	if (!descr->io_finished || !descr->child_finished)
		return;

	if (descr->widget != NULL && descr->destroy_handler_id != 0)
		g_signal_handler_disconnect(descr->widget,
			descr->destroy_handler_id);
	g_free(descr->shell_command);
	g_free(descr);
}

static void widget_progressbar_io_destroy_notify(gpointer data)
{
	progr_descr *descr = data;

	descr->watch_id = 0;
	descr->io_finished = TRUE;
	if (descr->channel != NULL) {
		g_io_channel_unref(descr->channel);
		descr->channel = NULL;
	}
	widget_progressbar_maybe_free(descr);
}

static void widget_progressbar_child_exited(GPid pid, gint status,
	gpointer data)
{
	progr_descr *descr = data;

#ifndef G_OS_WIN32
	/* The direct shell can exit after SIGTERM while a descendant in its
	 * process group keeps running.  Once the shell has completed its cleanup,
	 * terminate any such descendants before forgetting the group id. */
	if (descr->terminating)
		widget_progressbar_signal_child(pid, SIGKILL);
	if (descr->force_kill_id != 0) {
		g_source_remove(descr->force_kill_id);
		descr->force_kill_id = 0;
	}
	progressbar_child_pids = g_slist_remove(progressbar_child_pids,
		GINT_TO_POINTER((gint)pid));
#endif
	descr->child_watch_id = 0;
	descr->child_pid = 0;
	descr->child_finished = TRUE;
	if (!descr->terminating)
		widget_report_shell_status(descr->shell_command, status);
	g_spawn_close_pid(pid);
	widget_progressbar_maybe_free(descr);
}

#ifndef G_OS_WIN32
static gint widget_progressbar_signal_child(GPid pid, gint signal_number)
{
	if (kill(-((pid_t)pid), signal_number) == 0)
		return 0;
	if (errno == ESRCH)
		return kill((pid_t)pid, signal_number);
	return -1;
}

static void widget_progressbar_terminate_children(void)
{
	GSList *element;
	int status;
	pid_t result;

	if (progressbar_child_pids == NULL)
		return;

	for (element = progressbar_child_pids; element != NULL;
		element = element->next)
		widget_progressbar_signal_child(
			(GPid)GPOINTER_TO_INT(element->data), SIGTERM);

	/* exit() cannot return to the GTK loop to run the normal grace timer.
	 * Give all groups one short shared grace period, force any survivors to
	 * stop, and collect each direct child before gtkdialog itself exits. */
	g_usleep(PROGRESSBAR_EXIT_GRACE_MICROSECONDS);
	for (element = progressbar_child_pids; element != NULL;
		element = element->next)
		widget_progressbar_signal_child(
			(GPid)GPOINTER_TO_INT(element->data), SIGKILL);
	for (element = progressbar_child_pids; element != NULL;
		element = element->next) {
		do {
			result = waitpid((pid_t)GPOINTER_TO_INT(element->data),
				&status, 0);
		} while (result == -1 && errno == EINTR);
	}
	g_slist_free(progressbar_child_pids);
	progressbar_child_pids = NULL;
}

static gboolean widget_progressbar_force_termination(gpointer data)
{
	progr_descr *descr = data;

	descr->force_kill_id = 0;
	if (descr->child_pid != 0 &&
		widget_progressbar_signal_child(descr->child_pid, SIGKILL) == -1 &&
		errno != ESRCH)
		gtkdialog_warning("Couldn't stop progress command '%s': %s",
			descr->shell_command, g_strerror(errno));
	return FALSE;
}

static void widget_progressbar_begin_termination(progr_descr *descr)
{
	descr->terminating = TRUE;
	if (descr->child_pid == 0)
		return;
	if (widget_progressbar_signal_child(descr->child_pid, SIGTERM) == -1 &&
		errno != ESRCH)
		gtkdialog_warning("Couldn't terminate progress command '%s': %s",
			descr->shell_command, g_strerror(errno));
	if (descr->force_kill_id == 0)
		descr->force_kill_id = g_timeout_add(
			PROGRESSBAR_TERMINATE_GRACE_MILLISECONDS,
			widget_progressbar_force_termination, descr);
}
#endif

static void widget_progressbar_widget_destroyed(GtkWidget *widget,
	gpointer data)
{
	progr_descr *descr = data;

	(void)widget;
	descr->widget = NULL;
	descr->destroy_handler_id = 0;
#ifndef G_OS_WIN32
	widget_progressbar_begin_termination(descr);
#else
	descr->terminating = TRUE;
#endif
	if (descr->watch_id != 0)
		g_source_remove(descr->watch_id);
}

#ifndef G_OS_WIN32
static void widget_progressbar_child_setup(gpointer data)
{
	(void)data;
	setpgid(0, 0);
}
#endif

static gchar *widget_progressbar_build_shell_command(const gchar *command)
{
	gchar *quoted_include;
	gchar *shell_command;

	if (option_include_file == NULL)
		return g_strdup(command);

	quoted_include = g_shell_quote(option_include_file);
	shell_command = g_strdup_printf(". %s; %s", quoted_include, command);
	g_free(quoted_include);
	return shell_command;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* Start the input command when the widget is realized and process its
 * output from the GTK main loop. */

void widget_progressbar_realized_callback(GtkWidget *widget, AttributeSet *Attr)
{
	GList *element;
	GError *error = NULL;
	GIOFlags flags;
	GIOStatus status;
	progr_descr *descr;
	gchar *argv[] = { (gchar *)"/bin/sh", (gchar *)"-c", NULL, NULL };
	gchar *full_command;
	gchar *input;
	gint output_fd;

#ifdef DEBUG
	g_message("%s(%p, %p);", __func__, widget, Attr);
#endif	
	g_assert(GTK_IS_WIDGET(widget) && Attr != NULL);
	/*
	 * If there is no input, we can return.
	 */
	if (!attributeset_is_avail(Attr, ATTR_INPUT))
		return;
	/*
	 * Creating the descriptor from the first input.
	 */
	input = attributeset_get_first(&element, Attr, ATTR_INPUT);
	if (!input_is_shell_command(input))
		return;

	descr = g_new0(progr_descr, 1);
	descr->Attr = Attr;
	descr->widget = widget;
	descr->shell_command = g_strdup(input_get_shell_command(input));
	full_command = widget_progressbar_build_shell_command(
		descr->shell_command);
	argv[2] = full_command;
	if (!g_spawn_async_with_pipes(NULL, argv, NULL,
		G_SPAWN_DO_NOT_REAP_CHILD,
#ifdef G_OS_WIN32
		NULL,
#else
		widget_progressbar_child_setup,
#endif
		NULL, &descr->child_pid, NULL, &output_fd, NULL, &error)) {
		gtkdialog_warning("Couldn't start progress command '%s': %s",
			descr->shell_command, error->message);
		g_error_free(error);
		g_free(full_command);
		g_free(descr->shell_command);
		g_free(descr);
		return;
	}
	g_free(full_command);
#ifndef G_OS_WIN32
	progressbar_child_pids = g_slist_prepend(progressbar_child_pids,
		GINT_TO_POINTER((gint)descr->child_pid));
	if (!progressbar_exit_handler_registered) {
		atexit(widget_progressbar_terminate_children);
		progressbar_exit_handler_registered = TRUE;
	}
#endif
	descr->child_watch_id = g_child_watch_add_full(G_PRIORITY_DEFAULT,
		descr->child_pid, widget_progressbar_child_exited, descr, NULL);

#ifdef G_OS_WIN32
	descr->channel = g_io_channel_win32_new_fd(output_fd);
#else
	descr->channel = g_io_channel_unix_new(output_fd);
#endif
	g_io_channel_set_close_on_unref(descr->channel, TRUE);
	status = g_io_channel_set_encoding(descr->channel, NULL, &error);
	if (status != G_IO_STATUS_NORMAL) {
		gtkdialog_warning("%s(): Couldn't configure progress input: %s",
			__func__, error->message);
		g_error_free(error);
#ifndef G_OS_WIN32
		widget_progressbar_begin_termination(descr);
#else
		descr->terminating = TRUE;
#endif
		widget_progressbar_io_destroy_notify(descr);
		return;
	}

	flags = g_io_channel_get_flags(descr->channel);
	status = g_io_channel_set_flags(descr->channel,
		flags | G_IO_FLAG_NONBLOCK, &error);
	if (status != G_IO_STATUS_NORMAL) {
		gtkdialog_warning("%s(): Couldn't make progress input non-blocking: %s",
			__func__, error->message);
		g_error_free(error);
#ifndef G_OS_WIN32
		widget_progressbar_begin_termination(descr);
#else
		descr->terminating = TRUE;
#endif
		widget_progressbar_io_destroy_notify(descr);
		return;
	}

	descr->destroy_handler_id = g_signal_connect(widget, "destroy",
		G_CALLBACK(widget_progressbar_widget_destroyed), descr);
	descr->watch_id = g_io_add_watch_full(descr->channel,
		G_PRIORITY_DEFAULT, G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL,
		widget_progressbar_io_callback, descr,
		widget_progressbar_io_destroy_notify);
}
