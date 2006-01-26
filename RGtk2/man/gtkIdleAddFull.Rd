\alias{gtkIdleAddFull}
\name{gtkIdleAddFull}
\title{gtkIdleAddFull}
\description{
Like \code{\link{gtkIdleAdd}} this function allows you to have a function called
when the event loop is idle. The difference is that you can give a 
priority different from \code{GTK_PRIORITY_DEFAULT} to the idle function.
\strong{WARNING: \code{gtk_idle_add_full} is deprecated and should not be used in newly-written code. Use \code{gIdleAddFull()} instead.}
}
\usage{gtkIdleAddFull(priority, fun, data = NULL)}
\arguments{
\item{\code{priority}}{[integer] The priority which should not be above \code{G_PRIORITY_HIGH_IDLE}.
Note that you will interfere with GTK+ if you use a priority above
\code{GTK_PRIORITY_RESIZE}.}
\item{\code{data}}{[R object] Data to pass to that function.}
}
\value{[numeric] A unique id for the event source.}
\note{
This function is deprecated. You should use \code{GDK_PRIORITY_REDRAW} instead.
This function is deprecated. You should use \code{G_PRIORITY_HIGH} instead.
This function is deprecated. You should use \code{G_PRIORITY_DEFAULT_IDLE} instead.
This function is deprecated. You should use \code{G_PRIORITY_LOW} instead.
}
\author{Derived by RGtkGen from GTK+ documentation}
\keyword{internal}