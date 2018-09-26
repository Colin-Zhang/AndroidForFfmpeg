package com.sefon.zhang.androidforffmpeg



object FFmpegCmd {

    private var listener: OnExecListener? = null

    /**
     * 加载所有相关链接库
     */
    init {
        System.loadLibrary("ffmpegcmd")
        System.loadLibrary("avcodec")
        System.loadLibrary("avfilter")
        System.loadLibrary("avformat")
        System.loadLibrary("avutil")
        System.loadLibrary("swresample")
        System.loadLibrary("swscale")
    }

    /**
     * 调用底层执行
     * @param argc
     * @param argv
     * @return
     */
    external fun exec(argc: Int, argv: Array<String>): Int

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
        exec(cmds.size, cmds)
    }


    /**
     * 执行完成/错误 时的回调接口
     */
    interface OnExecListener {
        fun onExecuted(ret: Int)
    }
}