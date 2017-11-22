#' FFmpeg
#' 
#' List supported codecs and formats.
#' 
#' @rdname ffmpeg
#' @name ffmpeg
#' @export
av_formats <- function(){
  list_formats()
}

#' @rdname ffmpeg
#' @export
av_codecs <- function(){
  list_codecs()
}