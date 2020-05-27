#include <cstdio>
#include "tcp_unitL.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "dtsclient.h"
#include "GatePPDClient.h"

static int rcvfunc(void* argPtr, Value & value, int32_t chnlId)
{
    switch (value.type)
    {
    case Ana_VT:
        {
        AData* dataPtr = NULL;
        value.getData(dataPtr);
        std::cout << "RECEIVE CH[" << chnlId << "] ANA[" << value.idx << "]= " << dataPtr->value << "(" << dataPtr->quality << ")" << std::endl;
        }
        break;

    case Bin_VT:
        {
            BData* dataPtr = NULL;
            value.getData(dataPtr);
            std::cout << "RECEIVE CH[" << chnlId << "] BIN[" << value.idx << "]= " << dataPtr->value << "(" << dataPtr->quality << ")" << std::endl;
        }
        break;

    case Int_VT:
    {
        IData* dataPtr = NULL;
        value.getData(dataPtr);
        std::cout << "RECEIVE CH[" << chnlId << "] INT[" << value.idx << "]= " << dataPtr->value << "(" << dataPtr->quality << ")" << std::endl;
    }
    break;

    case Grp_VT:
    {
        GData* dataPtr = NULL;
        value.getData(dataPtr);
        std::cout << "RECEIVE CH[" << chnlId << "] GRP[" << value.idx << "] (GT:" << dataPtr->groupType << " GL:" << dataPtr->size << std::endl;
    }
    break;

    default:
        break;
    }
    return 0;
}

static Value value;

int main()
{
    GatePPDClient* gatePPD = new GatePPDClient();
    gatePPD->InitClientPPD("conf.txt");

    std::cout << "SIZE ANALOG IN "<< gatePPD->SizeAnalogDataIn << std::endl;
    std::cout << "SIZE ANALOG OUT " << gatePPD->SizeAnalogDataOut << std::endl;
    std::cout << "SIZE DISCRETE IN " << gatePPD->SizeDiscreteDataIn << std::endl;
    std::cout << "SIZE DISCRETE OUT " << gatePPD->SizeDiscreteDataOut << std::endl;

    std::cout << " ---------------------- " << std::endl;

    for (int i = 0; i < gatePPD->SndAnalogData.size(); i++)
    {
        std::cout << "SndAnalog " << gatePPD->SndAnalogData[i].size <<" "<< gatePPD->SndAnalogData[i].offset <<" "<< gatePPD->SndAnalogData[i].frequency << std::endl;
    }

    for (int i = 0; i < gatePPD->SndAnalogData.size(); i++)
    {
        std::cout << "SndDiscrete " << gatePPD->SndDiscreteData[i].size << " " << gatePPD->SndDiscreteData[i].offset << " " << gatePPD->SndDiscreteData[i].frequency << std::endl;
    }


    float* fw = new float[100];
    int* iw = new int[100];
    float* fr = new float[100];
    for (int i = 0; i < 100; i++)
    {
        fw[i] = (float)i + 0.1;
        iw[i] = i;
    }
    gatePPD->WriteDataInPPD(TypeSignalPPD::Analog, fw, 0, 100);
    gatePPD->WriteDataInPPD(TypeSignalPPD::Discrete, iw, 0, 100);

    for (int i = 0; i < 100; i++)
    {
        std::cout << gatePPD->bufAnalogIn[i] << " " << gatePPD->bufDiscreteIn[i] << std::endl;
    }


    r1:
    sleep(2);
    goto r1;






    /*timeval time;

    DTSClient* client = new DTSClient(rcvfunc,NULL) ;

    if (client->getState() != 0)
    {
        std::cout << "ERROR DTS" << client->getError() << std::endl;
    }
    else
    {
        std::cout << "OK DTS" << std::endl;
    }
    char gr[100];
    int i = 0;
    int res;
    sleep(5);
    std::cout << "MAXAnaRcv - " << client->getRcvAnaNum(0) << std::endl;
    std::cout << "MAXBinRcv - " << client->getRcvBinNum(0) << std::endl;
    std::cout << "MAXIntRcv - " << client->getRcvIntNum(0) << std::endl;

    std::cout << "MAXAnaSnd - " << client->getSndAnaNum(0) << std::endl;
    std::cout << "MAXBinSnd - " << client->getSndBinNum(0) << std::endl;
    std::cout << "MAXIntSnd - " << client->getSndIntNum(0) << std::endl;

rep:

    gettimeofday(&time,NULL);
    
    res=makeGValue(&value, 13, 1, gr, 100,(uint32_t)time.tv_sec, (uint32_t)(time.tv_usec/1000));
    std::cout << "makeGValue put: " << res << "-" << client->getError() << std::endl;
    res=client->put(value);
    std::cout << "result put: "<<res << "-" <<client->getError() << std::endl;
    
    sleep(5);
    goto rep;

     /*
    /*timeval timelast;
    timeval timenow;
    float time = 0.;
    char c;

    gettimeofday(&timelast, NULL);
    sleep(2);
    usleep(15000);
    gettimeofday(&timenow, NULL);
    
    time = (timenow.tv_sec - timelast.tv_sec) * 1000.0 + (timenow.tv_usec - timelast.tv_usec) / 1000.0;
    std::cout << time << std::endl;
    std::cin >> c;
    

    pthread_mutex_t mut_out = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mut_in = PTHREAD_MUTEX_INITIALIZER;
    
    std::list<ConfigSharedMemory> memory;
    std::list<ConfigUnitGate> gaties;

    TypeSignal m = TypeSignal::Analog;

    if (read_config_file("conf.txt", &memory, &gaties) != 0)
    {
        std::cout << "ERROR READ CONFIG FILE" << std::endl;
        return -1;
    }
    

    float* out = new float[100];
    /*float* in = new float[100];
    for (int i = 0; i < 100; i++)
    {
        out[i] = i;
    }

    std::cout << "Hello World!\n";

    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        if ((&*iter)->type_unit == TypeUnitGate::SERVER)
        {
            (&*iter)->buf_data = (char*)out;
            (&*iter)->mutex_data = mut_out;
        }
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT)
        {
            (&*iter)->buf_data = (char*)out;
            (&*iter)->mutex_data = mut_out;
        }  
    }

    /*
    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        if ((&*iter)->type_unit == TypeUnitGate::SERVER) std::cout << (int*)(&*iter)->buf_data << "  ---  " << out << std::endl;
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT) std::cout << (int*)(&*iter)->buf_data << "  ---  " << in << std::endl;

        std::cout << (&*iter)->type_unit << " " << (&*iter)->Port << " " << (&*iter)->IP << std::endl;
    }


    std::list<tcp_unit*> units_gate;
    int id = 0;
    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        units_gate.push_back(tcp_unit::create_tcp_unit(*iter, id));
        id++;
    }*/

next:
    sleep(2);

    /*pthread_mutex_lock(&mut_out);
    for (int i = 0; i < 100; i++)
    {
        out[i]++;
    }
    pthread_mutex_unlock(&mut_out);

    pthread_mutex_lock(&mut_in);
    std::cout << in[0] << std::endl;
    std::cout << in[99] << std::endl;
    pthread_mutex_unlock(&mut_in);
    */
    goto next;
    return 0;
}