#include "ffmpeg_types.h"

// [[Rcpp::export]]
XPtrEncoder new_encoder_internal(Rcpp::String path, Rcpp::String codec, size_t width, size_t height, size_t fps){
  Encoder * enc = new Encoder(path.get_cstring(), codec.get_cstring(), width, height, fps);
  XPtrEncoder ptr(enc);
  ptr.attr("class") = Rcpp::CharacterVector::create("ffmpeg-encoder");
  ptr.attr("path") = Rcpp::CharacterVector::create(path);
  ptr.attr("codec") = Rcpp::CharacterVector::create(codec);
  return ptr;
}

// [[Rcpp::export]]
XPtrEncoder add_frame_internal(XPtrEncoder encoder, Rcpp::RawVector image){

  return encoder;
}

