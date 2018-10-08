package com.sefon.zhang.androidforffmpeg.view

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.text.TextUtils
import android.util.Log
import android.widget.Toast
import com.sefon.zhang.androidforffmpeg.R
import com.sefon.zhang.androidforffmpeg.model.common.FFmpegUtils
import kotlinx.android.synthetic.main.activity_command_actvity.*


class CommandActvity : AppCompatActivity() , FFmpegUtils.OnExecListener{

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_command_actvity)
        sample_text.setOnClickListener {
            log_text.setText("")
            if (!TextUtils.isEmpty(command_edit.text))
                execute(command_edit.text.toString())
            else
                Toast.makeText(this, "command is null", Toast.LENGTH_SHORT).show()
        }
    }

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


    private fun execute(command: String) {
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
