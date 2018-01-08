#include "image.h"
#include "utils.h"
#include "blas.h"
//#include "cuda.h"
#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/********************************************************
 * Function: transpose_image                            *
 * Input: Image type                                    *
 * output: None                                         *
 * 해당된 이미지 행렬의 transpose 됨.                   *
 * edited: siyeong                                      *
 ********************************************************/
void
transpose_image(image im)
{
    assert(im.w == im.h);
    int n, m;
    int c;      
    for(c = 0; c < im.c; ++c){ 
        for(n = 0; n < im.w-1; ++n){
            for(m = n + 1; m < im.w; ++m){      
                float swap = im.data[m + im.w*(n + im.h*c)];        
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];    
                im.data[n + im.w*(m + im.h*c)] = swap;                              
            }    
        }    
    }    
}

/********************************************************
 * Function: rotate_image                               *
 * Input: Image type, int times: 몇 번회전 시킬 것인지  *
 * output: None                                         *
 * 시계 방향으로 회전시키는 것으로 보임(추후 확인 필요) *
 * edited: siyeong                                      *
 ********************************************************/
void 
rotate_image_cw(image im, int times)
{
    assert(im.w == im.h);
    times = (times + 400) % 4; 
    int i, x, y, c;
    int n = im.w;
    for(i = 0; i < times; ++i){
        for(c = 0; c < im.c; ++c){
            for(x = 0; x < n/2; ++x){
                for(y = 0; y < (n-1)/2 + 1; ++y){
                    float temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }    
            }    
        }    
    }    
}
/********************************************************
 * Function: flip_image                                 *
 * Input: Image type								    *
 * output: None                                         *
 * 좌우 반전용 인 것으로 판단됨 (추후 확인 필요)	    *
 * edited: siyeong                                      *
 ********************************************************/
void 
flip_image(image a)
{   
    int i,j,k; 
    for(k = 0; k < a.c; ++k){
        for(i = 0; i < a.h; ++i){ 
            for(j = 0; j < a.w/2; ++j){
                int index = j + a.w*(i + a.h*(k));
                int flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                float swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }                                            
        }                                                
    }                                                    
}

/********************************************************
 * Function: image_distance                             *
 * Input: Image A, B								    *
 * output: None                                         *
 * 두 이미지가 다차원 벡터로 표현되었을 때의            *
 * 유클리드 거리 									    *
 * edited: siyeong                                      *
 ********************************************************/
image 
image_distance(image a, image b)                   
{                                                            
    int i,j;                                                     
    image dist = make_image(a.w, a.h, 1);                        
    for(i = 0; i < a.c; ++i){                                    
        for(j = 0; j < a.h*a.w; ++j){                            
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }                                                        
    }                                                                    
    for(j = 0; j < a.h*a.w; ++j){                                            
        dist.data[j] = sqrt(dist.data[j]);                                   
    }                                                                        
    return dist;                                                             
} 


