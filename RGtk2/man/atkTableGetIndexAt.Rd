\alias{atkTableGetIndexAt}
\name{atkTableGetIndexAt}
\title{atkTableGetIndexAt}
\description{Gets a \code{integer} representing the index at the specified \code{row} and \code{column}.
The value -1 is returned if the object at row,column is not a child
of table or table does not implement this interface.}
\usage{atkTableGetIndexAt(object, row, column)}
\arguments{
\item{\code{object}}{[\code{\link{AtkTable}}]  a GObject instance that implements AtkTableIface}
\item{\code{row}}{[integer]  a \code{integer} representing a row in \code{table}}
\item{\code{column}}{[integer]  a \code{integer} representing a column in \code{table}}
}
\value{[integer]  a \code{integer} representing the index at specified position }
\author{Derived by RGtkGen from GTK+ documentation}
\keyword{internal}