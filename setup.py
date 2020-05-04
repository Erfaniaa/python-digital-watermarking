from distutils.core import setup, Extension

watermarker = Extension('digitalwatermarking',
                        sources=['digital-watermarking/binding.cpp',
                                 'digital-watermarking/watermarker.cpp'],
                        libraries=['opencv_core', 'opencv_highgui', 'opencv_imgproc', 'opencv_imgcodecs'])

setup(name='DigitalWatermarking',
      version='1.0',
      description='Python wrapper for digital watermarking using OpenCV',
      ext_modules=[watermarker])
