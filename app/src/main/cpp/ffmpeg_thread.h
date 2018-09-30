//
// Created by zhang on 2018/9/29.
//

#ifndef ANDROIDFORFFMPEG_FFMPEG_THREAD_H
#define ANDROIDFORFFMPEG_FFMPEG_THREAD_H

#endif //ANDROIDFORFFMPEG_FFMPEG_THREAD_H
int  run_cmd_thread (int cmdnum, char **argv,void* context);

        void thread_exit(int ret);
void*  threadCallBack(void* context);