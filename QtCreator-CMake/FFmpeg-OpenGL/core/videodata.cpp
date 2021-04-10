#include "videodata.h"

#include "qdebug.h"

extern "C"
{
    //#include <libavcodec/avcodec.h>
    //#include <libavformat/avformat.h>
    //#include <libswscale/swscale.h>
    //#include <libavdevice/avdevice.h>
    //#include <libavformat/version.h>
    //#include <libavutil/time.h>
    //#include <libavutil/mathematics.h>

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"

}

VideoData::VideoData(QObject *parent)
    : QThread(parent)
{

}

void VideoData::run()
{
#if 1
    av_register_all();
    avformat_network_init();
    AVFormatContext *pAVFomatContext = avformat_alloc_context();
    AVCodecContext* pAVCodecContext = nullptr;
    AVFrame *pAVFrame = av_frame_alloc();

    AVDictionary *opt = nullptr;
//    av_dict_set(&opt,"probesize","4096",0);
//    av_dict_set(&opt,"buffer_size","1024000",0);
//    av_dict_set(&opt,"stimeout","5000000",0); //wei miao 5000000
//    av_dict_set(&opt,"max_delay","0",0);
    av_dict_set(&opt,"rtsp_transport","udp",0);
    int result = avformat_open_input(&pAVFomatContext,m_url.toStdString().data(),nullptr,nullptr);
    if(result < 0){
        qDebug() << "open failed";//QStringLiteral("打开视频失败");
        return;
    }
    result = avformat_find_stream_info(pAVFomatContext,nullptr);
    if(result < 0){
        qDebug() << "open stream failed";//QStringLiteral("获取视频流信息失败");
        return;
    }

    int videoStreamIndex = -1;
    for(uint i = 0; i < pAVFomatContext->nb_streams; i++){
        if(pAVFomatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            videoStreamIndex = i;
            break;
        }
    }

    if(videoStreamIndex == -1){
        qDebug() << "get video stream index failed";//QStringLiteral("获取视频流索引失败");
        return;
    }

    int vden = 0, vnum = 0, fps;
    if(vden <= 0 || vnum <= 0){
        fps = 25;
        qDebug() << "get video fps error";
        qDebug() << "use default " << fps;
    }else{
        fps = vnum/vden;
        qDebug() << "video fps:" << fps;
    }

    pAVCodecContext = pAVFomatContext->streams[videoStreamIndex]->codec;
    int videoWidth = pAVCodecContext->width;
    int videoHeight = pAVCodecContext->height;

    AVCodec *pAVCodec;

//    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
    pAVCodec = avcodec_find_decoder_by_name("h264_qsv"); //h264_cuvid//查找n卡解码器
    if(!pAVCodec){
        qDebug() << "find h264_qsv failed";
        return;
    }

    int numBytes = avpicture_get_size(AV_PIX_FMT_NV12,videoWidth,videoHeight);
    uint8_t *out_buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));

    int y_size = pAVCodecContext->width * pAVCodecContext->height;
    AVPacket* pAVPacket = (AVPacket*) malloc(sizeof(AVPacket));
    av_new_packet(pAVPacket,y_size);
    av_dump_format(pAVFomatContext,0,m_url.toStdString().data(),0);
    result = avcodec_open2(pAVCodecContext,pAVCodec,nullptr);
    if(result < 0){
        qDebug() << "open decoder failed";//QStringLiteral("打开解码器失败");
        return;
    }

    qDebug() << "init stream failed";//QStringLiteral("视频流初始化成功");
    int got_picture = 0;
    while (!isInterruptionRequested()) {
        if(av_read_frame(pAVFomatContext,pAVPacket) <0){
            break;
        }
            avcodec_decode_video2(pAVCodecContext,pAVFrame,&got_picture,pAVPacket);
            if(got_picture){
                if(!is_started){
                    emit sigStarted(out_buffer,videoWidth,videoHeight);
                    is_started = true;
                }
                int bytes = 0;
                for(int i = 0; i < videoHeight; i++){ //将y分量拷贝
                    ::memcpy(out_buffer + bytes,pAVFrame->data[0] + pAVFrame->linesize[0] * i,videoWidth);
                    bytes += videoWidth;
                }
                int uv = videoHeight >> 1;
                for(int i = 0; i < uv; i++){ //将uv分量拷贝
                    ::memcpy(out_buffer + bytes,pAVFrame->data[1] + pAVFrame->linesize[1] * i,videoWidth);
                    bytes += videoWidth;
                }
                emit sigNewFrame(); //刷新
                msleep(fps);
            }
        av_free_packet(pAVPacket);
    }
//    av_free(pAVPicture);

    is_started = false;
    emit(is_started);
    avcodec_close(pAVCodecContext);
    avformat_close_input(&pAVFomatContext);
    av_free(out_buffer);
#endif
}
