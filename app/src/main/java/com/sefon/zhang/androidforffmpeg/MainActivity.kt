package com.sefon.zhang.androidforffmpeg


import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.text.TextUtils


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

        Thread(Runnable {
            FFmpegUtils.instance.executeCommand(getArr(command), this)
        }).start()
    }

    fun getArr(line: String): Array<String> {
        return line.split(" ").toTypedArray()
    }


}
