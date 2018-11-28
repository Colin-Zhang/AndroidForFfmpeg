package com.sefon.zhang.androidforffmpeg.view


import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.sefon.zhang.androidforffmpeg.R
import com.sefon.zhang.androidforffmpeg.model.common.ModelFlag
import com.sefon.zhang.androidforffmpeg.view.cutvideo.VideoCutActivity
import com.sefon.zhang.androidforffmpeg.view.getvoice.GetVoiceActivity
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity(), View.OnClickListener {


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        init()
    }

    fun init() {
        useCommand.setOnClickListener(this)
        videoControl.setOnClickListener(this)
        getVoice.setOnClickListener(this)
    }

    private fun openActivity(modelFlag: Int) {
        intent = Intent()
        when (modelFlag) {
            ModelFlag.MODEL_COMMAND -> intent.setClass(this, CommandActvity::class.java)
            ModelFlag.MODEL_VIDEO_CUT -> intent.setClass(this, VideoCutActivity::class.java)
            ModelFlag.MODEL_GET_VOICE -> intent.setClass(this, GetVoiceActivity::class.java)
        }
        startActivity(intent)
    }

    override fun onClick(v: View?) {
        when (v!!.id) {
            R.id.useCommand -> openActivity(ModelFlag.MODEL_COMMAND)
            R.id.videoControl -> openActivity(ModelFlag.MODEL_VIDEO_CUT)
            R.id.getVoice -> openActivity(ModelFlag.MODEL_GET_VOICE)
        }
    }
}
