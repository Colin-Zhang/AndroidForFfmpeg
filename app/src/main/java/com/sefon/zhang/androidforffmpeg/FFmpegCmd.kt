package com.sefon.zhang.androidforffmpeg

import android.widget.Toast





object FFmpegCmd {

    private var listener: OnExecListener? = null

    /**
     * 加载所有相关链接库
     */
    init {
        System.loadLibrary("native-lib")
        System.loadLibrary("avutil-55")
        System.loadLibrary("swresample-2")
        System.loadLibrary("avcodec-57")
        System.loadLibrary("avfilter-6")
        System.loadLibrary("swscale-4")
        System.loadLibrary("avformat-57")
    }

    /**
     * 调用底层执行
     * @param argc
     * @param argv
     * @return
     */
    private external fun execute(commands: Array<String>): Int

    fun onExecuted(ret: Int) {
        if (listener != null) {
            listener!!.onExecuted(ret)
        }
    }

    /**
     * 执行ffmoeg命令
     * @param cmds
     * @param listener
     */
    fun execommand(cmds: Array<String>, listener: OnExecListener) {
        FFmpegCmd.listener = listener
        execute( cmds)
    }

    /**
     * 执行完成/错误 时的回调接口
     */
    interface OnExecListener {
        fun onExecuted(ret: Int)
    }

}