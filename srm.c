/**
 * @file srm.c
 * @Brief SRM Functions
 * @author Bram Vlerick
 * @version 1.0
 * @date 2016-12-21
 */

#include "srm.h"

static HDCP_version resolve_hdcp_version(int version)
{
	if (version == HDCP_ID) return HDCP_1_4;
	if (version == HDCP_2_2_ID) return HDCP_2_2;
	return HDCP_INVALID;
}

/* ---------------------------------------------------------------------------*/
/**
 * @Brief  Parse SRM header
 *
 * @Param buffer Buffer containing the SRM message
 *
 * @Returns   NULL if failed, allocated header if success
 */
/* ---------------------------------------------------------------------------*/
struct srm_header_1_4 *parse_srm_header_1_4(const unsigned char *buffer)
{
	struct srm_header_1_4 *hdr = NULL;
	if (!buffer) goto error;

	logger_log(LOG_LVL_INFO,"Parsing SRM header");

	hdr = malloc(sizeof(struct srm_header_1_4));
	if (!hdr) goto error;

	logger_log(LOG_LVL_INFO,"Copying data from buffer to header");

	hdr->srm_id = (buffer[0] & 0xf0) >> 4;
	hdr->reserved = ((buffer[0] & 0x0f) << 8 ) | (buffer[1]);
	hdr->srm_version = ((buffer[2]) << 8) | (buffer[3]);
	hdr->srm_gen_number = (buffer[4]);
	hdr->vrl_len = ((buffer[5]) << 16) | ((buffer[6]) << 8) | ((buffer[7]));
	logger_log(LOG_LVL_INFO,"Header id: %d",hdr->srm_id);
	logger_log(LOG_LVL_INFO,"Header reserved: %d",hdr->reserved);
	logger_log(LOG_LVL_INFO,"Header version: %d",hdr->srm_version);
	logger_log(LOG_LVL_INFO,"Header generation: %d",hdr->srm_gen_number);
	logger_log(LOG_LVL_INFO,"Vector list len: %d",hdr->vrl_len);

	return hdr;

error:	if (hdr) free(hdr);
	return NULL;
}

/* ---------------------------------------------------------------------------*/
/**
 * @Brief  Parse SRM header
 *
 * @Param buffer Buffer containing the SRM message
 *
 * @Returns   NULL if failed, allocated header if success
 */
/* ---------------------------------------------------------------------------*/
struct srm_header_2_2 *parse_srm_header_2_2(const unsigned char *buffer)
{
	struct srm_header_2_2 *hdr = NULL;
	if (!buffer) goto error;

	logger_log(LOG_LVL_INFO,"Parsing SRM header");


error:	if (hdr) free(hdr);
	return NULL;
}


/* ---------------------------------------------------------------------------*/
/**
 * @Brief  Create the SRM package
 *
 * @Param buffer Buffer containing the SRM header
 *
 * @Returns  NULL if failed, allocated packet if successfull
 */
/* ---------------------------------------------------------------------------*/
struct srm_packet *create_srm_packet(const unsigned char *buffer)
{
	int tmp_id = 0;
	struct srm_packet *pkt = NULL;

	pkt = malloc(sizeof(struct srm_packet));
	if (!pkt) {
		logger_log(LOG_LVL_ERROR, "Failed to create packet");
		//goto error;
	}

	pkt->revoc_list = NULL;
	memset(pkt->DCP_LLC,0,DCP_LLC_LEN);

	pkt->hdcp_version = resolve_hdcp_version((buffer[0] & 0xf0) >> 4);
	if (pkt->hdcp_version == HDCP_1_4) {
		logger_log(LOG_LVL_INFO,"HDCP Version 1.4");
		pkt->header._1_4 = NULL;
		pkt->header._1_4 = parse_srm_header_1_4(buffer);
	} else if (pkt->hdcp_version == HDCP_2_2) {
		logger_log(LOG_LVL_INFO,"HDCP Version 2.2");
		pkt->header._2_2 = NULL;
	} else {
		logger_log(LOG_LVL_ERROR,"INVALID HDCP Version");
	}
}

	/*pkt->header = parse_srm_header(buffer);
	if (!pkt->header) {
		logger_log(LOG_LVL_ERROR, "Failed to create the header");
		goto error;
	}

	switch (pkt->header->srm_id) {
		case HDCP_ID:
			logger_log(LOG_LVL_OK, "HDCP 1.4 packet");
			break;
		case HDCP_2_2_ID:
			logger_log(LOG_LVL_OK, "HDCP 2.2 packet");
			break;
		default:
			logger_log(LOG_LVL_ERROR, "Invalid HDCP SRM packet");
			goto error;
	}

	// Check the VRL length to see if there are any keys
	if (pkt->header->vrl_len == VRL_MIN_SIZE) {
		logger_log(LOG_LVL_WARNING, "Vector revocation list"
			   "contains no keys");
	}

	// Offset 8 -> start of data first 8 bytes (0 - 7) are header
	memcpy(pkt->DCP_LLC, &buffer[8 + (pkt->header->vrl_len - VRL_MIN_SIZE)],
	       DCP_LLC_LEN);
	return pkt;
	*/

