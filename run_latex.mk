SRC=$(wildcard *.tex)
OBJECTS=$(SRC:.tex=-pdf.pdf)

all: $(OBJECTS)

%-pdf.pdf : %.tex
	pdflatex Hmatrix.tex
	rm Hmatrix.pdf
	pdflatex --jobname $(basename $@) Hmatrix
	rm *.log *.aux *.out

clean:
	rm -f *.pdf *.log *.aux *.tex *.out *~ *.gz

tar:
	tar -zcvf HFigs.tar.gz main.cpp H1D.h H2D.h HSS.h readme.txt run_latex.mk drawHmatrix Default_Images/
