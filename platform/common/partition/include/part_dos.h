/*
 * <part_dos.h>
 * Copyright (C) 2021 Unisoc Communications Inc.
 * History:
 * 	<2021> <part_dos.h>
 *	Macro definition and structure of dos type partition.
 */

#ifndef _PART_DOS_H
#define _PART_DOS_H

#define DOS_MBR	0
#define DOS_PBR	1

/* Offset in dos part */
#define DOS_PART_TBL_OFFSET	0x1be
#define DOS_PART_MAGIC_OFFSET	0x1fe
#define DOS_PART_DISKSIG_OFFSET	0x1b8

/* FS type */
#define DOS_PBR_FSTYPE_OFFSET	0x36
#define DOS_PBR32_FSTYPE_OFFSET	0x52

typedef struct dos_partition {
	unsigned char boot_ind;		/* 0x80 - active */
	unsigned char head;		/* starting head */
	unsigned char sector;		/* starting sector */
	unsigned char cyl;		/* starting cylinder */
	unsigned char sys_ind;		/* partition type */
	unsigned char end_head;		/* ending head */
	unsigned char end_sector;	/* ending sector */
	unsigned char end_cyl;		/* ending cylinder */
	unsigned char start[4];	/* starting sector counting from 0 */
	unsigned char size[4];		/* number of sectors in partition */
} dos_partition_t;

#endif	/* _PART_DOS_H */
