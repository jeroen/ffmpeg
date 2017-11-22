#include <R_ext/Rdynload.h>
#include <libavcodec/avcodec.h>

void R_init_ffmpeg(DllInfo *info) {
  av_register_all();
  avcodec_register_all();
}
