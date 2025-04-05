package com.crowxr.app;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.camera.core.CameraSelector;
import androidx.camera.core.ImageAnalysis;
import androidx.camera.core.resolutionselector.ResolutionSelector;
import androidx.camera.core.resolutionselector.ResolutionStrategy;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.util.Log;
import android.util.Size;

import com.crowxr.app.databinding.ActivityMainBinding;
import com.google.common.util.concurrent.ListenableFuture;

import java.nio.ByteBuffer;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity {
    private final String TAG = "CrowXRApp";
    private final String[] REQUIRED_PERMISSIONS = new String[] { android.Manifest.permission.CAMERA, android.Manifest.permission.RECORD_AUDIO };
    private final int REQUEST_CODE_PERMISSIONS = 1;

    private ActivityMainBinding viewBinding;
    private ExecutorService cameraExecutor;

    static {
        System.loadLibrary("CrowXR");
    }

    public native void crop(byte[] srcByteArray, byte[] dstByteArray, int input_width, int input_height, int roi_x, int roi_y, int roi_width, int roi_height);
    public native void rescale(byte[] srcByteArray, byte[] dstByteArray, int input_width, int input_height, int output_width, int output_height);
    public native void hconcate(byte[] srcByteArrayL, byte[] srcByteArrayR, byte[] dstByteArray, int left_width, int right_width, int input_height);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        viewBinding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(viewBinding.getRoot());

        if (allPermissionsGranted()) {
            startCamera();
        } else {
            ActivityCompat.requestPermissions(this, REQUIRED_PERMISSIONS, REQUEST_CODE_PERMISSIONS);
        }

        cameraExecutor = Executors.newSingleThreadExecutor();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_CODE_PERMISSIONS) {
            if (allPermissionsGranted()) {
                startCamera();
            }
            else {
                finish();
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        cameraExecutor.shutdown();
    }

    private boolean allPermissionsGranted() {
        boolean ret = true;
        for (String permission: REQUIRED_PERMISSIONS) {
            if (ContextCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
                ret = false;
                break;
            }
        }
        return ret;
    }

    private void startCamera() {
        ListenableFuture<ProcessCameraProvider> cameraProviderFuture = ProcessCameraProvider.getInstance(this);

        cameraProviderFuture.addListener(() -> {
            try {
                ProcessCameraProvider processCameraProvider = cameraProviderFuture.get();
                CameraSelector cameraSelector = CameraSelector.DEFAULT_BACK_CAMERA;
                ImageAnalysis imageAnalysis = new ImageAnalysis.Builder()
                    .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                    .setOutputImageFormat(ImageAnalysis.OUTPUT_IMAGE_FORMAT_RGBA_8888)
                    .setResolutionSelector(new ResolutionSelector.Builder()
                        .setResolutionStrategy(new ResolutionStrategy(new Size(1280, 960), ResolutionStrategy.FALLBACK_RULE_NONE))
                        .build()
                    )
                    .build();

                imageAnalysis.setAnalyzer(cameraExecutor, image -> {
                    int input_width = 1280;
                    int input_height = 960;
                    int roi_left_x = 0;
                    int roi_left_y = 240;
                    int roi_right_x = 640;
                    int roi_right_y = 240;
                    int roi_width = 640;
                    int roi_height = 480;
                    int rescale_width = 1024;
                    int rescale_height = 768;

                    Bitmap srcBitmap = image.toBitmap();
                    ByteBuffer srcByteBuffer = ByteBuffer.allocate(srcBitmap.getByteCount());
                    srcBitmap.copyPixelsToBuffer(srcByteBuffer);

                    Bitmap dstBitmap = Bitmap.createBitmap(rescale_width << 1, rescale_height, Bitmap.Config.ARGB_8888);

                    byte[] cropLByteArray = new byte[(roi_width * roi_height) << 2];
                    crop(srcByteBuffer.array(), cropLByteArray, input_width, input_height, roi_left_x, roi_left_y, roi_width, roi_height);

                    byte[] rescaleLByteArray = new byte[(rescale_width * rescale_height) << 2];
                    rescale(cropLByteArray, rescaleLByteArray, roi_width, roi_height, rescale_width, rescale_height);

                    byte[] cropRByteArray = new byte[(roi_width * roi_height) << 2];
                    crop(srcByteBuffer.array(), cropRByteArray, input_width, input_height, roi_right_x, roi_right_y, roi_width, roi_height);

                    byte[] rescaleRByteArray = new byte[(rescale_width * rescale_height) << 2];
                    rescale(cropRByteArray, rescaleRByteArray, roi_width, roi_height, rescale_width, rescale_height);

                    byte[] dstByteArray = new byte[dstBitmap.getByteCount()];
                    hconcate(rescaleLByteArray, rescaleRByteArray, dstByteArray, rescale_width, rescale_width, rescale_height);

                    dstBitmap.copyPixelsFromBuffer(ByteBuffer.wrap(dstByteArray));
                    image.close();

                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            viewBinding.xrView.setImageBitmap(dstBitmap);
                        }
                    });
                });

                processCameraProvider.unbindAll();
                processCameraProvider.bindToLifecycle(MainActivity.this, cameraSelector, imageAnalysis);

                Log.d(TAG, "Bind ProcessCameraProvider SUCCESS");
            } catch (Exception e) {
                Log.e(TAG, "Bind ProcessCameraProvider FAILED: " + e);
            }
        }, ContextCompat.getMainExecutor(this));
    }
}
