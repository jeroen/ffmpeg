extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#define R_NO_REMAP
#define STRICT_R_HEADERS

#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::DataFrame list_codecs(){
  Rcpp::CharacterVector name;
  Rcpp::CharacterVector longname;
  Rcpp::CharacterVector type;
  AVCodec * codec = av_codec_next(NULL);
  while(codec != NULL){
    name.push_back(codec->name);
    longname.push_back(codec->long_name);
    type.push_back(av_get_media_type_string(codec->type));
    codec = av_codec_next(codec);
  }
  return Rcpp::DataFrame::create(
    Rcpp::_["type"] = type,
    Rcpp::_["name"] = name,
    Rcpp::_["longname"] = longname,
    Rcpp:: _["stringsAsFactors"] = false
  );
}

// [[Rcpp::export]]
Rcpp::DataFrame list_formats(){
  Rcpp::CharacterVector name;
  Rcpp::CharacterVector longname;
  Rcpp::CharacterVector video;
  Rcpp::CharacterVector audio;
  av_register_all();
  AVOutputFormat * oformat = av_oformat_next(NULL);
  while(oformat != NULL){
    name.push_back(oformat->name);
    longname.push_back(oformat->long_name);
    video.push_back(avcodec_get_name(oformat->video_codec));
    audio.push_back(avcodec_get_name(oformat->audio_codec));
    oformat = av_oformat_next(oformat);
  }
  return Rcpp::DataFrame::create(
    Rcpp::_["name"] = name,
    Rcpp::_["longname"] = longname,
    Rcpp::_["video"] = video,
    Rcpp::_["audio"] = audio,
    Rcpp:: _["stringsAsFactors"] = false
  );
}

