/*
 * DiagManagerV2.h
 *
 *  Created on: 2016. 1. 12.
 *      Author: daeup.kim
 */

#ifndef SERVICE_LAYER_DAEMON_SLDD_DIAGMANAGERV2_H_
#define SERVICE_LAYER_DAEMON_SLDD_DIAGMANAGERV2_H_

#define MODULE_DIAG_MANAGER "DiagManager"

char* usage_diag(char *cmd);
extern void register_diag();
extern bool commandActionDiag(int32_t argc, char **argv);


#endif /* SERVICE_LAYER_DAEMON_SLDD_DIAGMANAGERV2_H_ */
