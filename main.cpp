#include <cstdio>
#include <stdio.h>
#include <linux/input.h>
#include "tcp_unitL.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "dtsclient.h"
#include "GatePPDClient.h"
#include <termios.h>
#include <cstdlib>

int getch()
{
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

pthread_mutex_t mut = PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP;
int sizemass = 300;
int frtime = 340;
float* fmassout = new float[5000];
float* fmassin = new float[5000];
int* imassout = new int[5000];
int* imassin = new int[5000];

/*static int FuncRcv(void* argPtr, Value& value, int32_t chnlId)
{

    float* ibuff = fmassout;
    float* jbuff;
    int* ibufi = imassout;
    int* jbufi;
    uint8_t* buf;
    switch (value.type)
    {
    case Ana_VT:
    {
        AData* dataPtr = NULL;
        value.getData(dataPtr);
        ibuff += value.idx;
        *ibuff = (dataPtr->value);
    }
    break;

    case Bin_VT:
    {
        BData* dataPtr = NULL;
        value.getData(dataPtr);
        //std::cout << "RECEIVE CH[" << chnlId << "] BIN[" << value.idx << "]= " << dataPtr->value << "(" << dataPtr->quality << ")" << std::endl;
    }
    break;

    case Int_VT:
    {
        IData* dataPtr = NULL;
        value.getData(dataPtr);

    }
    break;

    case Grp_VT:
    {
        GData* dataPtr = NULL;
        value.getData(dataPtr);
        std::cout << "RECEIVE CH[" << chnlId << "] GRP[" << value.idx << "] (GT:" << dataPtr->groupType << " GL:" << dataPtr->size << std::endl;
        /*if (dataPtr->groupType == 1)
        {
            jbuff = (float*)dataPtr->value;
            for (int i = 0; i < dataPtr->size; i++)
            {
                *ibuff = *jbuff;
                ibuff++;
                jbuff++;
            }
        }
        /*if (dataPtr->groupType != 0)
        {
            //pthread_mutex_lock(&mut);
            ibufi = imassout;
           // std::cout << dataPtr->groupType << " " << dataPtr->size << std::endl;
            ibufi+=sizemass*(dataPtr->groupType-1);
            buf = dataPtr->value;
            for (int i = 0; i < dataPtr->size; i++)
            {
                *ibufi = *(int*)buf;
                ibufi++;
                buf+=4;
            }

            //std::cout << value.idx << " " << *ibufi << " " << *jbufi << std::endl;
            //pthread_mutex_unlock(&mut);
        }*/
        /*}
        break;

        default:
            break;
        }

        return 0;
    }*/

int main()
{

    system("clear");
    std::cout << "------ NODE DTS CLIENT -------" << std::endl;
    std::cout << "------- PS: GATE DTS ---------" << std::endl;
    std::cout << "PRESS ANY KEY TO START WORK ...." << std::endl;
    char s = 0;
    s = getch();


    /*struct input_event ie;
    ssize_t res;
    int fkeyboard;
    fkeyboard = open("/dev/input/event4",O_RDONLY);
    std::cout << fkeyboard << std::endl;
    sleep(1);
    while (1)
    {
        res = read(fkeyboard, &ie, sizeof(ie));
        std::cout << res << std::endl;
        usleep(1000000);
    }*/


    /*char t;
    DTSClient cl(FuncRcv, NULL);

    if (cl.getState() == 0)
    {
        std::cout << "DTSClient connect OK" << std::endl;
    }
    else
    {
          std::cout << "DTSClient connect OK "<< cl.getError() << std::endl;
          std::cin >> t;
    }

    for (int i = 0; i < sizemass*3; i++)
    {
        fmassin[i] = i;
        imassin[i] = i;
    }

    Value val;
    timeval timenow;
    timeval timelast;
    float time = 0;
    int res;
    gettimeofday(&timelast, NULL);
    gettimeofday(&timenow, NULL);

    for (;;)
    {
        do
        {
            gettimeofday(&timenow, NULL);
            time = (timenow.tv_sec - timelast.tv_sec) * 1000. + (timenow.tv_usec - timelast.tv_usec) / 1000.;
            usleep(10);
        } while (time < frtime);
        gettimeofday(&timelast, NULL);

        for (int i = 0; i < sizemass; i++)
        {
            fmassin[i] += 0.01;
            imassin[i]++;

        }

        for (int i = 0; i < sizemass; i++)
        {
            makeAValue(&val,i,1,fmassin[i]);
            cl.put(val, 0);
        }

        //if (res == 1) std::cout << res << " "<< cl.getError() <<std::endl;
        pthread_mutex_lock(&mut);
        std::cout << "in: " << *(fmassin + sizemass - 1) << " out: " << *(fmassout + sizemass - 1) << std::endl;
        pthread_mutex_unlock(&mut);
    }*/




    GatePPDClient* gatePPD = new GatePPDClient();
    timeval timenow;
    timeval timelast;
    float time = 0;

    sleep(5);
    gatePPD->InitClientPPD("conf.txt");

    //std::cout << "SIZE ANALOG IN "<< gatePPD->SizeAnalogDataIn << std::endl;
    //std::cout << "SIZE ANALOG OUT " << gatePPD->SizeAnalogDataOut << std::endl;
    //std::cout << "SIZE DISCRETE IN " << gatePPD->SizeDiscreteDataIn << std::endl;
    //std::cout << "SIZE DISCRETE OUT " << gatePPD->SizeDiscreteDataOut << std::endl;

    //std::cout << " ---------------------- " << std::endl;

    /*for (int i = 0; i < gatePPD->SndAnalogData.size(); i++)
    {
        std::cout << "SndAnalog " << gatePPD->SndAnalogData[i].size <<" "<< gatePPD->SndAnalogData[i].offset <<" "<< gatePPD->SndAnalogData[i].frequency << std::endl;
    }*/

    //for (int i = 0; i < gatePPD->SndAnalogData.size(); i++)
    //{
    //    std::cout << "SndDiscrete " << gatePPD->SndDiscreteData[i].size << " " << gatePPD->SndDiscreteData[i].offset << " " << gatePPD->SndDiscreteData[i].frequency << std::endl;
    //}

    /*int sizemass = 100;
    int frtime = 340;
    float* fw = new float[sizemass];
    float* fr = new float[sizemass];
    int* iw = new int[sizemass];
    int* ir = new int[sizemass];

    int z = 1;
    for (int i = 0; i < sizemass; i++)
    {
        fw[i]=i+1;
        iw[i] = 0;
    }*/

    //gettimeofday(&timenow,NULL);
    //gettimeofday(&timelast,NULL);
/*r1:

    do
    {
        gettimeofday(&timenow,NULL);
        time = (timenow.tv_sec - timelast.tv_sec) * 1000. + (timenow.tv_usec - timelast.tv_usec) / 1000.;
        usleep(10);
    } while (time < frtime);

    for (int i = 0; i < sizemass; i++)
    {
        fw[i]+= 0.001;
        iw[i] = z;
    }

    gatePPD->WriteDataInPPD(TypeSignalPPD::Analog, fw, 0, sizemass);
    //gatePPD->WriteDataInPPD(TypeSignalPPD::Discrete, iw, 0, 100);

    gatePPD->ReadDataFromPPD(TypeSignalPPD::Analog, fr, 0, sizemass);
    //gatePPD->ReadDataFromPPD(TypeSignalPPD::Discrete, ir, 0, 100);

    /*for (int i = 0; i < 100; i++)
    {
        std::cout << "fw: " << *(fw + i) << " " << "fr: " << *(fr + i) << " " << std::endl;
    }*/

    /*gettimeofday(&timelast, NULL);
    std::cout << "fw: " << *(fw) << " " << "fr: " << *(fr) << " " << std::endl;
    z++;
    goto r1;*/
    ;


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
    float* in = new float[100];
    for (int i = 0; i < 100; i++)
    {
        out[i] = 0;
    }

    //std::cout << "Hello World!\n";

    InfobufPPD sera;
    InfobufPPD cla;
    InfobufPPD serd;
    InfobufPPD cld;
    InfobufPPD serb;
    InfobufPPD clb;

    sera = gatePPD->TakeInfoForReadPDD(TypeSignalPPD::Analog);
    cla = gatePPD->TakeInfoForWritePDD(TypeSignalPPD::Analog);
    serd = gatePPD->TakeInfoForReadPDD(TypeSignalPPD::Discrete);
    cld = gatePPD->TakeInfoForWritePDD(TypeSignalPPD::Discrete);
    serb = gatePPD->TakeInfoForReadPDD(TypeSignalPPD::Binar);
    clb = gatePPD->TakeInfoForWritePDD(TypeSignalPPD::Binar);

    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        if ((&*iter)->type_unit == TypeUnitGate::SERVER && (&*iter)->type_signal == TypeSignal::Analog)
        {
            (&*iter)->buf_data = (char*)sera.buf;
            (&*iter)->mutex_data = sera.mutex;
        }
        if ((&*iter)->type_unit == TypeUnitGate::SERVER && (&*iter)->type_signal == TypeSignal::Discrete)
        {
            (&*iter)->buf_data = (char*)serd.buf;
            (&*iter)->mutex_data = serd.mutex;
        }
        if ((&*iter)->type_unit == TypeUnitGate::SERVER && (&*iter)->type_signal == TypeSignal::Binar)
        {
            (&*iter)->buf_data = (char*)serb.buf;
            (&*iter)->mutex_data = serb.mutex;
        }
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT && (&*iter)->type_signal == TypeSignal::Analog)
        {
            (&*iter)->buf_data = (char*)cla.buf;
            (&*iter)->mutex_data = cla.mutex;
        }
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT && (&*iter)->type_signal == TypeSignal::Discrete)
        {
            (&*iter)->buf_data = (char*)cld.buf;
            (&*iter)->mutex_data = cld.mutex;
        }
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT && (&*iter)->type_signal == TypeSignal::Binar)
        {
            (&*iter)->buf_data = (char*)clb.buf;
            (&*iter)->mutex_data = clb.mutex;
        }

    }

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


    std::list<tcp_unit*> units_gate;
    int id = 0;
    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        units_gate.push_back(tcp_unit::create_tcp_unit(*iter, id));
        id++;
    }

    /*next:
        sleep(2);

       /* pthread_mutex_lock(&mut_out);
        for (int i = 0; i < 100; i++)
        {
            out[i]++;
        }
        pthread_mutex_unlock(&mut_out);*/

        /*pthread_mutex_lock(&mut_out);
        std::cout << out[0] << std::endl;
        std::cout <<out[99] << std::endl;
        pthread_mutex_unlock(&mut_out);*/

        //goto next;
    s = 0;
    while (s != 0x1B)
    {
        s = (char)getch();
    }

    return 0;
}