\alias{gtkDrawSlider}
\name{gtkDrawSlider}
\title{gtkDrawSlider}
\description{
Draws a slider in the given rectangle on \code{window} using the
given style and orientation.
\strong{WARNING: \code{gtk_draw_slider} is deprecated and should not be used in newly-written code.}
}
\usage{gtkDrawSlider(object, window, state.type, shadow.type, x, y, 
    width, height, orientation)}
\arguments{
\item{\verb{object}}{a \code{\link{GtkStyle}}}
\item{\verb{window}}{a \code{\link{GdkWindow}}}
\item{\verb{state.type}}{a state}
\item{\verb{shadow.type}}{a shadow}
\item{\verb{x}}{the x origin of the rectangle in which to draw a slider}
\item{\verb{y}}{the y origin of the rectangle in which to draw a slider}
\item{\verb{width}}{the width of the rectangle in which to draw a slider}
\item{\verb{height}}{the height of the rectangle in which to draw a slider}
\item{\verb{orientation}}{the orientation to be used}
}
\author{Derived by RGtkGen from GTK+ documentation}
\keyword{internal}
