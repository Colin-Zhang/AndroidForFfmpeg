package com.sefon.zhang.androidforffmpeg.view.cutvideo

import android.annotation.SuppressLint
import android.app.Activity
import android.hardware.Camera
import android.media.MediaRecorder
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View

import java.io.File
import java.io.IOException

/**
 * Created by tangyx on 2017/8/2.
 *
 */

class MediaHelper(private val activity: Activity) : SurfaceHolder.Callback {
    private var mMediaRecorder: MediaRecorder? = null
    private var mCamera: Camera? = null
    private var mSurfaceView: SurfaceView? = null
    private var mSurfaceHolder: SurfaceHolder? = null
    private var targetDir: File? = null
    private var targetName: String? = null
    private var targetFile: File? = null
    var isRecording: Boolean = false
        private set
    private var mDetector: GestureDetector? = null
    private var isZoomIn = false
    private val or = 90
    private var position = Camera.CameraInfo.CAMERA_FACING_BACK

    val targetFilePath: String
        get() = targetFile!!.path

    fun setTargetDir(file: File) {
        this.targetDir = file
    }

    fun setTargetName(name: String) {
        this.targetName = name
    }

    fun deleteTargetFile(): Boolean {
        return if (targetFile!!.exists()) {
            targetFile!!.delete()
        } else {
            false
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    fun setSurfaceView(view: SurfaceView) {
        this.mSurfaceView = view
        mSurfaceHolder = mSurfaceView!!.holder
        mSurfaceHolder!!.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS)
        mSurfaceHolder!!.addCallback(this)
        mDetector = GestureDetector(activity, ZoomGestureListener())
        mSurfaceView!!.setOnTouchListener { v, event ->
            mDetector!!.onTouchEvent(event)
            true
        }
    }

    fun record() {
        if (isRecording) {
            try {
                mMediaRecorder!!.stop()  // stop the recording
            } catch (e: RuntimeException) {
                e.printStackTrace()
                targetFile!!.delete()
            }

            releaseMediaRecorder() // release the MediaRecorder object
            mCamera!!.lock()         // take camera access back from MediaRecorder
            isRecording = false
        } else {
            startRecordThread()
        }
    }

    private fun prepareRecord(): Boolean {
        try {
            mMediaRecorder = MediaRecorder()
            mCamera!!.unlock()
            mMediaRecorder!!.setCamera(mCamera)
            mMediaRecorder!!.setAudioSource(MediaRecorder.AudioSource.DEFAULT)
            mMediaRecorder!!.setVideoSource(MediaRecorder.VideoSource.CAMERA)
            //                mMediaRecorder.setProfile(profile);
            mMediaRecorder!!.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4)
            mMediaRecorder!!.setVideoSize(1280, 720)
            //                mMediaRecorder.setVideoSize(640, 480);
            mMediaRecorder!!.setVideoEncodingBitRate(2 * 1024 * 1024)
            mMediaRecorder!!.setVideoEncoder(MediaRecorder.VideoEncoder.H264)
            mMediaRecorder!!.setAudioEncoder(MediaRecorder.AudioEncoder.AAC)
            if (position == Camera.CameraInfo.CAMERA_FACING_BACK) {
                mMediaRecorder!!.setOrientationHint(or)
            } else {
                mMediaRecorder!!.setOrientationHint(270)
            }
            targetFile = File(targetDir, targetName!!)
            mMediaRecorder!!.setOutputFile(targetFile!!.path)

        } catch (e: Exception) {
            e.printStackTrace()
            releaseMediaRecorder()
            return false
        }

        try {
            mMediaRecorder!!.prepare()
        } catch (e: IllegalStateException) {
            e.printStackTrace()
            releaseMediaRecorder()
            return false
        } catch (e: IOException) {
            e.printStackTrace()
            releaseMediaRecorder()
            return false
        }

        return true
    }

    fun stopRecordSave() {
        if (isRecording) {
            isRecording = false
            try {
                mMediaRecorder!!.stop()
            } catch (r: RuntimeException) {
                r.printStackTrace()
            } finally {
                releaseMediaRecorder()
            }
        }
    }

    fun stopRecordUnSave() {
        if (isRecording) {
            isRecording = false
            try {
                mMediaRecorder!!.stop()
            } catch (r: RuntimeException) {
                if (targetFile!!.exists()) {
                    //不保存直接删掉
                    targetFile!!.delete()
                }
            } finally {
                releaseMediaRecorder()
            }
            if (targetFile!!.exists()) {
                //不保存直接删掉
                targetFile!!.delete()
            }
        }
    }

    private fun startPreView(holder: SurfaceHolder?) {
        if (mCamera == null) {
            mCamera = Camera.open(position)
        }
        if (mCamera != null) {
            mCamera!!.setDisplayOrientation(or)
            try {
                mCamera!!.setPreviewDisplay(holder)
                val parameters = mCamera!!.parameters
                val mSupportedPreviewSizes = parameters.supportedPreviewSizes
                if (mSupportedPreviewSizes != null) {
                    val width = mSurfaceView!!.width
                    val height = mSurfaceView!!.height
                    val mPreviewSize = getOptimalPreviewSize(mSupportedPreviewSizes,
                            Math.max(width, height), Math.min(width, height))
                    parameters.setPreviewSize(mPreviewSize!!.width, mPreviewSize.height)
                }
                val focusModes = parameters.supportedFocusModes
                if (focusModes != null) {
                    for (mode in focusModes) {
                        if (mode.contains(Camera.Parameters.FOCUS_MODE_AUTO)) {
                            parameters.focusMode = Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO
                        }
                    }
                }
                mCamera!!.parameters = parameters
                mCamera!!.startPreview()
            } catch (e: IOException) {
                e.printStackTrace()
            }

        }
    }

    fun getOptimalPreviewSize(sizes: List<Camera.Size>?, w: Int, h: Int): Camera.Size? {
        val ASPECT_TOLERANCE = 0.1
        val targetRatio = w.toDouble() / h
        if (sizes == null) {
            return null
        }
        var optimalSize: Camera.Size? = null
        var minDiff = java.lang.Double.MAX_VALUE
        for (size in sizes) {
            val ratio = size.width.toDouble() / size.height
            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE)
                continue
            if (Math.abs(size.height - h) < minDiff) {
                optimalSize = size
                minDiff = Math.abs(size.height - h).toDouble()
            }
        }
        if (optimalSize == null) {
            minDiff = java.lang.Double.MAX_VALUE
            for (size in sizes) {
                if (Math.abs(size.height - h) < minDiff) {
                    optimalSize = size
                    minDiff = Math.abs(size.height - h).toDouble()
                }
            }
        }
        return optimalSize
    }

    private fun releaseMediaRecorder() {
        if (mMediaRecorder != null) {
            mMediaRecorder!!.reset()
            mMediaRecorder!!.release()
            mMediaRecorder = null
        }
    }

    fun releaseCamera() {
        if (mCamera != null) {
            mCamera!!.release()
            mCamera = null
        }
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        mSurfaceHolder = holder
        startPreView(holder)
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {

    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        if (mCamera != null) {
            releaseCamera()
        }
        if (mMediaRecorder != null) {
            releaseMediaRecorder()
        }
    }

    private fun startRecordThread() {
        if (prepareRecord()) {
            try {
                mMediaRecorder!!.start()
                isRecording = true
            } catch (r: RuntimeException) {
                r.printStackTrace()
                releaseMediaRecorder()
            }

        }
    }

    private inner class ZoomGestureListener : GestureDetector.SimpleOnGestureListener() {
        //双击手势事件
        override fun onDoubleTap(e: MotionEvent): Boolean {
            super.onDoubleTap(e)
            if (!isZoomIn) {
                setZoom(20)
                isZoomIn = true
            } else {
                setZoom(0)
                isZoomIn = false
            }
            return true
        }
    }

    private fun setZoom(zoomValue: Int) {
        var zoomValue = zoomValue
        if (mCamera != null) {
            val parameters = mCamera!!.parameters
            if (parameters.isZoomSupported) {
                val maxZoom = parameters.maxZoom
                if (maxZoom == 0) {
                    return
                }
                if (zoomValue > maxZoom) {
                    zoomValue = maxZoom
                }
                parameters.zoom = zoomValue
                mCamera!!.parameters = parameters
            }
        }
    }

    fun autoChangeCamera() {
        if (position == Camera.CameraInfo.CAMERA_FACING_BACK) {
            position = Camera.CameraInfo.CAMERA_FACING_FRONT
        } else {
            position = Camera.CameraInfo.CAMERA_FACING_BACK
        }
        releaseCamera()
        stopRecordUnSave()
        startPreView(mSurfaceHolder)
    }
}

