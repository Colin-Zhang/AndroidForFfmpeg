package com.sefon.zhang.androidforffmpeg.view.cutvideo

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.sefon.zhang.androidforffmpeg.R
import kotlinx.android.synthetic.main.activity_video_cut.*

class VideoCutActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_video_cut)
        shotVideo.setOnClickListener { startActivity(Intent(this,ShotActivity::class.java)) }
    }
}
