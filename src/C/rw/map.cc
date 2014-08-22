#include "map.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "stdio.h"

/*
 * Constructors
 * 	1] Initialize Map with sizeX and size Y
 * 	2] Initialzie Map with dimensions, nodes
 */

int initMap (Mat* matrix, int sizeX, int sizeY){

	// Inilialize matrix
	*matrix = Mat(sizeX, sizeY, CV_8UC3);

	return 0;

}

/*
 * Call mapInitAll() along with loadMap()
 */

//int mapInitAll (struct map *,int, int, struct node *, int);


void overlayNodes(Mat *matrix, struct node *cluster, int st_loca_size){

	for( int i=0; i< st_loca_size; i++ )
		circle( *matrix, Point(cluster[i].x,cluster[i].y), 2 , Scalar(0,0,255), -1, 8, 0 );

}



/*
 * Load node locations into map
 */
int loadMap(Mat* matrix,struct node *n0, int nodeCount){

	int x,y;
	//char str[10];
	/*
	 * Iterate through the node locations till nodeCount
	 * 	Draw circles at those locations in the map
	 */
	for(int i=0; i<nodeCount; i++){

		x = (int) *(&n0[i].x);
		y = (int) *(&n0[i].y);


		if( n0[i].pl == 0)
		circle( *matrix, Point(x,y), 40, Scalar(30,20,30), -1, 8, 0 );
		else
		circle( *matrix, Point(x,y), n0[i].pl, Scalar(255,255,255), -1, 8, 0 );

		if( n0[i].type == 1)
		circle( *matrix, Point(x,y), 5, Scalar(0,255,0), -1, 8, 0 );
		else
		circle( *matrix, Point(x,y), 2, Scalar(0,0,255), -1, 8, 0 );

		//sprintf(str,"%d: (%d,%d)",*(&n0[i].node_id),x,y);
		//putText(*matrix, str,Point(30,30),FONT_HERSHEY_PLAIN,1, Scalar(0,0,255),1,8,false);

	}// END OF FOR

	
	return 0;

}


// Calc Coverage
double getCoverage(Mat* matrix ){


	/* Calculate Coverage ( ratio of white to black )
	 *
	 * 	1] convert *matrix to GrayScale
	 * 	2] Binary threshold GrayScale image
	 * 	3] call countNonZero() method
	 */
	Mat dst;
	cvtColor(*matrix,dst,CV_BGR2GRAY,0);
	threshold(dst,dst,70,255,THRESH_BINARY);

	imshow("cov",dst);

	double coverage = (double)( countNonZero(dst)/(gMax_X*gMax_Y) );

	// deallocate memory
	dst.release();

	return coverage;

}

/* Activate Cluster */
void activateCluster(struct node* n0,struct node* cluster, int st_loca_size){

	for( int i=0; i< st_loca_size; i++){
		if( cluster[i].pl == 0 )
			if( calcDist( cluster[i].x, cluster[i].y, n0->x,n0->y ) < 55){

				cluster[i].pl = 40;
				gDiscNodes++;

				for( int j=0; j< st_loca_size; j++){

					if( cluster[j].pl == 0 )
					if( calcDist( cluster[i].x, cluster[i].y, cluster[j].x, cluster[j].y) < 70){
						cluster[j].pl = 40;
				gDiscNodes++;
					}

				}
					
			}

	}

}



/*
 * Update the pixel values based on node locations
 */
void updateMap (struct map *);

/*
 * Display the map as an image
 */
void drawMap (struct map *);

/*
 * Clear the map
 */
void cleanMap (struct map *);

/*
 * Destructor
 *
 * 	Destroy the map ( deallocate memory )
 */
void destroyMap (struct map *);
