#include "fincalStructs.h"

enum CLASSIFIER_TYPE
  {
    E_KERNEL_KMEANS
    //todo ,OTHERS
  };

bool process_data(const std::vector<const ticks<time_t, double>* >& finData, std::vector<std::map<std::string, double> >& results, const enum CLASSIFIER_TYPE);
