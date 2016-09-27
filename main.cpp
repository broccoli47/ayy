#include <iostream>
#include <stdio.h>

using namespace std;

struct Matrix{
    unsigned int height;
    unsigned int width;

    double *values;

    void AssignMatrix(){
        printf("Creating Matrix...\n");
        printf("Enter Height: ");
        scanf("%d", &height);
        printf("Enter Width: ");
        scanf("%d", &width);
        printf("Creating Matrix of size %dx%d\n", height, width);

        delete [] values;
        values = new double[height*width];

        for(unsigned int h=0; h<height; ++h){
            for(unsigned int w=0; w<width; ++w){
                printf("m [%d, %d]: ", h, w);
                scanf("%lf", &values[width * h + w]);
            }
        }
    }

    void Display(){
        //cout << "Matrix (" << height << "x" << width << ")" << endl;
        printf("Matrix (%ux%u)\n", height, width);

        for(unsigned int h=0; h<height; ++h){
            printf("|");

            for(unsigned int w=0; w<width; ++w){
                printf("% 0.2f ", values[width * h + w]);
            }

            printf("|\n");
        }

        cout << endl;
    }

    void Set(unsigned int x, unsigned int y, double f){
        if(x < width && y < height){
            values[y*width + x] = f;
        }
    }

    void BruteSolve(){

        unsigned int diagonalLength = height<width?height:width;

        printf("\n1. Faza: Uprava na trojuholnikovy tvar\n\n");

        for(unsigned int d=0;d<diagonalLength;++d){

            // If zero we FUCKED (although that doesn't necessarily mean it's not solvable)
            /*if(values[d*width + d] == 0.0){
                printf("Oh boy!");
                return;
            }*/

            for(unsigned int w=d+1; w<width; ++w){
                values[d*width + w] /= values[d*width + d];
            }
            values[d*width + d] = 1.0;

            Display();

            for(unsigned int h=d+1; h<height; ++h){
                for(unsigned int w=d+1; w<width; ++w){
                    values[h*width + w] -= values[width*d + w] * values[h*width + d];
                }
                values[h*width + d] = 0.0;
            }

            Display();
        }

        printf("2. Faza: Uprava na jednotkovu maticu\n\n");


        /*values[width*(d-1) + d] = 0.0
        values[width*(d-2) + d] = 0.0
        values[width*(d-3) + d] = 0.0
        values[width*(d-4) + d] = 0.0
        ...
        values[width*0 + d] = 0.0

        values[width*(d-1) + d + 1] -= values[width*(d-1) + d] * values[width*d + d + 1];
        values[width*(d-1) + d + 2] -= values[width*(d-1) + d] * values[width*d + d + 2];
        values[width*(d-1) + d + 3] -= values[width*(d-1) + d] * values[width*d + d + 3];
        values[width*(d-1) + d + 4] -= values[width*(d-1) + d] * values[width*d + d + 4];
        ...*/

        for(unsigned int d=0;d<diagonalLength;++d){
            for(unsigned int c=1; c<=d; ++c){
                for(unsigned int e = d+1; e<width; ++e){
                    values[width*(d-c) + e] -= values[width*(d-c) + d] * values[width*d + e];
                }

                values[width*(d-c) + d] = 0.0;

                Display();
            }
        }
    }
};

int main()
{
    Matrix m;
    m.AssignMatrix();

    printf("\nZadanie\n\n");

    m.Display();
    m.BruteSolve();

    return 0;
}
