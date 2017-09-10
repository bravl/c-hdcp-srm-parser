/**
 * @file main.c
 * @Brief Main file of SRM parse
 * @author Bram Vlerick
 * @version 1.0
 * @date 2016-12-21
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "srm.h"


/* ---------------------------------------------------------------------------*/
/**
 * @Brief  Main function of SRM parse
 *
 * @Param argc
 * @Param argv[]
 *
 * @Returns   -1 if failed, 0 if succesfull
 */
/* ---------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int fd;
	struct stat sb;
	struct srm_packet *pkt = NULL;
	const unsigned char *memblk = NULL;

	logger_set_loglevel(LOG_LVL_INFO | LOG_LVL_WARNING | LOG_LVL_OK |
			    LOG_LVL_ERROR);

	if (argc != 2) {
		logger_log(LOG_LVL_ERROR, "Usage: srm <filename>");
		return -1;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		logger_log(LOG_LVL_ERROR, "Failed to open file");
		return -1;
	}
	fstat(fd,&sb);

	memblk = mmap(NULL,sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (!memblk) {
		logger_log(LOG_LVL_ERROR, "Failed to create mmap");
		return -1;
	}
	logger_log(LOG_LVL_OK,"Mmap created");

	pkt = create_srm_packet(memblk);
	if (!pkt) {
		logger_log(LOG_LVL_ERROR, "Failed to parse header");
		return -1;
	}

	return 0;
}

