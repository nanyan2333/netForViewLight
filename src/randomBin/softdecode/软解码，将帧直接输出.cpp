
#include <iostream>
#include<fstream>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};
int main(int argc, char* argv[])
{
	//FFmpeg
	AVFormatContext* pFormatCtx;
	int				i, videoindex;
	AVDictionary* options = NULL;
	av_dict_set(&options, "buffer_size", "1024000", 0);
	FILE* fp_yuv;
	char filepath[] = "zc.mp4";

	avformat_network_init();

	pFormatCtx = avformat_alloc_context();

	if (avformat_open_input(&pFormatCtx, filepath, NULL, &options) != 0) {
		printf("Couldn't open input stream.\n");
		return -1;
	}
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoindex = i;
			break;
		}
	if (videoindex == -1) {
		printf("Didn't find a video stream.\n");
		return -1;
	}
	AVCodecContext* pCodecCtx = avcodec_alloc_context3(NULL);
	const AVCodec* pCodec;
	avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoindex]->codecpar);
	pCodec = avcodec_find_decoder( pCodecCtx ->codec_id);
	if (pCodec == NULL) {
		printf("Codec not found.\n");
		return -1;
	}
	pCodecCtx->thread_count = 16;
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Could not open codec.\n");
		return -1;
	}

	AVPacket packet;
	while (av_read_frame(pFormatCtx, &packet) >= 0)
	{
		if (packet.stream_index == videoindex)
		{
			AVFrame* frame=av_frame_alloc();
			int frameFinished = 0;
			frameFinished=avcodec_send_packet(pCodecCtx, &packet);
			if (frameFinished)
			{
				avcodec_receive_frame(pCodecCtx, frame);
				std::cout << frame<<std::endl;
			}
		}

	}
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	return 0;
}