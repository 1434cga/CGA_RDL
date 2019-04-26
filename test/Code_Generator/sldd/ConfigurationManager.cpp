#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <ConfigurationManagerService/IConfigurationManagerService.h>
#include <ConfigurationManagerService/IConfigurationManagerServiceType.h>
#include <services/ConfigurationManagerService/IConfigurationUpdaterService.h>
#include <services/ConfigurationManagerService/ConfigurationBuffer.h>
#include <services/ConfigurationManagerService/CountryDBBuffer.h>
#include <PowerManagerService/IPowerManagerService.h>
#include <PowerManagerService/IPowerManagerServiceType.h>

#include "man.h"
#include "sldd_common.h"
#include "ConfigurationManager.h"

#define LOG_TAG "sldd_config"

using namespace configmgr;
using namespace android;

enum Commands {
    CMD_GET_DATA=1,
    CMD_SET_DATA,
    CMD_LOCK_DATA,
    CMD_UNLOCK_DATA,
    CMD_COUNTRYDB_GET,
    CMD_COUNTRYDB_SET,
    CMD_COUNTRYDB_SAVE,
    CMD_FACTORY_SAVE,
    CMD_FACTORY_RESET,
    CMD_UPDATE_FILE,
    CMD_RESTORE_IMPORTANT,
    CMD_DELETE_ALL_DATA,
    CMD_UPDATE_LCF_FILE,
    CMD_TELE2_CONTROL,
    CMD_READ_FACTORY_VALUE,
    CMD_FACTORY_LCF_UPDATE,
    CMD_AUTO_FACTORY_UPDATE,
    CMD_DELETE_ONE_VALUE,
};

sldd_cmd_table_t config_cmds[] = {
    {"getData", CMD_GET_DATA, commandActionConfiguration,
     "  sldd configuration getData      -Get Data from Configuration Mgr \n"},
    {"setData", CMD_SET_DATA, commandActionConfiguration,
     "  sldd configuration setData      -Set Data to Configuration Mgr \n"},
    {"lockData", CMD_LOCK_DATA, commandActionConfiguration,
     "  sldd configuration lockData     -Lock Data to Configuration Mgr \n"},
    {"unlockData", CMD_UNLOCK_DATA, commandActionConfiguration,
     "  sldd configuration unlockData    -Unlock Data to Configuration Mgr \n"},
    {"getCountryData", CMD_COUNTRYDB_GET, commandActionConfiguration,
     "  sldd configuration getCountryData   -get Country Data from Configuration Mgr \n"},
    {"setCountryData", CMD_COUNTRYDB_SET, commandActionConfiguration,
     "  sldd configuration setCountryData   -set Country Data to Configuration Mgr \n"},
    {"saveCountryData", CMD_COUNTRYDB_SAVE, commandActionConfiguration,
     "  sldd configuration saveCountryData   -save Country Data to Configuration Mgr \n"},
    {"factorySave", CMD_FACTORY_SAVE, commandActionConfiguration,
     "  sldd configuration factorySave       -save Factory Data to Configuration Mgr \n"},
    {"factoryReset", CMD_FACTORY_RESET, commandActionConfiguration,
     "  sldd configuration factoryReset       -Reset Factory Data to Configuration Mgr \n"},
    {"updateFile", CMD_UPDATE_FILE, commandActionConfiguration,
     "  sldd configuration updateFile         -Update File to Configuration Mgr \n"},
    {"restoreCoreData", CMD_RESTORE_IMPORTANT, commandActionConfiguration,
     "  sldd configuration restoreCoreData       -Restore Core Data of Configuration Mgr \n"},
    {"deleteAllData", CMD_DELETE_ALL_DATA, commandActionConfiguration,
     "  sldd configuration deleteAllData        - Delete All configuration data\n"},
    {"updateLCFFile", CMD_UPDATE_LCF_FILE, commandActionConfiguration,
     "  sldd configuration updateLCFFile        - Update LCF file\n"},
    {"manualSetTele2", CMD_TELE2_CONTROL, commandActionConfiguration,
     "  sldd configuration manualSetTele2        -manualSetTele2\n"},
    {"readFactoryValue", CMD_READ_FACTORY_VALUE, commandActionConfiguration,
     "  sldd configuration readFactoryValue        -readFactoryValue\n"},
    {"factoryLCFUpdate", CMD_FACTORY_LCF_UPDATE, commandActionConfiguration,
     "  sldd configuration factoryLCFUpdate        -factory LCF update\n"},
    {"autoFactoryLCFUpdate", CMD_AUTO_FACTORY_UPDATE, commandActionConfiguration,
     "  sldd configuration autoFactoryLCFUpdate        -Auto factory LCF update\n"},
    {"deleteOneData", CMD_DELETE_ONE_VALUE, commandActionConfiguration,
     "  sldd configuration deleteOneData        -Delete one data\n"},

    {NULL, NULL, NULL, NULL}
};

//-----------------------------------------------------------------------------

#define printMessage(...) (fprintf(stderr, __VA_ARGS__))

static char* usage_configuration_id(int32_t id);
static int32_t checkArguments(int32_t argc, char* argv[]);
static int32_t getCommandId(char* string);

//-----------------------------------------------------------------------------

void register_configuration(){
    registerCommands(MODULE_CONFIGURATION_MANAGER, NULL, config_cmds);
}


bool commandActionConfiguration(int32_t argc, char **argv) {

    sp<IConfigurationManagerService> config = interface_cast<IConfigurationManagerService>
                (defaultServiceManager()->getService(
                    String16(CONFIGURATION_SRV_NAME)));
    sp<IConfigurationUpdaterService> config_update = interface_cast<IConfigurationUpdaterService>
                (defaultServiceManager()->getService(
                    String16(CONFIGURATION_SRV_NAME)));
    sp<IPowerManagerService> power_mgr = interface_cast<IPowerManagerService>
                (defaultServiceManager()->getService(
                    String16(POWER_SRV_NAME)));


    if (config == NULL) {
        printMessage("Configuration manager is not ready, try later\n");
        return false;
    }
    if (config_update == NULL) {
        printMessage("Configuration manager is not ready, try later\n");
        return false;
    }

    int32_t result = checkArguments(argc, argv);
    if (result == -1) {
        printMessage("%s", usage_configuration_id(0));
        return true;
    } else if (result > 0) {
        printMessage("%s", usage_configuration_id(result));
        return true;
    } else {
        // normal case
    }

    int32_t id = getCommandId(argv[0]);
    if (id == -1) {
        printMessage("%s", usage_configuration_id(0));
        return true;
    }

    switch(id) {
        case CMD_GET_DATA: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get Data from Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");

            sp<ConfigurationBuffer> config_buffer;
            LOGV("first arguments %s", argv[1]);
            config_buffer= config->get(OEM_CONFIG_FILE, NAME_VAL, argv[1]);

            if (config_buffer == NULL) {
                printMessage(" -> result : Unknown\n");
            } else {
                printMessage(" -> result : %s\n", (char*)config_buffer->data());
            }
        } break;

        case CMD_SET_DATA: {

             sp<ConfigurationBuffer> config_buffer_input = new ConfigurationBuffer();
             error_t error_flag;

             config_buffer_input->setTo((uint8_t*)argv[3]);

             if(strcmp(argv[2], "bool")==0){
                config_buffer_input->setType(ConfigurationBuffer::TYPE_BOOL);
             }else if(strcmp(argv[2], "U8")==0){
                config_buffer_input->setType(ConfigurationBuffer::TYPE_UINT8);
             }else if(strcmp(argv[2], "U16")==0){
                config_buffer_input->setType(ConfigurationBuffer::TYPE_UINT16);
             }else if(strcmp(argv[2], "U32")==0){
                config_buffer_input->setType(ConfigurationBuffer::TYPE_UINT32);
             }else if(strcmp(argv[2], "String")==0){
                //nothing to do
             }else{
                printMessage(" -> Wrong type of arguments\n");
                break;
             }

             printMessage("-------------------------------------------------\n");
             printMessage("    Set Data to Configuration Mgr\n");
             printMessage("-------------------------------------------------\n");


             error_flag= config->change_set(OEM_CONFIG_FILE, NAME_VAL, argv[1], config_buffer_input);

             if (error_flag == E_OK) {
                 printMessage(" -> result : Success to change data %s\n", argv[1]);
             } else {
                 printMessage(" -> result : Fail to change data %s\n", argv[1]);
             }
         } break;

        case CMD_LOCK_DATA: {
            error_t error_flag;
            printMessage("-------------------------------------------------\n");
            printMessage("    Lock Data from Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");
            error_flag = config->change_write_permission(OEM_CONFIG_FILE, NAME_VAL, argv[1], 1);

            if(error_flag == E_OK){
                printMessage(" -> result : Success to lock %s\n", argv[1]);
            }else{
                printMessage(" -> result : Fail to lock %s\n", argv[1]);
            }
        } break;

        case CMD_UNLOCK_DATA: {
            error_t error_flag;
            printMessage("-------------------------------------------------\n");
            printMessage("    Unlock Data from Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");
            error_flag = config->change_write_permission(OEM_CONFIG_FILE, NAME_VAL, argv[1], 0);

            if(error_flag == E_OK){
                printMessage(" -> result : Success to unlock %s\n", argv[1]);
            }else{
                printMessage(" -> result : Fail to unlock %s\n", argv[1]);
            }
        } break;

        case CMD_COUNTRYDB_GET: {
            uint32_t temp_data=0;
            printMessage("-------------------------------------------------\n");
            printMessage("    Get Data from Configuration Mgr Country Database\n");
            printMessage("-------------------------------------------------\n");
            sscanf(argv[1], "%d", &temp_data);
            sp<CountryDBBuffer> get_countryDB_item= config->get_countryDB(temp_data);
            if(get_countryDB_item==NULL)
                printMessage("NULL pointer return, False");
            else{
                printMessage("MCC: %d Active:%d type:%d getEcallRequestCscNumber:%d getEcallPsapType:%d get countryItemSize:%d\n", \
                    get_countryDB_item->getMcc(), get_countryDB_item->getActive(), get_countryDB_item->getType(), get_countryDB_item->getECallRequestCscNumber(), get_countryDB_item->getECallPsapType(), get_countryDB_item->getCountryItemsSize());
                for(int32_t i=1; i<=4; i++){
                printMessage("%d of addressID: %d addressItem: %s\n", i, get_countryDB_item->getCountryItemIndexMap(i), get_countryDB_item->getCountryItem(i)->data());
                }
            }
        }break;

        case CMD_COUNTRYDB_SET:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Set Data to Configuration Mgr Country Database\n");
            printMessage("-------------------------------------------------\n");

            sp<CountryDBBuffer> new_item = new CountryDBBuffer();
            new_item->setMcc(100);
            new_item->setActive(false);
            new_item->setType(CountryDBBuffer::GSM_GPRS);
            new_item->setECallRequestCscNumber(false);
            new_item->setEcallPsapType(CountryDBBuffer::autoAllowed);
            new_item->setCountryItemsSize(4);
            sp<Buffer> temp_buf[4];
            for(uint32_t i=1; i<=4; i++){
                new_item->setCountryItemIndexMap(i, i);
                temp_buf[i-1] = new Buffer();
                temp_buf[i-1] -> setSize(strlen("12345678901234567")+1);
                temp_buf[i-1] -> setTo((uint8_t*)"12345678901234567", strlen("12345678901234567")+1);
                new_item->setCountryItem(i, temp_buf[i-1]);
            }
            error_t confirm = config->set_countryDB(new_item);
            if(confirm == E_ERROR)
                printMessage("ERROR\n");
            else
                printMessage("SUCCESS\n");


        }break;

        case CMD_COUNTRYDB_SAVE:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Save Data to Configuration Mgr Country Database\n");
            printMessage("-------------------------------------------------\n");

            error_t confirm = config->save_countryDB();
            if(confirm == E_ERROR)
                printMessage("ERROR\n");
            else
                printMessage("SUCCESS\n");


        }break;

        case CMD_FACTORY_SAVE:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Save Factory Data to Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");

            error_t confirm = config->factory_save(argv[1]);
            if(confirm == E_ERROR)
                printMessage("ERROR\n");
            else
                printMessage("SUCCESS\n");


        }break;

        case CMD_FACTORY_RESET:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Reset Factory Data to Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");

            error_t confirm = config->factory_reset(argv[1]);
            if(confirm == E_ERROR)
                printMessage("ERROR\n");
            else
                printMessage("SUCCESS\n");

        }break;

        case CMD_UPDATE_FILE:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Update File to Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");

            error_t confirm = config_update->parseUpdateFileToConfigMgr((uint8_t*)argv[1]);
            if(confirm == E_ERROR)
                printMessage("ERROR\n");
            else
                printMessage("SUCCESS\n");
        }break;

        case CMD_RESTORE_IMPORTANT:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Restore File to Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");

            error_t confirm = config_update->restoreImportantValue();
            if(confirm == E_ERROR)
                printMessage("ERROR\n");
            else
                printMessage("SUCCESS\n");
        }break;

        case CMD_DELETE_ALL_DATA:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Delete all data of Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");

            //std::remove("/userdata/config/*.*");
            system("rm -rf /userdata/config");
            printMessage("Delete File System, Rebooting automatically!! Don't touch!!\n");

            power_mgr->reset(POWERMANAGER_RESET_ALL);
        }break;

        case CMD_UPDATE_LCF_FILE:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Update LCF logic from Configuration Mgr\n");
            printMessage("-------------------------------------------------\n");

            error_t confirm = config_update->updateFileToConfigMgr((uint8_t*)argv[1]);
            if(confirm == E_ERROR)
                printMessage("ERROR\n");
            else
                printMessage("SUCCESS\n");

        }break;

        case CMD_TELE2_CONTROL:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Telematics 2 value control only use for controlling NAD\n");
            printMessage("-------------------------------------------------\n");

            sp<ConfigurationBuffer> tele2 = new ConfigurationBuffer();
            tele2->setType(ConfigurationBuffer::TYPE_UINT8);
            tele2->setTo((uint8_t*)argv[1]);
            error_t error_flag= config->change_set(OEM_CONFIG_FILE, NAME_VAL, "telematics2", tele2);
            if(error_flag == E_ERROR){
                printMessage("ERROR\n");
                break;
            }
            uint8_t data=0;
            sscanf(argv[1], "%d", &data);
            if(config_update->handToNAD(data, 0)==E_ERROR){
                printMessage("ERROR NOT hand to NAD\n");
            }else{
                printMessage("Success\n");
            }
        }break;

        case CMD_READ_FACTORY_VALUE:{
            printMessage("-------------------------------------------------\n");
            printMessage("    read factory value\n");
            printMessage("-------------------------------------------------\n");
            sp<ConfigurationBuffer> config_buffer;
            LOGV("first arguments %s", argv[1]);
            config_buffer= config_update->getFactoryValue(reinterpret_cast<uint8_t*>(argv[1]));

            if (config_buffer == NULL) {
                printMessage(" -> result : Unknown\n");
            } else {
                printMessage(" -> result : %s\n", (char*)config_buffer->data());
            }
        }break;

        case CMD_FACTORY_LCF_UPDATE:{
            printMessage("-------------------------------------------------\n");
            printMessage("    factory LCF update\n");
            printMessage("-------------------------------------------------\n");

            uint32_t countryID = 0;
            uint32_t server = 0;

            sscanf(argv[1], "%d", &countryID);
            sscanf(argv[2], "%d", &server);
            error_t confirm = config_update->factoryLCFUpdate(countryID, server);
            if(confirm == E_OK){
                printMessage(" -> result : Success\n");
            }else{
                printMessage(" -> result : Fail\n");
            }

        }break;

        case CMD_AUTO_FACTORY_UPDATE:{
            printMessage("-------------------------------------------------\n");
            printMessage("    factory LCF update\n");
            printMessage("-------------------------------------------------\n");
            error_t confirm = config_update->autoFactoryUpdate();
            if(confirm == E_OK){
                printMessage(" -> result : Success\n");
            }else{
                printMessage(" -> result : Fail\n");
            }

        }break;

        case CMD_DELETE_ONE_VALUE:{
            printMessage("-------------------------------------------------\n");
            printMessage("    delete One Data\n");
            printMessage("-------------------------------------------------\n");
            error_t confirm = config->deleteConfiguration(OEM_CONFIG_FILE, NAME_VAL, argv[1]);
            if(confirm == E_OK){
                printMessage(" -> result : Success\n");
            }else{
                printMessage(" -> result : Fail\n");
            }

        }break;

        default:
            printMessage("Not command : %d\n", sl::atoi(argv[0]));
            break;

    }

    return true;
}


char* usage_configuration(char* cmd) {
    return usage_configuration_id(getCommandId(cmd));
}


static int32_t checkArguments(int32_t argc, char* argv[]) {
    if (argc < 1) {
        return -1;
    }

    int32_t cmdId = getCommandId(argv[0]);
    if (cmdId < 0) {
        return -1;
    }

    int32_t result = -1;
    switch(cmdId) {
        case CMD_GET_DATA:
            if (argc == 2) {
                result = 0;
            }break;
        case CMD_LOCK_DATA:
            if (argc == 2) {
                result = 0;
            }break;
        case CMD_UNLOCK_DATA:
            if (argc == 2) {
                result = 0;
            }break;
        case CMD_SET_DATA:
            if (argc == 4) {
                result = 0;
            }break;
        case CMD_COUNTRYDB_GET:
            if(argc == 2){
                result = 0;
            }break;
        case CMD_COUNTRYDB_SET:
            if(argc == 1){
                result = 0;
            }break;
        case CMD_COUNTRYDB_SAVE:
            if(argc == 1){
                result = 0;
            }break;
        case CMD_FACTORY_SAVE:
            if(argc == 2){
                result = 0;
            }break;
        case CMD_FACTORY_RESET:
            if(argc == 2){
                result = 0;
            }break;
        case CMD_UPDATE_FILE:
            if(argc == 2){
                result = 0;
            }break;
        case CMD_RESTORE_IMPORTANT:
            if(argc == 1){
                result = 0;
            }break;
        case CMD_DELETE_ALL_DATA:
            if(argc == 1){
                result = 0;
            }break;
        case CMD_UPDATE_LCF_FILE:
            if(argc == 2){
                result = 0;
            }break;
        case CMD_TELE2_CONTROL:
            if(argc == 2){
                result = 0;
            }break;
        case CMD_READ_FACTORY_VALUE:
            if(argc == 2){
                result = 0;
            }break;
        case CMD_FACTORY_LCF_UPDATE:
            if(argc == 3){
                result = 0;
            }break;
        case CMD_AUTO_FACTORY_UPDATE:
            if(argc == 1){
                result = 0;
            }break;
        case CMD_DELETE_ONE_VALUE:
            if(argc == 1){
                result = 0;
            }break;
        default:
            break;
    }

    return result;
}

static int32_t getCommandId(char* string) {
    for (int32_t i=0; config_cmds[i].id != -1; i++) {
        if (!strcasecmp(config_cmds[i].name, string)) {
            return config_cmds[i].id;
        }
    }
    return -1;

}

static char* usage_configuration_id(int32_t id) {

    return " sldd cofiguration [<command>] [parameters] \n"
            "    <command> \n"
            "       getData                     - Get Data from Configuration Manager \n"
            "        - getData parameter : name (e.g. getData VIN)\n"
            "       setData                     - Set Data to Configuration Manager \n"
            "        - setData parameter : name, type, value (e.g. setData VIN String abc123)\n"
            "       lockData                    - Lock Data on Configuration Manager \n"
            "        - lockData parameter : name (e.g. lockData VIN)\n"
            "       unlockData                  - Unlock Data on Configuration Manager \n"
            "        - lockData parameter : name (e.g. lockData VIN)\n"
            "       getCountryData              - Get country Data on Configuration Manager \n"
            "        - getCountryData parameter : NONE\n"
            "       setCountryData              - Set country Data on Configuration Manager \n"
            "        - setCountryData : This can be only used for test\n"
            "       saveCountryData              - Save country Data on Configuration Manager \n"
            "        - saveCountryData : This can be only used for test\n"
            "       factorySave                - Save Factory Data on Configuration Manager \n"
            "        - factorySave parameter: Path to save file\n"
            "       factoryReset              - Reset Factory Data on Configuration Manager \n"
            "        - factoryReset parameter : Path of reset file\n"
            "       updateFile                - Update Configuration file to Confiiguration Manager \n"
            "        - updateFile parameter: Path to update file\n"
            "       restoreCoreData            -Restore Core Data of Configuration Mgr \n"
            "        - restoreCoreData parameter : NONE\n"
            "       deleteAllData              - Delete All configuration data from Configuration Manager \n"
            "        - deleteAllData Parameter : NONE\n"
            "       updateLCFFile                - Update Configuration file to Confiiguration Manager \n"
            "        - updateFile parameter: Path to update file\n"
            "       manualSetTele2             - Manual Setting Telematics2 value use for svt \n "
            "       - Change Telematics2 value\n"
             "       readFactoryValue             - Read factory value \n "
            "       - readFactoryValue Parameter : Parameter\n"
            "       factoryLCFUpdate             - Update LCF in factory \n "
            "       - factoryLCFUpdate Parameter : countryID value, server value\n"
            "       autoFactoryLCFUpdate         - Update LCF in factory \n "
            "       - autofactoryLCFUpdate Parameter :None \n"
            ;

}

