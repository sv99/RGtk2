## Makefile to run latex, Sweave or Stangle

#all: preface basics programming gWidgets gWidgetsEx RGtk2 RGtk2Ex tcltk www
all: preface basics programming gWidgets RGtk2 Qt tcltk latex
	
preface:
	cd ch-Preface; $(MAKE)

basics:
	cd ch-GUIBasics; $(MAKE)

programming:
	cd ch-ProgrammingPractices; $(MAKE)

gWidgets:
	cd ch-gWidgets; $(MAKE)

# gWidgetsEx:
# 	cd ch-gWidgetsExamples; $(MAKE)

RGtk2:
	cd ch-RGtk2; $(MAKE)

Qt:
	cd ch-Qt; $(MAKE)

# RGtk2Ex:
# 	cd ch-RGtk2Example; $(MAKE)

tcltk:
	cd ch-tcltk; $(MAKE)

www:
	cd ch-WebGUIs; $(MAKE)



latex:  
	pdflatex --interaction scrollmode guis; 
	bibtex guis 
	bibtex guis.fb
	makeindex general
	makeindex concepts
	pdflatex guis