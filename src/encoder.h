#ifndef __FFMPEG_ENCODER__
#define __FFMPEG_ENCODER__

#include <fstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
}

class Encoder {
  AVCodec *codec_;
  AVCodecContext *ctx_;
  AVFrame *frame_;
  AVPacket *pkt_;
  std::ofstream file_;

  public:
    Encoder(std::string filename, std::string codec, size_t width, size_t height, size_t fps);
    ~Encoder();
    void test_data(int secs);
    void write_frame(AVFrame * frame);
    void finish();

  private:
    AVCodec * initCodec(std::string codec);
    AVCodecContext * initContext(size_t width, size_t height, int fps);
    AVFrame * initFrame();
    AVPacket * initPacket();
    void bail_if(int ret, std::string str);
};

#endif
