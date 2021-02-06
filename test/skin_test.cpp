#include "export/export.h"
#include "utils.h"
#include <opencv2/highgui.hpp>


int main(int arv, char** arg)
{
    if(arv<2)
    {
        printf("use: ./Test_app filepath\n");
        return -1;
    }

    int iSum = 0;
    char filepath[150];
    memset(filepath, 0, sizeof(filepath));
    sprintf(filepath, arg[1]);
    std::vector<std::string> filename_vec;
    get_dir(filepath, filename_vec);

    for(int i=0;i<filename_vec.size();++i)
    {
        std::string temppath = filename_vec[i];
        std::string filename = temppath.substr(temppath.find_last_of('/')+1);
        std::string fileinname = filename.substr(0, filename. find_first_of("."));
        cv::Mat Img = cv::imread(temppath);
        if(Img.cols != 1920 || Img.rows != 1080)
        {
            continue;
        }
        double time_start = getCurrentTime();
        int ibrightness = 0;
        int iret = skinWhite(temppath.c_str(), filename.c_str());
        if(iret != 0)
        {
            printf("beauty_img error code is %d\n", (int)iret);
            continue;
        }
        double time_process = getCurrentTime() - time_start;
        printf("image process time: %f\n", time_process);
        iSum += ibrightness;

        memset(filepath, 0, sizeof(filepath));
        sprintf(filepath, "./%s_%d_b%d_out.jpg",fileinname.c_str(), i, ibrightness);
        cv::imwrite(filepath, Img);
    }
    printf("finished!\n");
}
