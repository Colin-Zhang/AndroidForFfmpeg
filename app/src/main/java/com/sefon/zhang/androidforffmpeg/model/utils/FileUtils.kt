package com.sefon.zhang.androidforffmpeg.model.utils

import android.content.Context
import android.os.Environment

import java.io.File

class FileUtils(context: Context) {

    /**
     * 获取储存Image的目录
     * @return
     */
    val storageDirectory: String
        get() {
            val localPath = if (Environment.getExternalStorageState() == Environment.MEDIA_MOUNTED)
                mSdRootPath + FOLDER_NAME
            else
                mDataRootPath!! + FOLDER_NAME
            val folderFile = File(localPath)
            if (!folderFile.exists()) {
                folderFile.mkdir()
            }
            return localPath
        }

    init {
        mDataRootPath = context.cacheDir.path
        makeAppDir()
    }

    fun makeAppDir(): String {
        var path = storageDirectory
        var folderFile = File(path)
        if (!folderFile.exists()) {
            folderFile.mkdir()
        }
        path = path + IMAGE_NAME
        folderFile = File(path)
        if (!folderFile.exists()) {
            folderFile.mkdir()
        }
        return path
    }

    /**
     * 删除文件
     */
    fun deleteFile(deletePath: String, videoPath: String) {
        val file = File(deletePath)
        if (file.exists()) {
            val files = file.listFiles()
            for (f in files) {
                if (f.isDirectory) {
                    if (f.listFiles().size == 0) {
                        f.delete()
                    } else {
                        deleteFile(f.absolutePath, videoPath)
                    }
                } else if (f.absolutePath != videoPath) {
                    f.delete()
                }
            }
        }
    }

    companion object {
        /**
         * sd卡的根目录
         */
        private val mSdRootPath = Environment.getExternalStorageDirectory().path
        /**
         * 手机的缓存根目录
         */
        private var mDataRootPath: String? = null
        /**
         * 保存Image的目录名
         */
        private val FOLDER_NAME = "/ffmpeg"

        val IMAGE_NAME = "/cache"
    }

}
