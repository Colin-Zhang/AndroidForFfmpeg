package com.sefon.zhang.androidforffmpeg.view.cutvideo

import android.annotation.SuppressLint
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.os.Message
import android.util.Log
import android.view.View
import android.view.WindowManager
import com.sefon.zhang.androidforffmpeg.R

import kotlinx.android.synthetic.main.activity_shot.*





class ShotActivity : AppCompatActivity(), View.OnClickListener {
    private var helper: MediaHelper? = null
    private var mProgressNumber: Int? = null
    override fun onClick(v: View?) {
        when (v!!.id) {
            R.id.inversion -> helper!!.autoChangeCamera()
            R.id.close -> finish()
            R.id.start_video -> startShot()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val flag = WindowManager.LayoutParams.FLAG_FULLSCREEN
        window.setFlags(flag, flag)
        setContentView(R.layout.activity_shot)
        init()
    }

    fun init() {
        helper = MediaHelper(this)
        helper!!.setSurfaceView(video_surface_view)
        inversion.setOnClickListener(this)
        close.setOnClickListener(this)
        start_video.setOnClickListener(this)
    }


    @SuppressLint("SetTextI18n")
    private fun startView() {
        start_video.visibility = View.GONE
        start_video_ing.visibility = View.VISIBLE
        mProgressNumber = 0
        time.text = "00:00"
        handler.removeMessages(0)
        handler.sendMessage(handler.obtainMessage(0))
    }

    private fun startShot() {
        mProgressNumber = 0
        progress.progress = 0
        startView()
    }

    private val handler: Handler = object : Handler(Looper.getMainLooper()) {
        @SuppressLint("SetTextI18n")
        override fun handleMessage(msg: Message?) {
           when(msg!!.what) {
                0 ->  {
                    Log.e("progress", mProgressNumber.toString())
                    progress.progress =mProgressNumber!!
                    val index :String = if (mProgressNumber!! < 10)
                        "0$mProgressNumber"
                    else
                        mProgressNumber.toString()
                    time.text= "00:$index"
                    if (progress.progress>=progress.max) {
                        helper!!.stopRecordSave()
                    }else if (helper!!.isRecording){
                        mProgressNumber = mProgressNumber!! + 1
                        sendMessageDelayed(this.obtainMessage(0), 1000)
                    }
                }
           }
        }
    }


}


