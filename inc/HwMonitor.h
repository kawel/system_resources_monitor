/**
* @file: HwMonitor.h
* @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
* @brief: 
* -----
* Copyright 2024 - KElectronics
* -----
* HISTORY:
* Date      	By	Comments
* ----------	---	---------------------------------------------------------
*/

class HwMonitor
{
private:
    /* data */
    int up_time;
public:
    HwMonitor(/* args */);
    ~HwMonitor();
    int getUpTime();
    int getTemp();
    int getRamUsage();
    int getCpuUsage();
    int getDiskUsage();
    int getNetUsage();
    int getNetSpeed();
     
};

class UpTimeInfo
{
  private:
    /* data */
    double _uptime;
  public:
    int update();
    double get() {return _uptime;}
    std::string serialize();
    UpTimeInfo(): _uptime(0.0) {};
    ~UpTimeInfo();
};


class Cpu
{
private:
    /* data */
    std::string cpuinfo;
public:

};

