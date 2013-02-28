#include "SimplexNoise.h"

// Private static member definitions
const double SimplexNoise::F2 = 0.5f * (sqrt( 3.0f ) - 1.0f);
const double SimplexNoise::G2 = (3.0f - sqrt( 3.0f )) / 6.0f;

uint8_t SimplexNoise::p[256] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,
    142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,
    203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,
    74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,
    220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,
    132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,
    186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,
    59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,
    70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,
    178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,
    241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,
    176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,
    128,195,78,66,215,61,156,180
};
const Grad SimplexNoise::grad3[12] = {
    Grad(1,1,0),Grad(-1,1,0),Grad(1,-1,0),Grad(-1,-1,0),Grad(1,0,1),
    Grad(-1,0,1),Grad(1,0,-1),Grad(-1,0,-1),Grad(0,1,1),Grad(0,-1,1),
    Grad(0,1,-1),Grad(0,-1,-1)
};
uint8_t SimplexNoise::perm[512] = {0};
uint8_t SimplexNoise::permMod12[512] = {0};

SimplexNoise::SimplexNoise(){
	
}

SimplexNoise::~SimplexNoise(){

}

void SimplexNoise::Init(int seed){
	//--- Shuffle elements by randomly exchanging each with one other.
    for (int i=0; i<255; i++) {
        int r = i + (seed % (255-i)); // Random remaining position.
        int temp = p[i]; 
		p[i] = p[r]; 
		p[r] = temp;
    }
	for ( uint16_t i = 0; i < 512; ++i ) {
        perm[i] = p[i & 255];
        permMod12[i] = static_cast<uint8_t>(perm[i] % 12);
    }
    //delete[] &p; // free up memory
}

double SimplexNoise::Noise2D(double xin, double yin){
	double s = (xin + yin) * F2;
    int32_t i = fastFloor( xin + s );
    int32_t j = fastFloor( yin + s );
    double t = (i + j) * G2;
    double x0 = xin - (i - t);
    double y0 = yin - (j - t);
    uint8_t i1 = 0, j1 = 1;
    if ( x0 > y0 ) {
        i1 = 1;
        j1 = 0;
    }
    double x1 = x0 - i1 + G2;
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0f + 2.0f * G2;
    double y2 = y0 - 1.0f + 2.0f * G2;
    uint8_t ii = i & 255;
    uint8_t jj = j & 255;
    uint8_t gi0 = permMod12[ii + perm[jj]];
    uint8_t gi1 = permMod12[ii + i1 + perm[jj + j1]];
    uint8_t gi2 = permMod12[ii + 1 + perm[jj + 1]];
    double n0 = 0.0f;
    double t0 = 0.5f - x0 * x0 - y0 * y0;
    if ( t0 >= 0.0f ) {
        t0 *= t0;
        n0 = t0 * t0 * dot( grad3[gi0], x0, y0 );
    }
    double n1 = 0.0f;
    double t1 = 0.5f - x1 * x1 - y1 * y1;
    if ( t1 >= 0.0f ) {
        t1 *= t1;
        n1 = t1 * t1 * dot( grad3[gi1], x1, y1 );
    }
    double n2 = 0.0f;
    double t2 = 0.5f - x2 * x2 - y2 * y2;
    if ( t2 >= 0.0 ) {
        t2 *= t2;
        n2 = t2 * t2 * dot( grad3[gi2], x2, y2 );
    }
	double toReturn = 70.0f * (n0 + n1 + n2);
	toReturn = (toReturn + 1.0f) * 0.5f;
    return toReturn;
}