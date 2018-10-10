package com.sefon.zhang.androidforffmpeg.view.cutvideo

import android.Manifest
import android.annotation.SuppressLint
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.os.Message

import android.view.View
import android.view.WindowManager
import com.sefon.zhang.androidforffmpeg.R
import com.sefon.zhang.androidforffmpeg.model.utils.FileUtils
import com.sefon.zhang.androidforffmpeg.model.utils.MediaHelper
import com.sefon.zhang.androidforffmpeg.model.utils.PermissionHelper

import kotlinx.android.synthetic.main.activity_shot.*
import java.io.File
import java.util.*




class ShotActivity : AppCompatActivity(), View.OnClickListener {
    private var helper: MediaHelper? = null
    private var mProgressNumber: Int? = null
    private var permissionHelper:PermissionHelper ?= null
    val PERMISSIONS = arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO, Manifest.permission.CAMERA)

    override fun onClick(v: View?) {
        when (v!!.id) {
            R.id.inversion -> helper!!.autoChangeCamera()
            R.id.close -> finish()
            R.id.start_video -> startShot()
            R.id.start_video_ing -> stopShot()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val flag = WindowManager.LayoutParams.FLAG_FULLSCREEN
        window.setFlags(flag, flag)
        setContentView(R.layout.activity_shot)
        permissionHelper = PermissionHelper(this)
        init()
    }

    fun init() {
        helper = MediaHelper(this)
        helper!!.setTargetName(UUID.randomUUID().toString()+".mp4")
        helper!!.setTargetDir(File(FileUtils(this).storageDirectory))
        inversion.setOnClickListener(this)
        close.setOnClickListener(this)
        start_video.setOnClickListener(this)
        start_video_ing.setOnClickListener(this)
    }


    @SuppressLint("SetTextI18n")
    private fun startView() {
        time.text = "00:00"
        handler.removeMessages(0)
        handler.sendMessage(handler.obtainMessage(0))
    }

    private fun startShot() {
        start_video.visibility = View.GONE
        start_video_ing.visibility = View.VISIBLE
        mProgressNumber = 0
        progress.progress = 0
        helper!!.record()
        startView()
    }
    private fun stopShot() {
        mProgressNumber = 0
        progress.progress = 0
        start_video.visibility = View.VISIBLE
        start_video_ing.visibility = View.GONE
        helper!!.stopRecordSave()
    }
    private val handler: Handler = object : Handler(Looper.getMainLooper()) {
        @SuppressLint("SetTextI18n")
        override fun handleMessage(msg: Message?) {
           when(msg!!.what) {
                0 ->  {
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
                        sendMessageDelayed(obtainMessage(0), 1000)
                    }
                }
           }
        }
    }

    override fun onResume() {
        super.onResume()
        if (permissionHelper!!.lacksPermissions(PERMISSIONS)) {

        }else{
            helper!!.setSurfaceView(video_surface_view)
        }
    }

}


