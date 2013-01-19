#ifndef LOGGER_H_
# define LOGGER_H_

Public class logManager
{
 private:
  logManager(){}

 public:
  const logManager* createInstance(){
    if(instance_ == nullptr)
      instance_ = std::unique_ptr<logManager>(new logManager());
    return instance_;
  }
  const logManager* getInstance() = createInstance;
  
  
private:
  std::unique_ptr<logManager> instance_;
};

#endif /*! LOGGER_H_*/
