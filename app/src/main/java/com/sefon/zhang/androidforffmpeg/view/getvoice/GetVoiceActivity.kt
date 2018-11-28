package com.sefon.zhang.androidforffmpeg.view.getvoice

import android.annotation.SuppressLint
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View

import com.sefon.zhang.androidforffmpeg.R


import kotlinx.android.synthetic.main.activity_get_voice.*

import android.content.Intent

import android.graphics.drawable.AnimationDrawable
import android.media.MediaPlayer
import com.vincent.filepicker.Constant
import com.vincent.filepicker.Constant.*
import com.vincent.filepicker.activity.VideoPickActivity
import com.vincent.filepicker.activity.VideoPickActivity.IS_NEED_CAMERA

import android.util.Log
import com.sefon.zhang.androidforffmpeg.model.common.FFmpegUtils
import com.sefon.zhang.androidforffmpeg.view.custom.ExtractLoadingDialog
import com.vincent.filepicker.filter.entity.VideoFile
import com.vincent.filepicker.activity.AudioPickActivity.IS_NEED_RECORDER
import com.vincent.filepicker.activity.AudioPickActivity




class GetVoiceActivity : AppCompatActivity(), View.OnClickListener, FFmpegUtils.OnExecListener {
    private lateinit var player: MediaPlayer
    private lateinit var confirmDialog: ExtractLoadingDialog
    private var animationDrawable: AnimationDrawable? = null
    private lateinit var outPath: String
    override fun onProgressLog(info: String) {

    }

    override fun onFinish() {
        Thread.sleep(2000)
        stopAnimation()
        runOnUiThread {
            voiceLayout.visibility = View.VISIBLE
            voiceAddr.setText(outPath)
        }
    }

    override fun onClick(v: View?) {
        when (v!!.id) {
            R.id.pickFile ->
                openVoiceFile()
            R.id.playVoice -> playVoice()
            R.id.openVoiceAddr ->{
                val intent3 = Intent(this, AudioPickActivity::class.java)
                intent3.putExtra(IS_NEED_RECORDER, true)
                intent3.putExtra(Constant.MAX_NUMBER, 9)
                startActivityForResult(intent3, Constant.REQUEST_CODE_PICK_AUDIO)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_get_voice)
        init()
    }

    fun init() {
        playVoice.setOnClickListener(this)
        pickFile.setOnClickListener(this)
        openVoiceAddr.setOnClickListener(this)
    }


    private fun openVoiceFile() {
        val intent2 = Intent(this, VideoPickActivity::class.java)
        intent2.putExtra(IS_NEED_CAMERA, true)
        intent2.putExtra(Constant.MAX_NUMBER, 1)
        startActivityForResult(intent2, Constant.REQUEST_CODE_PICK_VIDEO)
    }

    private fun playVoice() {
        if (animationDrawable != null) {
            animationDrawable!!.stop()
            playVoice.setImageResource(R.drawable.voice_player_three)
            player.stop()
            player.release()
        } else {
            player = MediaPlayer()
            player.setDataSource(outPath)
            player.prepareAsync()
            player.start()
            playVoice.setImageResource(R.drawable.play_voice_animatoin)
            animationDrawable = playVoice.drawable as AnimationDrawable
            animationDrawable!!.start()
        }
    }

    @SuppressLint("SetTextI18n")
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        val list = data!!.getParcelableArrayListExtra<VideoFile>(Constant.RESULT_PICK_VIDEO)
        when (requestCode) {
            REQUEST_CODE_PICK_VIDEO -> {
                startExtract(list[0].path)
                voice_duration.text = list[0].duration.toString() + "″"
            }
        }
    }

    private fun startExtract(path: String) {
        startAnimation()
        videoAddr.setText(path)
        outPath = path.substring(0, path.lastIndexOf("/")) + "/demo.ac3"
        val command = "ffmpeg -i $path -vn -y -acodec copy $outPath"
        Thread(Runnable {
            FFmpegUtils.instance.executeCommand(getArr(command), this)
        }).start()
    }

    private fun startAnimation() {
        confirmDialog = ExtractLoadingDialog(this)
        confirmDialog.show()
    }

    private fun stopAnimation() {
        confirmDialog.cancel()
    }

    private fun getArr(line: String): Array<String> {
        val array = line.split(" ").toTypedArray()
        for (item in array.indices) {
            array[item] = array[item].trim()
            Log.e("command : ", array[item])
        }
        return array
    }
}
