###################################################
### chunk number 1: 
###################################################
w <- gwindow("Example of selection widgets", visible=FALSE)
g <- ggroup(horizontal=FALSE, cont=w)
cb <- gcheckbox("Allow selection", cont=g, checked=FALSE, 
                handler = function(h, ...) {
                  enabled(f) <- svalue(cb)
                })

f <- gframe("set off variables", cont=g)
tbl <- glayout(cont=f)
right <- c(1, 1); left <- c(-1, 1)
tbl[1,1, anchor=right] <- "Radio label"
tbl[1,2, anchor=left] <- (rb <- gradio(1:3, horizontal=TRUE, 
           cont = tbl))
tbl[2,1, anchor=right] <- "checkbox group label"
tbl[2,2, achor=left] <- (chb <- gcheckboxgroup(letters[1:3], 
           horizontal=FALSE, cont = tbl))
tbl[3,1, anchor=right] <- "combobox label"
tbl[3,2, achor=left] <- (combo <- gcombobox(state.name, 
           cont = tbl))
enabled(f) <- FALSE                     
visible(w) <- TRUE


