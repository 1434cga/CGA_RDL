
#ifndef SLDD_CANSIGNAL_H
#define SLDD_CANSIGNAL_H

#define MODULE_CAN "can"

//bool CAN_getCANSignal(uint16_t sigid, sp<Buffer> sigdatabuf);
void CAN_PowerMode(uint16_t sigid);
void CAN_RCMCrashStatus(uint16_t sigid);
void CAN_EPBEUCDStatus(uint16_t sigid);
void CAN_ODORollingCount(uint16_t sigid);
void CAN_getsignaldata(uint16_t sigid);
void CAN_transmitsignaldata(uint16_t sigid, char **argv);// argv is signal data

//[2016.01.21 Seungah Jung] Add WiFi CAN Rx
void CAN_HotspotRequest(uint16_t sigid);
void CAN_PasswordRequest(uint16_t sigid);
void CAN_PasswordHotspot(uint16_t sigid);
void CAN_RestorePwdDefault(uint16_t sigid);

//[2016.01.23 Joonho Park] Add MBC CAN Rx
void CAN_DataContract(uint16_t sigid);
void CAN_Roaming(uint16_t sigid);
void CAN_InfotainmentLanguage(uint16_t sigid);

extern bool commandActionCAN(int32_t argc, char **argv);
extern char* usage_CAN(char* cmd);
extern void register_can(void);

#endif
