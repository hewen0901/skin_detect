#ifndef SKIN_DETECT_UTILS_H_
#define SKIN_DETECT_UTILS_H_

#include <string>
#include <vector>

int get_dir(const char *path,std::vector<std::string> &filename_vec);
bool compare(int a, int b);
double getCurrentTime();


#endif
