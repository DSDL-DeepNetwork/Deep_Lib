#include "im2col.h"
#include <stdio.h>


/************************************************/
/*함수이름 im2col_get_pixel*/
/*입력: *im : 타겟 이미지 주소
        height, width : 타겟 이미지 규격
        channel : RGB 선택인수
        row, col : 타겟 픽셀 좌표
        pad : 픽셀 대각 행렬 이동 위한 인수*/
/*출력: row, col 입력이 잘못되었으면 0
        문제가 없다면 이미지의 타겟 픽셀의 타겟 channel의 값을 반환*/

/************************************************/

float im2col_get_pixel(float *im, int height, int width, int channels,
                        int row, int col, int channel, int pad)
{
    row -= pad;
    col -= pad;

    if (row < 0 || col < 0 ||
        row >= height || col >= width) return 0;
    return im[col + width*(row + height*channel)];
}

/**********************************************************************
input	:*data_im: image data 주소값
	 channels : channel 수
	 height : image height	width : image width
	 ksize : kernel size	stride : stride
	 pad : padding		
output	: None 
기능: *data_col : column으로 저장된 image date 주소값
ref: https://arxiv.org/pdf/1603.07285.pdf page 15 (relationship 6)
*********************************************************************/
void im2col_cpu(float* data_im,
     int channels,  int height,  int width,
     int ksize,  int stride, int pad, float* data_col)
{
    int c,h,w;
    int height_col = (height + 2*pad - ksize) / stride + 1; //ref
    int width_col = (width + 2*pad - ksize) / stride + 1;   //ref

    int channels_col = channels * ksize * ksize;
    for (c = 0; c < channels_col; ++c) {
        int w_offset = c % ksize;
        int h_offset = (c / ksize) % ksize;
        int c_im = c / ksize / ksize;
        for (h = 0; h < height_col; ++h) {
            for (w = 0; w < width_col; ++w) {
                int im_row = h_offset + h * stride;
                int im_col = w_offset + w * stride;
                int col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}


