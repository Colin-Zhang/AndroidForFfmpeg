package com.sefon.zhang.androidforffmpeg


import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.text.TextUtils
import android.util.Log


import kotlinx.android.synthetic.main.activity_main.*
import android.widget.Toast
import java.nio.file.Files.size


class MainActivity : AppCompatActivity(), FFmpegUtils.OnExecListener {
    override fun onProgressLog(info: String) {
        runOnUiThread {
            log_text.append(info)
        }
    }

    override fun onFinish() {
        runOnUiThread {
            Toast.makeText(this, "执行完成", Toast.LENGTH_SHORT).show()
            log_text.append("Execute Finish!!!")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        sample_text.setOnClickListener {
            log_text.setText("")
            if (!TextUtils.isEmpty(command_edit.text))
                execute(command_edit.text.toString())
            else
                Toast.makeText(this, "command is null", Toast.LENGTH_SHORT).show()
        }
    }

    private fun execute(command: String) {
        val commands = arrayOf("ffmpeg", "-y", "-i", "/storage/emulated/0/DCIM/Camera/5b769345be3365e7bfaa3350e563b330.mp4", "-vframes", "30", "-y", "-f", "gif", "/sdcard/crop_30.gif")

        Thread(Runnable {
            FFmpegUtils.instance.executeCommand(getArr(command), this)
        }).start()
    }

    fun getArr(line: String): Array<String> {
        var array = line.split(" ").toTypedArray()
        for (item in array.indices) {
            array[item] = array[item].trim()
            Log.e("command : " ,array[item])
        }
        return array
    }


}
