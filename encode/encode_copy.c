/*
 * encode_copy.c -- passthrough A/V frames through deep copy.
 * (C) 2005-2010 Francesco Romani <fromani at gmail dot com>
 *
 * This file is part of transcode, a video stream processing tool.
 *
 * transcode is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * transcode is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "src/transcode.h"
#include "src/framebuffer.h"

#include "libtcutil/optstr.h"

#include "libtcmodule/tcmodule-plugin.h"

#define MOD_NAME    "encode_copy.so"
#define MOD_VERSION "v0.0.5 (2009-02-03)"
#define MOD_CAP     "copy (passthrough) A/V frames"

#define MOD_FEATURES \
    TC_MODULE_FEATURE_ENCODE|TC_MODULE_FEATURE_VIDEO|TC_MODULE_FEATURE_AUDIO

#define MOD_FLAGS \
    TC_MODULE_FLAG_RECONFIGURABLE

static const char copy_help[] = ""
    "Overview:\n"
    "    this module passthrough A/V frames copying them from input\n"
    "    to output.\n"
    "    For a faster passthrough consider usage of 'null' module.\n"
    "Options:\n"
    "    help    produce module overview and options explanations\n";


static int copy_init(TCModuleInstance *self, uint32_t features)
{
    TC_MODULE_INIT_CHECK(self, MOD_FEATURES, features);

    if (self == NULL) {
        tc_log_error(MOD_NAME, "init: bad instance data reference");
        return TC_ERROR;
    }

    if (verbose) {
        tc_log_info(MOD_NAME, "%s %s", MOD_VERSION, MOD_CAP);
    }
    self->userdata = NULL;

    return TC_OK;
}

static int copy_fini(TCModuleInstance *self)
{
    TC_MODULE_SELF_CHECK(self, "fini");

    return TC_OK;
}

static int copy_inspect(TCModuleInstance *self,
                        const char *param, const char **value)
{
    TC_MODULE_SELF_CHECK(self, "inspect");

    if (optstr_lookup(param, "help")) {
        *value = copy_help;
    }

    return TC_OK;
}

static int copy_configure(TCModuleInstance *self,
                          const char *options,
                          TCJob *vob,
                          TCModuleExtraData *xdata[])
{
    TC_MODULE_SELF_CHECK(self, "configure");

    return TC_OK;
}

static int copy_stop(TCModuleInstance *self)
{
    TC_MODULE_SELF_CHECK(self, "stop");

    return TC_OK;
}

static int copy_encode_video(TCModuleInstance *self,
                             TCFrameVideo *inframe, TCFrameVideo *outframe)
{
    TC_MODULE_SELF_CHECK(self, "encode_video");

    vframe_copy(outframe, inframe, 1);
    outframe->video_len = outframe->video_size;

    return TC_OK;
}

static int copy_encode_audio(TCModuleInstance *self,
                             TCFrameAudio *inframe, TCFrameAudio *outframe)
{
    TC_MODULE_SELF_CHECK(self, "encode_audio");

    aframe_copy(outframe, inframe, 1);
    outframe->audio_len = outframe->audio_size;

    return TC_OK;
}


/*************************************************************************/

static const uint32_t copy_codecs_video_in[] = { 
    TC_CODEC_ANY, TC_CODEC_ERROR 
};
static const uint32_t copy_codecs_audio_in[] = { 
    TC_CODEC_ANY, TC_CODEC_ERROR 
};
static const uint32_t copy_codecs_video_out[] = { 
    TC_CODEC_ANY, TC_CODEC_ERROR 
};
static const uint32_t copy_codecs_audio_out[] = { 
    TC_CODEC_ANY, TC_CODEC_ERROR 
};
TC_MODULE_CODEC_FORMATS(copy);

TC_MODULE_INFO(copy);

static const TCModuleClass copy_class = {
    TC_MODULE_CLASS_HEAD(copy),

    .init         = copy_init,
    .fini         = copy_fini,
    .configure    = copy_configure,
    .stop         = copy_stop,
    .inspect      = copy_inspect,

    .encode_video = copy_encode_video,
    .encode_audio = copy_encode_audio,
};

TC_MODULE_ENTRY_POINT(copy);

/*************************************************************************/

/*
 * Local variables:
 *   c-file-style: "stroustrup"
 *   c-file-offsets: ((case-label . *) (statement-case-intro . *))
 *   indent-tabs-mode: nil
 * End:
 *
 * vim: expandtab shiftwidth=4:
 */
