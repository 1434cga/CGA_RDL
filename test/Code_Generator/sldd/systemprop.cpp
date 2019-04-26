/**
 * \file    systemprop.cpp
 * \brief   systemproperty sldd command
 *
 * \details
 *    This software is copyright protected and proprietary to
 *    LG electronics. LGE grants to you only those rights as
 *    set out in the license conditions. All other rights remain
 *    with LG electronics.
 * \author       dy616.lee
 * \date         2018.02.05
 * \attention Copyright (c) 2017 by LG electronics co, Ltd. All rights reserved.
 */
#define LOG_TAG "sysprop"

#include <map>
#include <cstring>

#include "sldd_common.h"
#include "systemprop.h"
#include "man.h"
#include "utils/external/mindroid/lang/String.h"

#include <ctype.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system_property.h>

enum Commands {
    CMD_PROP_TEST = 0,

    //--------------------------------
    // hidden commands
    CMD_HIDDEN,
};

static void usage_sysprop_id(void);
static int32_t getCommandId(char* string);
static int32_t checkArguments(int32_t argc, char** argv);
bool commandActionPROP(int32_t argc, char **argv);

sldd_cmd_table_t syspropCmds[] = {
    {"test", CMD_PROP_TEST, commandActionPROP, "systemproperty test \n"},
    {NULL, NULL, NULL, NULL}
};

//-----------------------------------------------------------------------------

#define fprintf(stderr, ...) (fprintf(stderr, __VA_ARGS__))

static bool tmp_property_check(const char_t *prop_name, char_t *prop_val,
		char_t *prop_perm)
{
	FILE *fp;
	char_t tmp_file[PROP_MAX] = {0,};
	char_t line[PROP_MAX] = {0,};
	char_t *result, *perm_name, *name, *val, *perm;
	bool matched = false;

	strcpy(tmp_file, TEMP_PROPERTY_FILE);
	fp = fopen(tmp_file, "r");
	if (!fp) {
		fprintf(stderr, "Fail to open /tmp/property\n");
		fclose(fp);
		return matched;
	}

	while((result = fgets(line, sizeof(line), fp)) != NULL) {
		perm_name = converting_char_name(line, "=");
		name = converting_char(perm_name, "_");
		if (strcmp(prop_name, name))
			continue;

		val = converting_char(line, "=");
		int32_t size_val = sizeof(prop_val);
		/* Except white space comparison (\0 vs. \n) */
		if (strncmp(prop_val, val, size_val))
			goto out;

		perm = converting_char_name(line, ".");
		if (strcmp(prop_perm, perm))
			goto out;

		matched = true;
		break;
	}

out:
	fclose(fp);
	return matched;
}

static void remove_test_prop(const char_t *prop_name)
{
	DIR *dir;
	struct dirent* entry;
	char_t *tmp;
	char_t name_1[PROP_NAME_MAX] = {0,};
	char_t name_2[PROP_NAME_MAX] = {0,};
	char_t path[PROP_MAX] = {0,};

	dir = opendir(SYSTEM_PROPERTY_FILES);
	if (!dir) {
		fprintf(stderr, "Fail to open system property directory\n");
	}

	while((entry = readdir(dir)) != NULL) {
		if (strstr(entry->d_name, ".prop"))
			continue;

		tmp = converting_char(entry->d_name, "_");
		strcpy(name_1, tmp);
		tmp = converting_char(name_1, "_");
		strcpy(name_2, tmp);

		if (!strcmp(prop_name, name_1) || !strcmp(prop_name, name_2)) {
			strcpy(path, SYSTEM_PROPERTY_FILES);
			strcat(path, entry->d_name);
			remove(path);
		}
	}

	closedir(dir);
}

static bool perst_property_check(const char_t *prop_name, char_t *prop_val,
		char_t *prop_perm, bool set_val_chg)
{
	DIR *dir;
	FILE *fp;
	struct dirent* entry;
	char_t *name, *val, *ver;
	char_t path[PROP_MAX] = {0,};
	char_t crc_val[PROP_MAX] = {0,};
	bool matched = false;

	dir = opendir(SYSTEM_PROPERTY_FILES);
	if (!dir) {
		fprintf(stderr, "Fail to open system property directory\n");
		return matched;
	}

	while((entry = readdir(dir)) != NULL) {
		if (strstr(entry->d_name, ".prop"))
			continue;

		name = converting_char(entry->d_name, "_");
		if (strcmp(prop_name, name))
			continue;

		if (set_val_chg) {
			ver = converting_char_name(entry->d_name, ".");
			if (sl::atoi(ver) == 0)
				continue;
		}

		strcpy(path, SYSTEM_PROPERTY_FILES);
		strcat(path, entry->d_name);
		fp = fopen(path, "r");
		if (!fp) {
			fprintf(stderr, "Fail to open property file\n");
			fclose(fp);
			goto out_no_fclose;
		}
		fscanf(fp, "%s", crc_val);
		val = converting_char(crc_val, "_");
		if (strcmp(prop_val, val))
			goto out;

		if (access(path, R_OK | W_OK))
			goto out;

		matched = true;
		fclose(fp);
		break;
	}

	closedir(dir);
	return matched;

out:
	fclose(fp);
out_no_fclose:
	closedir(dir);
	return false;
}

static bool memory_prop_test_rw(void)
{
	const char_t *prop_name = "rw.sysprop.mem";
	char_t prop_val[PROP_VALUE_MAX];
	char_t *prop_perm = converting_char_name(prop_name, ".");
	char_t get_val[PROP_VALUE_MAX] = {0,};
	bool result = false;

	sprintf(prop_val, "%d", time(NULL));

	int ret = set_open_property(prop_name, prop_val, MEMORY);
	if (ret < 0) {
		fprintf(stderr, "Set property failed, ret=%d\n", ret);
		return result;
	}

	result = tmp_property_check(prop_name, prop_val, prop_perm);
	if (!result) {
		fprintf(stderr, "Set property tmp check failed\n");
		return result;
	}

	char_t* _ret = get_open_property(prop_name, get_val, sizeof(prop_val));
	if (!_ret) {
		fprintf(stderr, "Get property failed\n");
		return result;
	}

	if (!strcmp(prop_val, get_val))
		result = true;

	return result;
}

static bool memory_prop_test_ro(void)
{
	const char_t *prop_name = "ro.sysprop.mem";
	char_t *prop_val = "test";
	bool result = false;

	int ret = set_open_property(prop_name, prop_val, MEMORY);
	if (ret < 0)
		result = true;

	return result;
}

static bool persist_prop_test_rw(void)
{
	const char_t *prop_name = "rw.sysprop.persist";
	char_t prop_val[PROP_VALUE_MAX];
	char_t *prop_perm = converting_char_name(prop_name, ".");
	char_t get_val[PROP_VALUE_MAX] = {0,};
	bool result = false;

	sprintf(prop_val, "%d", time(NULL));

	int ret = set_open_property(prop_name, prop_val, PERSISTENT);
	if (ret < 0) {
		fprintf(stderr, "Set property failed, ret=%d", ret);
		return result;
	}

	result = tmp_property_check(prop_name, prop_val, prop_perm);
	if (!result) {
		fprintf(stderr, "Set property tmp check failed\n");
		return result;
	}

	result = perst_property_check(prop_name, prop_val, prop_perm, false);
	if (!result) {
		fprintf(stderr, "Set property perst check failed\n");
		return result;
	}

	char_t* _ret = get_open_property(prop_name, get_val, sizeof(prop_val));
	if (!_ret) {
		fprintf(stderr, "Get property failed\n");
		return result;
	}

	if (!strcmp(prop_val, get_val))
		result = true;

	remove_test_prop(prop_name);

	return result;
}

static bool persist_prop_test_ro(void)
{
	const char_t *prop_name = "ro.sysprop.persist";
	char_t *prop_val = "test";
	bool result = false;

	int ret = set_open_property(prop_name, prop_val, PERSISTENT);
	if (ret < 0)
		result = true;

	return result;
}

static bool persist_prop_test_dup(void)
{
	const char_t *prop_name = "rw.sysprop.persist.dup";
	char_t prop_val[PROP_VALUE_MAX];
	char_t *prop_perm = converting_char_name(prop_name, ".");
	char_t get_val[PROP_VALUE_MAX] = {0,};
	bool result = false;

	for (int i = 0 ; i < 2 ; i++) {
		sprintf(prop_val, "%d", time(NULL));

		int ret = set_open_property(prop_name, prop_val, PERSISTENT);
		if (ret < 0) {
			fprintf(stderr, "Set property failed, ret=%d", ret);
			return result;
		}
		sleep(2);
	}

	result = tmp_property_check(prop_name, prop_val, prop_perm);
	if (!result) {
		fprintf(stderr, "Set property tmp check failed\n");
		return result;
	}

	result = perst_property_check(prop_name, prop_val, prop_perm, true);
	if (!result) {
		fprintf(stderr, "Set property perst check failed\n");
		return result;
	}

	char_t* _ret = get_open_property(prop_name, get_val, sizeof(prop_val));
	if (!_ret) {
		fprintf(stderr, "Get property failed\n");
		return result;
	}

	if (!strcmp(prop_val, get_val))
		result = true;

	remove_test_prop(prop_name);

	return result;
}

bool commandActionPROP(int32_t argc, char **argv)
{
    int32_t id = checkArguments(argc, argv);
	bool result;

    if (id < 0) {
		usage_sysprop_id();
        return true;
    }

    switch(id) {
		case CMD_PROP_TEST:
			result = memory_prop_test_rw();
            fprintf(stderr, "-------------------------------------------------\n");
            fprintf(stderr, "Property get/set %s for rw w/ memory\n",
					result ? "SUCCESS" : "FAIL");
            fprintf(stderr, "-------------------------------------------------\n");

			result = memory_prop_test_ro();
            fprintf(stderr, "-------------------------------------------------\n");
            fprintf(stderr, "Property get/set %s for ro w/ memory\n",
					result ? "SUCCESS" : "FAIL");
            fprintf(stderr, "-------------------------------------------------\n");

			result = persist_prop_test_rw();
            fprintf(stderr, "-------------------------------------------------\n");
            fprintf(stderr, "Property get/set %s for rw w/ persistent\n",
					result ? "SUCCESS" : "FAIL");
            fprintf(stderr, "-------------------------------------------------\n");

			result = persist_prop_test_ro();
            fprintf(stderr, "-------------------------------------------------\n");
            fprintf(stderr, "Property get/set %s for ro w/ persistent\n",
					result ? "SUCCESS" : "FAIL");
            fprintf(stderr, "-------------------------------------------------\n");

			result = persist_prop_test_dup();
            fprintf(stderr, "-------------------------------------------------\n");
            fprintf(stderr, "Duplicated Property get/set %s w/ persistent\n",
					result ? "SUCCESS" : "FAIL");
            fprintf(stderr, "-------------------------------------------------\n");
			break;

        default:
            fprintf(stderr, "-------------------------------------------------------\n");
            fprintf(stderr, "    Not command : %d\n", sl::atoi(argv[0]));
            fprintf(stderr, "-------------------------------------------------------\n\n");
            break;
    }

    return true;
}

static int32_t checkArguments(int32_t argc, char** argv)
{
	int32_t cmdId;

    if (argc < 1)
        return -1;

	cmdId = getCommandId(argv[0]);

    switch(cmdId) {
        case CMD_PROP_TEST:
            if (argc != 1)
                cmdId = -1;
            break;
        default:
            break;
    }

    return cmdId;
}

static int32_t getCommandId(char* string)
{
    for (int32_t i = 0 ; syspropCmds[i].name ; i++) {
        android::sp<mindroid::String> name = new mindroid::String(syspropCmds[i].name);
        if (name->equalsIgnoreCase(string)) {
            return syspropCmds[i].id;
        }
    }
    return -1;
}

static void usage_sysprop_id(void)
{
	fprintf(stderr, " sldd sysprop [<command>] \n"
			"    <command> \n"
			"       test				- Systemproperty test \n");
}

void register_sysprop()
{
    registerCommands(MODULE_SYSTEM_PROP, NULL, syspropCmds);
}
