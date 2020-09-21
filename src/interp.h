#ifndef _INTERP1_H_
#define _INTERP1_H_

double interp1(int xlen, double x[], double y[], double xq) {
	//Note: X must be strictly monotonically increasing.

	//Check that we are in range
	if ( xq < x[0] || xq > x[xlen-1] ) {
		return 1.0/0.0;
	}

	//Now find what segment we are in
	int segment = 0;
	while ( xq > x[segment+1] ) {
        segment++; 
    }

	//Compute the start and endpoints of the current segment.
	double x0 = x[segment];
	double x1 = x[segment+1];
	double y0 = y[segment];
	double y1 = y[segment+1];

	//Compute the relative position within the segment in x
	double xt = (xq - x0)/(x1-x0);

	//Compute the Y postion to return
	double yRet = y0 + (y1-y0)*xt;

	return yRet;
}

#endif
