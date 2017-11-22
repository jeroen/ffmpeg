#' Encode Video
#'
#' @importFrom Rcpp sourceCpp
#' @useDynLib ffmpeg
#' @export
new_encoder <- function(file = "output.mp4", codec = "libx264"){
  stopifnot(is.character(codec))
  file <- normalizePath(file, mustWork = FALSE)
  new_encoder_internal(file, codec, width = 600, height = 400, fps = 25)
}
