#include "GatePPDClient.h"
#include <iostream>


int GatePPDClient::read_config_file(const char* Namefile)
{
    FILE* config_file = NULL;
    char simvol = 0;
    std::string str_info;
    std::string helpstr;
    int res_read = 0;
    int pos[2] = { 0,0 };
    FrequencySndData infoSndData;

    config_file = fopen(Namefile, "r");
    if (config_file == NULL)
    {
        fclose(config_file);
        return -1;
    }
        

    for (;;)
    {
        simvol = 0;
        str_info.clear();
        while (simvol != '\n' && res_read != EOF)
        {
            res_read = fscanf(config_file, "%c", &simvol);
            if ((simvol > 0x1F || simvol == '\t') && res_read != EOF) str_info += simvol;
        }

        if (str_info == "[List]" || res_read == EOF)
        {
            if (res_read == EOF)
            {
                std::cout << "MAIT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                fclose(config_file);
                return -1;
            }
            break;
        }

        if (str_info.substr(0, 5) == "@GATE")
        {
            pos[0] = 0;
            pos[1] = 0;
            infoSndData.clear();
                    

            pos[0] = str_info.find('\t', 0);
            if (pos[0] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                fclose(config_file);
                return -1;
            }
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                fclose(config_file);
                return -1;
            }

            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            infoSndData.offset = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                fclose(config_file);
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            infoSndData.size = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                fclose(config_file);
                return -1;
            }

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                fclose(config_file);
                return -1;
            }

            pos[0] = pos[1];
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1);
            infoSndData.frequency = atoi(helpstr.c_str());

            if (str_info.find("Analog") != -1)
            {
                if (str_info.find("Input") != -1)
                {
                    SndAnalogData.push_back(infoSndData);
                    if ((infoSndData.size + infoSndData.offset) > SizeAnalogDataIn) SizeAnalogDataIn = infoSndData.size + infoSndData.offset;
                }
                else if (str_info.find("Output") != -1)
                {
                    if ((infoSndData.size + infoSndData.offset) > SizeAnalogDataOut) SizeAnalogDataOut = infoSndData.size + infoSndData.offset;
                }
            }
            else if (str_info.find("Discrete") != -1)
            {
                if (str_info.find("Input") != -1)
                {
                    SndDiscreteData.push_back(infoSndData);
                    if ((infoSndData.size + infoSndData.offset) > SizeDiscreteDataIn) SizeDiscreteDataIn = infoSndData.size + infoSndData.offset;
                }
                else if (str_info.find("Output") != -1)
                {
                    if ((infoSndData.size + infoSndData.offset) > SizeDiscreteDataOut) SizeDiscreteDataOut = infoSndData.size + infoSndData.offset;
                }
            }

        }
        else if (str_info.substr(0, 5) != "[EMT]" && str_info.substr(0, 6) != "[GATE]" && str_info.substr(0, 4) != "@EMT")
        {
            std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
            fclose(config_file);
            return -1;
        }
    }
    fclose(config_file);
    return 0;
}

int GatePPDClient::InitClientPPD(const char* filename)
{
    int result = 0;
    if (read_config_file(filename) == -1)
    {
        std::cout << "ERROR INIT PPClient: BAD CONFIG FILE" << std::endl;
        return -1;
    }

    result = client->getRcvAnaNum();
    if (result == 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvAnaNum CODE ERROR " << client->getError() << std::endl;
        return -1;
    }
    if (result < SizeAnalogDataOut)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS ANALOG-OUT" << client->getError() << std::endl;
        return -1;
    }
    bufAnalogOut = new float[result];
    memset(bufAnalogOut, 0, sizeof(bufAnalogOut));

    result = client->getRcvIntNum();
    if (result == 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvIntNum CODE ERROR " << client->getError() << std::endl;
        return -1;
    }
    if (result < SizeDiscreteDataOut)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS DISCRETE-OUT" << client->getError() << std::endl;
        return -1;
    }
    bufDiscreteOut = new int[result];
    memset(bufDiscreteOut, 0, sizeof(bufDiscreteOut));

    result = client->getSndAnaNum();
    if (result == 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvAnaNum CODE ERROR " << client->getError() << std::endl;
        return -1;
    }
    if (result < SizeAnalogDataIn)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS ANALOG-IN" << client->getError() << std::endl;
        return -1;
    }
    bufAnalogIn = new float[result];
    memset(bufAnalogIn, 0, sizeof(bufAnalogIn));

    result = client->getSndIntNum();
    if (result == 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvIntNum CODE ERROR " << client->getError() << std::endl;
        return -1;
    }
    if (result < SizeDiscreteDataIn)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS DISCRETE-IN" << client->getError() << std::endl;
        return -1;
    }
    bufDiscreteIn = new int[result];
    memset(bufDiscreteIn, 0, sizeof(bufDiscreteIn));

    statusInitClient = 1;
    threadWriteServerDTS = std::thread(&GatePPDClient::FuncWriteServerDTS,this);

    return 0;    
}


static int FuncRcvPPD(void* argPtr, Value& value, int32_t chnlId)
{
    GatePPDClient* gate = (GatePPDClient*)argPtr;

    float* ibuff;
    int* ibufi;

    if (gate->channel != chnlId) return 1;
    
    switch (value.type)
    {
    case Ana_VT:
    {
        AData* dataPtr = NULL;
        value.getData(dataPtr);

        if (gate->statusInitClient != 1) break;
        if (value.idx > gate->SizeAnalogDataOut)
        {
            std::cout << "ERROR BAD ANALOG VALUE" << std::endl;
            return -1;
        }
        ibuff = gate->bufAnalogOut;
        ibuff += value.idx;
        //pthread_mutex_lock(&gate->mutex_analog_out);
        *ibuff = (dataPtr->value);
        //pthread_mutex_unlock(&gate->mutex_analog_out);
    }
    break;

    case Bin_VT:
    {
        BData* dataPtr = NULL;
        value.getData(dataPtr);

        if (gate->statusInitClient != 1) break;
        std::cout << "RECEIVE CH[" << chnlId << "] BIN[" << value.idx << "]= " << dataPtr->value << "(" << dataPtr->quality << ")" << std::endl;
    }
    break;

    case Int_VT:
    {
        IData* dataPtr = NULL;
        value.getData(dataPtr);
        
        if (gate->statusInitClient != 1) break;
        if (value.idx > gate->SizeDiscreteDataOut)
        {
            std::cout << "ERROR BAD ANALOG VALUE" << std::endl;
            return -1;
        }
        ibufi = gate->bufDiscreteOut;
        ibufi += value.idx;
        pthread_mutex_lock(&gate->mutex_discrete_out);
        *ibufi = (dataPtr->value);
        pthread_mutex_unlock(&gate->mutex_discrete_out);
    
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

int GatePPDClient::sendvalana(int indx, float val)
{
    Value value;
    makeAValue(&value, indx, 1, val);
    client->put(value);
}



GatePPDClient::GatePPDClient()
{
	client = new DTSClient(FuncRcvPPD, this);
	if (client->getState() == 0)
	{
		std::cout << "GATE CONNECT WITH DTS" << std::endl;
	}
	else
	{
		std::cout << "ERROR CONNECTION WITH DTS CODE ERROR:"<< client->getError() << std::endl;
	}
}

InfobufPPD GatePPDClient::TakeInfoForReadPDD(TypeSignalPPD type_signal)
{
    InfobufPPD infobuf;
    
    if (type_signal == TypeSignalPPD::Analog)
    {
        infobuf.buf = bufAnalogOut;
        infobuf.mutex = mutex_analog_out;
    }
    else if (type_signal == TypeSignalPPD::Discrete)
    {
        infobuf.buf = bufDiscreteOut;
        infobuf.mutex = mutex_discrete_out;
    }
    
    return infobuf;
}

InfobufPPD GatePPDClient::TakeInfoForWritePDD(TypeSignalPPD type_signal)
{
    InfobufPPD infobuf;
    infobuf.buf = 0;

    if (type_signal == TypeSignalPPD::Analog)
    {
        infobuf.buf = bufAnalogIn;
        infobuf.mutex = mutex_analog_in;
    }
    else if (type_signal == TypeSignalPPD::Discrete)
    {
        infobuf.buf = bufDiscreteIn;
        infobuf.mutex = mutex_discrete_in;
    }

    return infobuf;
}

int GatePPDClient::ReadDataFromPPD(TypeSignalPPD type_signal, void* buf, int offset, int size)
{
    if (type_signal == TypeSignalPPD::Analog)
    {
        if (offset + size > SizeAnalogDataOut)
        {
            std::cout << "ReadDataFromPPD ERROR: BAD DATA" << std::endl;
            return -1;
        }
        float* ibuf=(float*)buf;
        float* jbuf = bufAnalogOut;
        jbuf += offset;
        //pthread_mutex_lock(&mutex_analog_out);
        for (int i = 0; i < size; i++)
        {
            *ibuf = *jbuf;
            ibuf++;
            jbuf++;
        }
        //pthread_mutex_unlock(&mutex_analog_out);

    }
    else if (type_signal == TypeSignalPPD::Discrete)
    {
        if (offset + size > SizeDiscreteDataOut)
        {
            std::cout << "ReadDataFromPPD ERROR: BAD DATA" << std::endl;
            return -1;
        }
        int* ibuf = (int*)buf;
        int* jbuf = bufDiscreteOut + offset;
        pthread_mutex_lock(&mutex_discrete_out);
        for (int i = 0; i < size; i++)
        {
            *ibuf = *jbuf;
            ibuf++;
            jbuf++;
        }
        pthread_mutex_unlock(&mutex_discrete_out);
    }
    return 0;    
}

int GatePPDClient::WriteDataInPPD(TypeSignalPPD type_signal, void* buf, int offset, int size)
{
    if (type_signal == TypeSignalPPD::Analog)
    {
        if (offset + size > SizeAnalogDataOut)
        {
            std::cout << "ReadDataFromPPD ERROR: BAD DATA" << std::endl;
            return -1;
        }
        float* ibuf = (float*)buf;
        float* jbuf = bufAnalogIn + offset;
        //pthread_mutex_lock(&mutex_analog_in);
        for (int i = 0; i < size; i++)
        {
            *jbuf = *ibuf;
            ibuf++;
            jbuf++;
        }
        //pthread_mutex_unlock(&mutex_analog_in);
    }
    else if (type_signal == TypeSignalPPD::Discrete)
    {
        if (offset + size > SizeAnalogDataOut)
        {
            std::cout << "ReadDataFromPPD ERROR: BAD DATA" << std::endl;
            return -1;
        }
        int* ibuf = (int*)buf;
        int* jbuf = bufDiscreteIn + offset;
        pthread_mutex_lock(&mutex_discrete_in);
        for (int i = 0; i < size; i++)
        {
            *jbuf = *ibuf;
            ibuf++;
            jbuf++;
        }
        pthread_mutex_unlock(&mutex_discrete_in);
    }
    return 0;
};

int GatePPDClient::FuncWriteServerDTS()
{
    timeval timenow;
    float time;
    float* ibuff;
    int* ibufi;
    Value value;
    int result;
    float f;

    for (int i = 0; i < SndAnalogData.size(); i++)
    {
        gettimeofday(&SndAnalogData[i].TimeLastSnd, NULL);
    }
    for (int i = 0; i < SndDiscreteData.size(); i++)
    {
        gettimeofday(&SndDiscreteData[i].TimeLastSnd, NULL);
    }

    for (;;)
    {
        gettimeofday(&timenow, NULL);

        for (int i = 0; i < SndAnalogData.size(); i++)
        {
            time = (timenow.tv_sec * 1000.+ timenow.tv_usec / 1000.)-(SndAnalogData[i].TimeLastSnd.tv_sec * 1000. + SndAnalogData[i].TimeLastSnd.tv_usec / 1000.);

            if (SndAnalogData[i].frequency < time)
                std::cout << "DTSCLEINT ERROR SEND ANALOG DATA TIME: " << time << std::endl;

            if (SndAnalogData[i].frequency - TIMEDEV < time)
            {
             
                ibuff = bufAnalogIn;
                //ibuff += SndAnalogData[i].offset;
                //pthread_mutex_lock(&mutex_analog_in);
                for (int j = 0; j < SndAnalogData[i].size; j++)
                {
                    f = *ibuff;
                    makeAValue(&value, j, 1, f);
                    //if (j == 0) std::cout << "index 0 snd " << *ibuff << std::endl;
                    result = client->put(&value,0);
                    if (result < 0) std::cout << "DTSCLEINT ERROR SEND ANALOG DATA ERROR"<< client->getError() << time;
                    ibuff++;
                    gettimeofday(&SndAnalogData[i].TimeLastSnd, NULL);
                } 
                //pthread_mutex_unlock(&mutex_analog_in);
            }
        }

        for (int i = 0; i < SndDiscreteData.size(); i++)
        {
            time = (timenow.tv_sec * 1000.+timenow.tv_usec / 1000.)-(SndDiscreteData[i].TimeLastSnd.tv_sec * 1000. +SndDiscreteData[i].TimeLastSnd.tv_usec / 1000.);

            if (SndDiscreteData[i].frequency < time)
                std::cout << "DTSCLEINT ERROR SEND DISCRETE DATA TIME: " << time << std::endl;

            if (SndDiscreteData[i].frequency - TIMEDEV < time)
            {

                ibufi = bufDiscreteIn;
                ibufi += SndDiscreteData[i].offset;
                pthread_mutex_lock(&mutex_discrete_in);
                for (int j = 0; j < SndDiscreteData[i].size; j++)
                {
                    makeIValue(&value, j, 1, *ibufi);
                    result = client->put(&value, 0);
                    //std::cout << result<<" "<< client->getError() << time << std::endl;
                    if (result < 0) std::cout << "DTSCLEINT ERROR SEND DISCRETE DATA ERROR" << client->getError() << time <<std::endl;
                    ibufi++;
                    gettimeofday(&SndDiscreteData[i].TimeLastSnd, NULL);
                }
                pthread_mutex_unlock(&mutex_discrete_in);
            }
        }
    }
}



GatePPDClient::~GatePPDClient()
{
    delete(client);
    delete(bufAnalogOut);
    delete(bufAnalogIn);
    delete(bufDiscreteOut);
    delete(bufDiscreteIn);
    delete(bufGroupOut);
    delete(bufGroupgIn);
}