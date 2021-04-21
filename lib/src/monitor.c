#ifndef MONITOR_H_
#define MONITOR_H_

#include "btbb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define byte	unsigned char

typedef struct indicator_btbr indicator_btbr;
typedef struct indicator_btle indicator_btle;
typedef struct indicator_btle_adv indicator_btle_adv;


typedef enum {
	PIPE_OK = 0,
	PIPE_INVALID_HANDLE,
	PIPE_FILE_NOT_ALLOWED,
	PIPE_NO_MEMORY,
} PIPE_RESULT;

struct btbb_monitor_handle {
	FILE *monitor_file;
} monitor_handle;

struct indicator_btbr {
	uint16_t flags;
	uint8_t uap;
	uint32_t lap;
	// uint16_t nap;
	uint32_t timestamp;
} ind_btbr;

struct indicator_btle {
	uint32_t aa;
	uint32_t timestamp;
} ind_btle;

struct indicator_btle_adv {
	uint8_t type;
	uint8_t random;
	char mac[6];
	uint32_t timestamp;
} ind_btle_adv;

int btbb_monitor_open_pipe(const char * filename, btbb_monitor_handle ** ph){

	int retval = 0;
	btbb_monitor_handle * handle = malloc( sizeof(btbb_monitor_handle) );
	if (handle) {
		
		FILE *monitor_file = fopen(filename, "wb");

		if (monitor_file == NULL) { return -PIPE_INVALID_HANDLE; };

		handle->monitor_file = monitor_file;
		monitor_handle.monitor_file = monitor_file;

		if (handle->monitor_file) {
			*ph = handle;
		}
		else {
			retval = -PIPE_FILE_NOT_ALLOWED;
			goto fail;
		}
	}
	else {
		retval = -PIPE_NO_MEMORY;
		goto fail;
	}
	return retval;

	fail:
	(void) btbb_monitor_close( handle );
	return retval;
}

int btbb_monitor_close(btbb_monitor_handle * h)
{
	if (h && h->monitor_file) {
		fclose(h->monitor_file);
	}
	if (h) {
		free(h);
		return 0;
	}
	return -PIPE_INVALID_HANDLE;
}

void btbb_monitor_write_btbr(uint16_t flags, uint8_t uap, uint32_t lap, uint32_t timestamp) {

	ind_btbr.lap = lap;
	ind_btbr.uap = uap;
	ind_btbr.timestamp = timestamp;
	ind_btbr.flags = flags;

	fwrite(&ind_btbr, sizeof(indicator_btbr), 1, monitor_handle.monitor_file);
	fflush(monitor_handle.monitor_file);
}

void btbb_monitor_write_btle(uint32_t aa, uint32_t timestamp) {

	ind_btle.aa = aa;
	ind_btle.timestamp = timestamp;

	fwrite(&ind_btle, sizeof(indicator_btle), 1, monitor_handle.monitor_file);
	fflush(monitor_handle.monitor_file);
}

void btbb_monitor_write_btle_adv(uint8_t type, uint8_t random, uint8_t const * mac, uint32_t timestamp){
	ind_btle_adv.type = type;
	ind_btle_adv.random = random;
	memcpy(ind_btle_adv.mac, mac, 6);
	ind_btle_adv.timestamp = timestamp;

	fwrite(&ind_btle_adv, sizeof(indicator_btle_adv), 1, monitor_handle.monitor_file);
	fflush(monitor_handle.monitor_file);
}

#endif /* MONITOR_H_ */