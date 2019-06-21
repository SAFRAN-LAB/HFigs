#
# File: Readme.txt
# Description: The following code helps in generating figures of 
# hierarchical matrix
# ------------------------------------------------------------------
Below are the instructions to generate figures of hierarchical matrix.

1) Untar the file and go to the respective directory.
2) Run the following commands on the terminal to generate the figure of the hierarchical matrix.

	$./drawHmatrix

	(i) You will be asked to enter an option either 0 (or) 1 (or) 2 (or) 3. Enter one of the three options.
	(ii)You will be asked to enter the number of levels in the tree. Typical values range from '0' to '4'.

3) Now there should be a file named Hmatrix-pdf.pdf in your working directory.
4) To change colors, you need change them inside the code.
5) To tar the file, run the following command in the terminal.

	$make -f run_latex.mk tar
