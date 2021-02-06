## skin_detect_SDK

- 功能介绍：

  1. 美颜图像；
  2. 判断输入图像是否为灰度图；

- 使用说明：

  1. 输入图像只支持8位三通道的RGB 1080p图像，其他图像需要转换；

  2. ```sh
     cd baby_skin_detect
     mkdir build
     cd build
     cmake ..
     make -j4
     ./Test_app filepath
     ```

- 适用平台：pc

- 编写语言：c++ 

  20210106 -----by nero
