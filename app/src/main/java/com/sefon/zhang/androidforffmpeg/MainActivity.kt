package com.sefon.zhang.androidforffmpeg

import android.app.ProgressDialog
import android.media.MediaPlayer
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.SystemClock
import android.os.SystemClock.currentThreadTimeMillis
import android.util.Log
import android.widget.ProgressBar
import kotlinx.android.synthetic.main.activity_main.*
import android.widget.Toast


class MainActivity : AppCompatActivity(), FFmpegUtils.OnExecListener {
    var startTime: Long = 0
    override fun onProgress() {

        val endTime = SystemClock.currentThreadTimeMillis()
        runOnUiThread {
            Toast.makeText(this, "执行完成", Toast.LENGTH_SHORT).show()
            Log.e("tag", "使用时间：" + (endTime - startTime) + "毫秒")
        }
    }


    private lateinit var show: ProgressDialog
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        sample_text.setOnClickListener {

            crop()
        }
    }


    private fun crop() {
        startTime = currentThreadTimeMillis()
        val commands = arrayOf("ffmpeg", "-y", "-i", "/storage/emulated/0/DCIM/Camera/5b769345be3365e7bfaa3350e563b330.mp4", "-vframes", "30", "-y", "-f", "gif", "/sdcard/crop_30.gif")
        Thread(Runnable {
            FFmpegUtils.minstance.executeCommand(commands, this)
        }).start()
    }

}
