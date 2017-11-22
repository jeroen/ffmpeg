#include "encoder.h"

Encoder::Encoder(std::string filename, std::string codec, size_t width, size_t height, size_t fps):
  codec_(initCodec(codec.c_str())),
  ctx_(initContext(width, height, fps)),
  frame_(initFrame()),
  pkt_(initPacket()){
    file_.open(filename, std::ofstream::out | std::ofstream::binary);
    if(file_.fail())
      throw std::runtime_error("Failed to open file: " + filename);
}

Encoder::~Encoder() {
  test_data(250);
  finish();
  avcodec_free_context(&ctx_);
  av_frame_free(&frame_);
  av_packet_free(&pkt_);
}

void Encoder::test_data(int n){


  int x, y;
  /* prepare a dummy image */
  /* Y */
  for (int i = 0; i < n; i++) {
    bail_if(av_frame_make_writable(frame_), "make frame writable");
    for (y = 0; y < ctx_->height; y++) {
      for (x = 0; x < ctx_->width; x++) {
        frame_->data[0][y * frame_->linesize[0] + x] = x + y + i * 3;
      }
    }

    /* Cb and Cr */
    for (y = 0; y < ctx_->height/2; y++) {
      for (x = 0; x < ctx_->width/2; x++) {
        frame_->data[1][y * frame_->linesize[1] + x] = 128 + y + i * 2;
        frame_->data[2][y * frame_->linesize[2] + x] = 64 + x + i * 5;
      }
    }
    frame_->pts = i;
    write_frame(frame_);
  }
}

void Encoder::write_frame(AVFrame * frame){
  int ret = avcodec_send_frame(ctx_, frame);
  bail_if(ret, "sending a frame for encoding");
  while (ret >= 0) {
    ret = avcodec_receive_packet(ctx_, pkt_);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
      return;
    bail_if(ret, "video encoding");
    file_.write((const char *) pkt_->data, pkt_->size);
    av_packet_unref(pkt_);
  }
}

AVCodec * Encoder::initCodec(std::string name){
  AVCodec * out = avcodec_find_encoder_by_name(name.c_str());
  if (!out)
    throw std::runtime_error("Failed to find codec: " + name);
  return out;
}

AVCodecContext * Encoder::initContext(size_t width, size_t height, int fps){
  AVCodecContext * c = avcodec_alloc_context3(codec_);
  if (!c)
    throw std::runtime_error("Failed to allocate AVCodecContext");

  /* use standard bitrate for now */
  c->bit_rate = 400000;

  /* resolution must be a multiple of two */
  c->width = width;
  c->height = height;

  /* frames per second */
  c->time_base = (AVRational){1, fps};
  c->framerate = (AVRational){fps, 1};

  /* emit one intra frame every ten frames */
  c->gop_size = 10;
  c->max_b_frames = 1;
  c->pix_fmt = AV_PIX_FMT_YUV420P;
  if (codec_->id == AV_CODEC_ID_H264)
    av_opt_set(c->priv_data, "preset", "slow", 0);

  /* open it */
  bail_if(avcodec_open2(c, codec_, NULL), "open codec");
  return c;
}

AVFrame * Encoder::initFrame(){
  AVFrame * frame = av_frame_alloc();
  if (!frame)
    throw std::runtime_error("Failed to allocate AVFrame");
  frame->format = ctx_->pix_fmt;
  frame->width  = ctx_->width;
  frame->height = ctx_->height;
  bail_if(av_frame_get_buffer(frame, 32), "allocate video frame data");
  return frame;
}

AVPacket * Encoder::initPacket(){
  AVPacket * out = av_packet_alloc();
  if (!out)
    throw std::runtime_error("Failed to allocate AVFrame");
  return out;
}

void Encoder::bail_if(int res, std::string str){
  if (res < 0)
    throw std::runtime_error("FFMPEG error (" + str + "):" + av_err2str(res));
}

void Encoder::finish(){
  static uint8_t endcode[4] = { 0, 0, 1, 0xb7 };
  if(file_.is_open()){
    write_frame(NULL);
    file_.write((char*) endcode, 4);
    file_.close();
  }
}
