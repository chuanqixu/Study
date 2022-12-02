/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				Chuanqi Xu
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color *color;
	if ((color = (Color *) malloc(sizeof(Color))) == NULL){
		exit(-1);
	}

	if (((image->image)[row][col].B & 0b00000001) == 1){
		color->R = 255;
		color->G = 255;
		color->B = 255;
	} else {
		color->R = 0;
		color->G = 0;
		color->B = 0;
	}
	return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *ip;
	Color *color;
	if ((ip = (Image *) malloc(sizeof(Image))) == NULL){
		exit(-1);
	}

	ip->rows = image->rows;
	ip->cols = image->cols;

	if ((ip->image = (Color **) malloc((ip->rows) * sizeof(Color *))) == NULL){
		exit(-1);
	}
	for (int i = 0; i < ip->rows; i++){
		if (((ip->image)[i] = (Color *) malloc((ip->cols) * sizeof(Color))) == NULL){
			exit(-1);
		}
		for (int j = 0; j < ip->cols; j++){
			color = evaluateOnePixel(image, i, j);
			(ip->image)[i][j].R = color->R;
			(ip->image)[i][j].G = color->G;
			(ip->image)[i][j].B = color->B;
			free(color);
		}
	}
	return ip;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	Image *ip, *secret;
	ip = readData(argv[1]);
	secret = steganography(ip);
	writeData(secret);
	freeImage(ip);
	freeImage(secret);
	exit(0);
}
