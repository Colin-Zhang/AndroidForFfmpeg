package com.sefon.zhang.androidforffmpeg

import android.util.Log

class FFmpegUtils {
    private var listener: OnExecListener? = null

    /**
     * 调用底层执行
     * @param argc
     * @param argv
     * @return
     */
    private external fun execute(args :Int,commands: Array<String>): Int
    fun executeCommand(cmds: Array<String>, listener: OnExecListener) {
        this.listener = listener
        execute(cmds.size, cmds)
    }

    interface OnExecListener {
        fun onProgress()
    }
    fun onProgressCallBack(flag: Int) {
        if (listener!=null)
            listener!!.onProgress()
        Log.e("result","回调回调")
    }
    companion object {

        private var mInstance: FFmpegUtils? = null

        init {
            System.loadLibrary("native-lib")
            System.loadLibrary("avutil-55")
            System.loadLibrary("swresample-2")
            System.loadLibrary("avcodec-57")
            System.loadLibrary("avfilter-6")
            System.loadLibrary("swscale-4")
            System.loadLibrary("avformat-57")
        }

        public val minstance: FFmpegUtils
            get() {
                if (mInstance == null)
                    mInstance = FFmpegUtils()
                return mInstance!!
            }
    }
}
