/**
 * @file srm.h
 * @Brief  SRM declarations + function prototypes
 * @author Bram Vlerick
 * @version 1.0
 * @date 2016-12-21
 */

#ifndef _SRM_H_
#define _SRM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "debug.h"

// Digital Content protection LLC length defined by HDCP spec
#define DCP_LLC_LEN 40

// Empty Vector Revocation List size
// 40 bytes of DCP_LLC + 3 byte header (see HDCP1.4 spec p56 - VRL Length)
#define VRL_MIN_SIZE (DCP_LLC_LEN + 3)

// 0x08 Definition for HDCP SRM ID (See HDCP1.4 spec p56 - SRM ID)
#define HDCP_ID 0x08

// 0x09 Definition for HDCP 2.2 SRM ID (See HDCP2.2 spec p64-65 SRM ID)
#define HDCP_2_2_ID 0x09

typedef enum {
	HDCP_1_4,
	HDCP_2_2,
	HDCP_INVALID
}HDCP_version;

/* ---------------------------------------------------------------------------*/
/**
 * @Brief  System renewability messages structure
 */
/* ---------------------------------------------------------------------------*/
struct srm_header_1_4 {
	uint8_t	srm_id :4;
	uint16_t reserved :12;
	uint16_t srm_version;
	uint8_t srm_gen_number;
	uint32_t vrl_len :24;
};

struct srm_header_2_2 {
	uint8_t srm_id :4;
	uint8_t srm_hdcp_ind :4;
	uint8_t reserved;
	uint16_t srm_version;
	uint8_t srm_gen_number;
	uint32_t length: 24;
};

/* ---------------------------------------------------------------------------*/
/**
 * @Brief  Vector revocation list structure
 */
/* ---------------------------------------------------------------------------*/
struct vrl {
	uint8_t reserved :1;
	uint8_t num_of_devices :7;
	unsigned char *device_ksv[40];
};


/* ---------------------------------------------------------------------------*/
/**
 * @Brief  SRM package structure
 */
/* ---------------------------------------------------------------------------*/
struct srm_packet {
	HDCP_version hdcp_version;
	union {
		struct srm_header_1_4 *_1_4;
		struct srm_header_2_2 *_2_2;
	}header;
	struct vrl *revoc_list;
	unsigned char DCP_LLC[DCP_LLC_LEN];
};


/* ---------------------------------------------------------------------------*/
/**
 * @Brief  Parse SRM header
 *
 * @Param buffer Buffer containing the SRM message
 *
 * @Returns   NULL if failed, allocated header if success
 */
/* ---------------------------------------------------------------------------*/
struct srm_header_1_4 *parse_srm_header_1_4(const unsigned char *buffer);

/* ---------------------------------------------------------------------------*/
/**
 * @Brief  Parse SRM header
 *
 * @Param buffer Buffer containing the SRM message
 *
 * @Returns   NULL if failed, allocated header if success
 */
/* ---------------------------------------------------------------------------*/
struct srm_header_2_2 *parse_srm_header_2_2(const unsigned char *buffer);


/* ---------------------------------------------------------------------------*/
/**
 * @Brief  Create the SRM package
 *
 * @Param buffer Buffer containing the SRM header
 *
 * @Returns  NULL if failed, allocated packet if successfull
 */
/* ---------------------------------------------------------------------------*/
struct srm_packet *create_srm_packet(const unsigned char *buffer);
#endif
