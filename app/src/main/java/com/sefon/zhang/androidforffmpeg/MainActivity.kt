package com.sefon.zhang.androidforffmpeg

import android.app.ProgressDialog
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.ProgressBar
import kotlinx.android.synthetic.main.activity_main.*
import android.widget.Toast


class MainActivity : AppCompatActivity() {
    private lateinit var show: ProgressDialog
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        sample_text.setOnClickListener {
            val str = "ffmpeg -version "
            toExec(str)
        }
    }

    private fun toExec(cmd: String) {
        val cmds = cmd.split(" ".toRegex()).dropLastWhile { it.isEmpty() }.toTypedArray()
        show = ProgressDialog.show(this, null, "执行中...", true);
        FFmpegCmd.execommand(cmds, object : FFmpegCmd.OnExecListener {
            override fun onExecuted(ret: Int) {

                runOnUiThread {
                    Toast.makeText(this@MainActivity, "执行完成=$ret", Toast.LENGTH_SHORT).show()
                    show.dismiss()
                }
            }
        })

    }
}
