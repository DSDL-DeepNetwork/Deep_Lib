#include <stdio.h>
#include <math.h>

/************************************************/
/*함수이름 col2im_add_pixel*/
/*입력: *im : 타겟 이미지 주소
        height, width : 타겟 이미지 규격
        channel : RGB 선택인수
        row, col : 타겟 픽셀 좌표
        pad : 픽셀 대각 행렬 이동 위한 인수
	val : 이미지의 타겟 픽셀의 타겟 channel 값*/
/*출력: 없음*/
/*기능: 이미지의 타겟 픽셀의 타겟 channel의 값을 im 배열의 해당 위치에 저장*/

/************************************************/

void col2im_add_pixel(float *im, int height, int width, int channels,
                        int row, int col, int channel, int pad, float val)
{
    row -= pad;
    col -= pad;

    if (row < 0 || col < 0 ||
        row >= height || col >= width) return;
    im[col + width*(row + height*channel)] += val;
}

/**********************************************************************
input   :*data_im: image data 주소값
         channels : channel 수
         height : image height  width : image width
         ksize : kernel size    stride : stride
         pad : padding
output  : None
기능: column으로 저장된 각 픽셀 데이터를 im배열에 저장 
ref: https://arxiv.org/pdf/1603.07285.pdf page 15 (relationship 6)
*********************************************************************/

void col2im_cpu(float* data_col,
         int channels,  int height,  int width,
         int ksize,  int stride, int pad, float* data_im) 
{
    int c,h,w;
    int height_col = (height + 2*pad - ksize) / stride + 1;
    int width_col = (width + 2*pad - ksize) / stride + 1;

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
                double val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

