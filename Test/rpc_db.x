#define OK 0
#define ERROR 1

struct SensorData
{
    int dataId;
    int noValues;
    float *value;
};

program RPC_DB
{
    version RPC_DB_VERS
    {
        int ADD(struct SensorData) = 1;
        int DELETE(int dataId) = 2;
        int UPDATE(struct SensorData) = 3;
        int READ(int dataId) = 4;
    } = 1;
} = 1;