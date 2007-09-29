\alias{pangoLanguageIncludesScript}
\name{pangoLanguageIncludesScript}
\title{pangoLanguageIncludesScript}
\description{Determines if \code{script} is one of the scripts used to
write \code{language}. The returned value is conservative;
if nothing is known about the language tag \code{language},
\code{TRUE} will be returned, since, as far as Pango knows,
\code{script} might be used to write \code{language}.}
\usage{pangoLanguageIncludesScript(object, script)}
\arguments{
\item{\code{object}}{[\code{\link{PangoLanguage}}]  a \code{\link{PangoLanguage}}}
\item{\code{script}}{[\code{\link{PangoScript}}]  a \code{\link{PangoScript}}}
}
\details{This routine is used in Pango's itemization process when
determining if a supplied language tag is relevant to
a particular section of text. It probably is not useful for
applications in most circumstances.  

  Since  1.4}
\value{[logical]  \code{TRUE} if \code{script} is one of the scripts used
to write \code{language}, or if nothing is known about \code{language}.}
\author{Derived by RGtkGen from GTK+ documentation}
\keyword{internal}