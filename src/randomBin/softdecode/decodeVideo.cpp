#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavutil/adler32.h>
#include <libavfilter/avfilter.h>
#include <libavutil/imgutils.h>
}
#include <stdio.h>

void SaveFrame(AVFrame* pFrame, int width, int height, int index)
{
	cout << index << endl;
	FILE* pFile;
	char szFilename[32];
	int  y;

	// Open file
	sprintf(szFilename, "%d.png", index);//文件名
	pFile = fopen(szFilename, "wb");

	if (pFile == nullptr)
	{
		cout << "pFile = NULL" << endl;
		return;
	}

	// Write header
	fprintf(pFile, "P6\n%d %d\n255", width, height);
	cout << width << "   " << height << endl;

	// Write pixel data
	for (y = 0; y < height; y++)
	{
		fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);
	}

	// Close file
	fclose(pFile);

}

int main(int argc, char* argv[])
{
	//FFmpeg
	AVFormatContext* pFormatCtx;
	int	i, videoindex;

	FILE* fp_yuv;
	char filepath[] = "pikaqiu.mp4";

	avformat_network_init();//执行网络库的全局初始化，描述一个媒体文件或媒体流的构成和基本信息

	pFormatCtx = avformat_alloc_context();//分配一个解封装上下文指针

	//打开文件
	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {
		printf("Couldn't open input stream.\n");
		return -1;
	}

	//探寻文件中是否存在信息流
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		printf("Couldn't find stream information.\n");
		return -1;
	}

	//循环查找视频中包含的流信息，直到找到视频类型的流
	//便将其记录下来 保存到videoindex变量中
	//这里我们现在只处理视频流  音频流先不管
	videoindex = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoindex = i;
			break;
		}
	//如果videoStream为-1 说明没有找到视频流
	if (videoindex == -1) {
		printf("Didn't find a video stream.\n");
		return -1;
	}
	AVCodecContext* pCodecCtx = avcodec_alloc_context3(NULL);//描述编解码器上下文的数据结构，包含了众多编解码器需要的参数信息
	const AVCodec* pCodec;//存储编解码器信息的结构体
	avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoindex]->codecpar);//将音频流信息拷贝到新的AVCodecContext结构体中
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);//获取视频流的编码器
	//未发现编码器
	if (pCodec == NULL) {
		printf("Codec not found.\n");
		return -1;
	}
	//无法打开编码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Could not open codec.\n");
		return -1;
	}

	static struct SwsContext* img_convert_ctx;  //用于视频图像的转换
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
		pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
		AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

	int numBytes;
	numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height, 1);

	///=====第八步=====
	AVFrame* pFrame, * pFrameRGB;    //存储音视频原始数据（即未被编码的数据）的结构体
	pFrame = av_frame_alloc();
	pFrameRGB = av_frame_alloc();

	uint8_t* out_buffer;            //缓存
	out_buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
	av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, out_buffer, AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height, 1);

	AVPacket* packet;//解码
	int ret, got_picture;
	int y_size = pCodecCtx->width * pCodecCtx->height;

	packet = (AVPacket*)malloc(sizeof(AVPacket)); //分配一个packet
	av_new_packet(packet, y_size); //分配packet的数据

	av_dump_format(pFormatCtx, 0, filepath, 0); //输出视频信息

	int index = 0;

	while (av_read_frame(pFormatCtx, packet) >= 0)//判断视频是否读取完成
	{
		if (packet->stream_index == videoindex)
		{
			int k = avcodec_send_packet(pCodecCtx, packet);//将包发送到解码器
			got_picture = avcodec_receive_frame(pCodecCtx, pFrame);//接收解码帧

			cout << k << " " << got_picture << endl;

			if (k == 0 && got_picture == 0) {
				cout << k << " " << got_picture+1 << endl;
				sws_scale(img_convert_ctx,
					(uint8_t const* const*)pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
					pFrameRGB->linesize);

				//保存图片
				SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, index++); //保存图片
				
				if (index > 10) return 0; //这里我们就保存10张图片
			}
			else
			{
				return -1;
			}

			/*AVFrame* frame = av_frame_alloc();
			int frameFinished = 0;
			frameFinished = avcodec_send_packet(pCodecCtx, packet);
			if (frameFinished)
			{
				avcodec_receive_frame(pCodecCtx, frame);
				std::cout << frame;
			}*/
		}

	}
	cout << index+8 << endl;
	av_packet_unref(packet);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}