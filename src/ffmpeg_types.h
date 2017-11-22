#ifndef __FFMPEG_TYPES__
#define __FFMPEG_TYPES__

#include "encoder.h"

#define R_NO_REMAP
#define STRICT_R_HEADERS

#include <Rcpp.h>

//trigger destructor
inline void finalizeEncoder(Encoder * enc) {
  delete enc;
}

typedef Rcpp::XPtr<Encoder, Rcpp::PreserveStorage, finalizeEncoder, true> XPtrEncoder;

#endif
