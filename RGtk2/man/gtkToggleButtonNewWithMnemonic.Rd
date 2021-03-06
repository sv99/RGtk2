\alias{gtkToggleButtonNewWithMnemonic}
\name{gtkToggleButtonNewWithMnemonic}
\title{gtkToggleButtonNewWithMnemonic}
\description{Creates a new \code{\link{GtkToggleButton}} containing a label. The label
will be created using \code{\link{gtkLabelNewWithMnemonic}}, so underscores
in \code{label} indicate the mnemonic for the button.}
\usage{gtkToggleButtonNewWithMnemonic(label, show = TRUE)}
\arguments{\item{\verb{label}}{the text of the button, with an underscore in front of the
mnemonic character}}
\value{[\code{\link{GtkWidget}}] a new \code{\link{GtkToggleButton}}}
\author{Derived by RGtkGen from GTK+ documentation}
\keyword{internal}
