###################################################
### chunk number 1: gtkTableGetLabelsFromDataFrame
###################################################
#line 8 "ex-RGtk2-calculator-buttons.Rnw"
gtkTableGetLabelsFromDataFrame <-
  function(object, dataFrame, callBack, userData=NULL) {
    checkPtrType(object, "GtkTable")
    dataFrame <- as.data.frame(dataFrame)
    d <- dim(dataFrame)
    object$resize(d[1], d[2])            # resize dynamically
    for(i in 1:d[1]) {
      for(j in 1:d[2]) {
        child <-  gtkButton(as.character(dataFrame[i,j]))
        id <- connectSignal(child,"clicked", f=callBack,
                            data=list(i=i, j=j, data=userData),
                            user.data.first=TRUE)
        ## 0-based for attaching
        object$attach(child, 
                      left.attach=j-1, j, top.attach=i-1, i)
      }}}


###################################################
### chunk number 2: buttonLayout
###################################################
#line 29 "ex-RGtk2-calculator-buttons.Rnw"
m = rbind(
  c("^", "(", ")", " / ", " == "),
  c(7  ,   8,   9, " * ", " > "),
  c(4  ,   5,   6, " - ", " >= "),
  c(1  ,   2,   3, " + ", " < "),
  c(0  , ".", " ", " ! ", " <= ")
  )


###################################################
### chunk number 3: buttonGUI
###################################################
#line 40 "ex-RGtk2-calculator-buttons.Rnw"
tab <- gtkTable(homogeneous=TRUE)       # same size buttons
tab$getLabelsFromDataFrame(m, function(userData, b) 
                           cat(b$getLabel()))
w <- gtkWindow(show=FALSE)
w$setTitle("Show matrix")
w$add(tab)
w$show()


