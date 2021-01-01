/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Chuanqi Xu
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	Image *ip;
	if ((ip = (Image *) malloc(sizeof(Image))) == NULL){
		exit(-1);
	}
	FILE *fp;
	char s[10];
	int i;

	if ((fp=fopen(filename, "r")) == NULL) {
		printf("Fail to open files!\n");
		exit(-1);
	}
	fscanf(fp, "%s", s);
	fscanf(fp, "%d %d", &(ip->cols), &(ip->rows));
	fscanf(fp, "%d", &i);

	if ((ip->image = (Color **) malloc(ip->rows * sizeof(Color *))) == NULL){
		exit(-1);
	}
	for (int i = 0; i < ip->rows; i++) {
		if (((ip->image)[i] = (Color *) malloc(ip->cols * sizeof(Color))) == NULL){
			exit(-1);
		}
		for (int j = 0; j < ip->cols; j++){
			fscanf(fp, "%hhu %hhu %hhu", &(ip->image)[i][j].R, &(ip->image)[i][j].G, &(ip->image)[i][j].B);
		}
	}

	fclose(fp);
	return ip;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	for (int i = 0; i < image->rows; i++){
		for (int j = 0; j < image->cols; j++){
			printf("%3d %3d %3d", (image->image)[i][j].R, (image->image)[i][j].G, (image->image)[i][j].B);
			if (j != image->cols - 1){
				printf("   ");
			} else {
				printf("\n");
			}
		}
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows; i++){
		free((image->image)[i]);
	}
	free(image->image);
	free(image);
}
