#define OK 0
#define ERROR 1

struct SensorData
{
    int dataId;
    int noValues;
    float value<>;
};

struct UserPackage
{
    struct SensorData data;
    string name<>;
};

struct SpecificId
{
    int dataId;
    string name<>;
};

program RPC_DB
{
    version RPC_DB_VERS
    {
        int ADD(struct UserPackage) = 1;
        int DELETE(struct SpecificId) = 2;
        int UPDATE(struct UserPackage) = 3;
        string READ(struct SpecificId) = 4;
        string GETSTAT(struct SpecificId) = 5;
        string GETSTATALL(string) = 6;
        int LOAD(string) = 7;
        string STORE(string) = 8;
        int LOGIN(string) = 9;
        int LOGOUT(string) = 10;
        string READALL(string) = 11;
    } = 1;
} = 1;