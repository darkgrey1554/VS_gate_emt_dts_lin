#include <cstdio>
#include "tcp_unitL.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "dtsclient.h"
#include "GatePPDClient.h"


int main()
{

    GatePPDClient* gatePPD = new GatePPDClient();
    timeval timenow;
    timeval timelast;
    float time = 0;

    sleep(5);
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

    //for (int i = 0; i < gatePPD->SndAnalogData.size(); i++)
    //{
    //    std::cout << "SndDiscrete " << gatePPD->SndDiscreteData[i].size << " " << gatePPD->SndDiscreteData[i].offset << " " << gatePPD->SndDiscreteData[i].frequency << std::endl;
    //}


    float* fw = new float[100];
    float* fr = new float[100];
    int* iw = new int[100];   
    int* ir = new int[100];

    int z = 1;
    for (int i = 0; i < 100; i++)
    {
        fw[i]=i+1;
        iw[i] = 0;
    }

    gettimeofday(&timenow,NULL);
    gettimeofday(&timelast,NULL);
r1:

    do
    {
        gettimeofday(&timenow,NULL);
        time = (timenow.tv_sec - timelast.tv_sec) * 1000. + (timenow.tv_usec - timelast.tv_usec) / 1000.;
        usleep(10);
    } while (time < 80);

    for (int i = 0; i < 100; i++)
    {
        fw[i]+= 0.001;
        iw[i] = z;
    }

    gatePPD->WriteDataInPPD(TypeSignalPPD::Analog, fw, 0, 100);
    //gatePPD->WriteDataInPPD(TypeSignalPPD::Discrete, iw, 0, 100);

    gatePPD->ReadDataFromPPD(TypeSignalPPD::Analog, fr, 0, 100);    
    //gatePPD->ReadDataFromPPD(TypeSignalPPD::Discrete, ir, 0, 100);

    /*for (int i = 0; i < 100; i++)
    {
        std::cout << "fw: " << *(fw + i) << " " << "fr: " << *(fr + i) << " " << std::endl;
    }*/

    gettimeofday(&timelast, NULL);
    std::cout << "fw: " << *(fw) << " " << "fr: " << *(fr) << " " << std::endl;
    z++;
    goto r1;
;
    

    /*pthread_mutex_t mut_out = PTHREAD_MUTEX_INITIALIZER;
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
    float* in = new float[100];
    for (int i = 0; i < 100; i++)
    {
        out[i] = 0;
    }

    std::cout << "Hello World!\n";

    /*InfobufPPD ser;
    InfobufPPD cl;

    ser = gatePPD->TakeInfoForReadPDD(TypeSignalPPD::Analog);
    cl = gatePPD->TakeInfoForWritePDD(TypeSignalPPD::Analog);

    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        if ((&*iter)->type_unit == TypeUnitGate::SERVER)
        {
            (&*iter)->buf_data = (char*)ser.buf;
            (&*iter)->mutex_data = ser.mutex;
        }
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT)
        {
            (&*iter)->buf_data = (char*)cl.buf;
            (&*iter)->mutex_data = cl.mutex;
        }
    }*/

    /*for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
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
    }*/

    
    /*for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        if ((&*iter)->type_unit == TypeUnitGate::SERVER) std::cout << (int*)(&*iter)->buf_data << "  ---  " << out << std::endl;
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT) std::cout << (int*)(&*iter)->buf_data << "  ---  " << in << std::endl;

        std::cout << (&*iter)->type_unit << " " << (&*iter)->Port << " " << (&*iter)->IP << std::endl;
    }*/


    /*std::list<tcp_unit*> units_gate;
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