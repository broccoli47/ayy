#include <iostream>
#include <stdio.h>

using namespace std;

struct Matrix{
    unsigned int height;
    unsigned int width;

    double *values;

    void Input(){
        printf("Creating Matrix...\n");
        printf("Enter Height: ");
        scanf("%d", &height);
        printf("Enter Width: ");
        scanf("%d", &width);
        printf("Creating Matrix of size %dx%d\n", height, width);

        Allocate();

        for(unsigned int h=0; h<height; ++h){
            for(unsigned int w=0; w<width; ++w){
                printf("m [%d, %d]: ", h, w);
                scanf("%lf", &values[width * h + w]);
            }
        }
    }

    void Allocate(){
        delete [] values;
        values = new double[height*width];
    }

    void Null(){
        for(unsigned int i=0; i<width*height; ++i){
            values[i] = 0;
        }
    }

    void SetDiagonal(double Value){
        for(unsigned int i=0; i<(width<height?width:height); ++i){
            Set(i,i,Value);
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

    double Get(unsigned int x, unsigned int y){
        //if(x < width && y < height){
            return values[y*width + x];
        //}
    }

    Matrix operator+(Matrix& m){
        Matrix r;
        r.width = width;
        r.height = height;
        r.Allocate();

        for(unsigned int y=0; y<height; ++y){
            for(unsigned int x=0; x<width; ++x){
                r.Set(x,y, Get(x,y) + m.Get(x,y));
            }
        }

        return r;
    }

    Matrix operator*(Matrix& m){
        //Assert(width == m.height);

        Matrix r;
        r.width = m.width;
        r.height = height;
        r.Allocate();


        /*for(int j=0; j<height; ++j){
            for(int i=0; i<width; ++i){
                for(int l=0; l<m->height; ++l){
                    for(int k=0; k<m->width; ++k){
                        r.Set(, , Get(i,j) * m->Get(k, l));
                    }
                }
            }
        }*/

        for(unsigned int j=0; j<r.height; ++j){
            for(unsigned int i=0; i<r.width; ++i){
                float value = 0.0;

                //printf("%d,%d = ",i,j);

                for(unsigned int d=0;d<width;++d){
                    //printf("%lf + ", Get(d,j) * m.Get(i, d));
                    value += Get(d,j) * m.Get(i, d);
                }

                // This not multiply, it's SUPERMULTIPLY
                // Kinda wonder what meaning this shit has. Well, we'll never know.
                /*for(unsigned int row=0; row<width; ++row){
                    for(unsigned int col=0; col<m->height; ++col){
                        printf("%lf + ", Get(row,j) * m->Get(i, col));
                        value += Get(row,j) * m->Get(i, col);
                    }
                }*/

                //printf("= %lf\n", value);

                r.Set(i,j,value);
            }
        }

        return r;
    }

    void Copy(Matrix& m){
        width = m.width;
        height = m.height;
        Allocate();

        for(unsigned int y=0; y<height*width; ++y){
            values[y] = m.values[y];
        }
    }

    Matrix Inverse(){
        Matrix m;
        m.width = width*2;
        m.height = height;

        for(unsigned int y=0; y<height; ++y){
            for(unsigned int x=0; x<width; ++x){
                m.Set(x,y,Get(x,y));
            }
            for(unsigned int x=0; x<width; ++x){
                m.Set(x+width,y,x==y?1.0:0.0);
            }
        }

        m.GaussEliminationBrute();

        Matrix result;
        result.width = width;
        result.height = height;
        result.Allocate();

        for(unsigned int y=0; y<height; ++y){
            for(unsigned int x=0; x<width; ++x){
                result.Set(x,y,m.Get(width+x,y));
            }
        }

        return result;
    }

    void GaussEliminationBrute(){

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
    m.Input();
    m.Display();
    Matrix n;
    n.Input();
    n.Display();

    //printf("\nMultiplication\n");
    Matrix result = m*n;//m.Multiply(&n);
    result.Display();

    //printf("\nAddition\n");
    //result = m+n;
    //result.Display();

    //printf("\nZadanie\n\n");

    //m.GaussEliminationBrute();

    //Matrix inv = m.Inverse();
    //inv.Display();

    getchar();
    getchar();
    getchar();

    return 0;
}
