/*
 * Notifier
 *
 * Copyright(C) 2015, Christopher E. Winton <GitHubDevSpam@winton.cc>,
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or(at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 *
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* config.h may define PURPLE_PLUGINS; protect the definition here so that we
 * don't get complaints about redefinition when it's not necessary. */
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <glib.h>

/* This will prevent compiler errors in some instances and is better explained in the
 * how-to documents on the wiki */
#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

#include <notify.h>
#include <plugin.h>
#include <version.h>

#include "conversation.h"
#include "debug.h"
#include "signals.h"
#include "sound.h"


#include "trex.h"



//Prototypes because I'm lazy
int
trex_test(
    TRex              * regex,
    const TRexChar    * text
);


PurplePlugin          * notifier_plugin     = NULL;

TRex                  * notice_regex        = NULL;
TRex                  * raw_notice_regex    = NULL;


PurpleConversation    * Notice_Conv         = NULL;

int                     Legit_Notice        = 0;


int
trex_test(
    TRex              * regex,
    const TRexChar    * text
) {

	TRexBool result = trex_match(regex, text);

    return result;

}


static void
irctxt_received_cb(
    PurpleConnection      * gc, 
    char                 ** msg
)  {

    if( trex_test(raw_notice_regex, *msg) )
    {
        Legit_Notice = 1;
    }

    //if(Legit_Notice)
    //{
    //    purple_debug_misc(
    //        "notifier",
    //        "RAW MESSAGE('%s')\n",
    //        *msg
    //    );
    //}

}


static gboolean
receiving_chat_msg_cb(
    PurpleAccount         * account,
    char                 ** sender,
    char                 ** buffer,
    PurpleConversation    * chat,
    PurpleMessageFlags    * flags, 
    void                  * data
) {
    char              * new_message = NULL;
    PurpleConvIm      * im_convo    = NULL;


	//purple_debug_misc(
    //    "notifier",
	//	"receiving-chat-msg (%s, %s, %s, %s, %d)\n",
	//	purple_account_get_username(account),
    //    *sender,
    //    *buffer,
	//	purple_conversation_get_name(chat),
    //    *flags
    //);


    if( trex_test(notice_regex, *buffer) && Legit_Notice ) {
        Legit_Notice = 0;

        purple_sound_play_event(PURPLE_SOUND_POUNCE_DEFAULT, NULL);

        new_message = g_strdup_printf("<font color=\"#CC7000\"><b>%s</b></font>", *buffer);

        g_free(*buffer);
        *buffer = new_message;

        //Start a new converstation
        if(!Notice_Conv)
        {
            Notice_Conv = purple_conversation_new(PURPLE_CONV_TYPE_IM, account, "Notice Log");
        }

        //Get a handle to the im window
        im_convo = purple_conversation_get_im_data(Notice_Conv);

        //If we don't get a converstation window back, then convo window was closed so we'll need to start a new convo and try to get another handle to the window.
        if(!im_convo)
        {
            Notice_Conv = purple_conversation_new(PURPLE_CONV_TYPE_IM, account, "Notice Log");
            im_convo = purple_conversation_get_im_data(Notice_Conv);
        }


        //Create another message just for the "Notice Log"
        new_message = g_strdup_printf("%s - %s - %s", purple_conversation_get_name(chat), *buffer, *sender);
        purple_conv_im_write(
            im_convo,
            "AUTO: ",
            new_message,
            PURPLE_MESSAGE_RECV,
            time(NULL)
        );


        //purple_debug_misc(
        //    "notifier",
        //    "Got a match(%s, %s, %s, %s, %d)\n",
        //    purple_account_get_username(account),
        //    *sender,
        //    *buffer,
        //    purple_conversation_get_name(chat),
        //    *flags
        //);

    }


	return FALSE;

}




static gboolean
plugin_load(PurplePlugin * plugin)
{
    void              * conv_handle = purple_conversations_get_handle();
    PurplePlugin      * irc         = purple_find_prpl("prpl-irc");
	const TRexChar    * error       = NULL;


    purple_signal_connect(
        conv_handle,
        "receiving-chat-msg",
        plugin,
        PURPLE_CALLBACK(receiving_chat_msg_cb),
        NULL
    );

    purple_signal_connect(
        irc,
        "irc-receiving-text",
        plugin,
        PURPLE_CALLBACK(irctxt_received_cb),
        NULL
    );


    notice_regex = trex_compile(_TREXC("\\(notice\\)"), &error);

    raw_notice_regex = trex_compile(_TREXC("^:\\S+ NOTICE \\S+ :"), &error);


    notifier_plugin = plugin; 

    return TRUE;

}


static gboolean
plugin_unload(
    PurplePlugin      * plugin
) {
    void  * conv_handle = purple_conversations_get_handle();

    purple_signal_disconnect(
        conv_handle,
        "receiving-chat-msg",
        plugin,
        PURPLE_CALLBACK(receiving_chat_msg_cb)
    );

    purple_debug_misc("notifications", "Yup, still got it Received unload\n");


    trex_free(notice_regex);
    trex_free(raw_notice_regex);


    return TRUE;

}


static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,

    "core-notifier",
    "Notifier",
    DISPLAY_VERSION, //This constant is defined in config.h, but you shouldn't use it for your own plugins.  We use it here because it's our plugin. And we're lazy.
    "Notifies the user of server notifications",
    "Notifies the user of notices and when people mention their name.",
    "Christopher E. Winton <GitHubDevSpam@winton.cc>",
    "http://www.youtube.com/watch?v=dQw4w9WgXcQ",


    plugin_load,
    plugin_unload,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

static void
init_plugin(
    PurplePlugin    * plugin
) {
}

PURPLE_INIT_PLUGIN(notifier, init_plugin, info)

