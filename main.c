/*
 * Copyright (c) 2016 Sugizaki Yukimasa
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <vc4vec.h>

const unsigned code[] = {
#include "test.qhex"
};
const int code_size = sizeof(code);
const int unif_len = 1024;

int main()
{
	int i, j;
	signed *p = NULL;
	float *pf = NULL;
	struct vc4vec_mem mem_code, mem_unif, mem_vpm;

	vc4vec_init();
	vc4vec_mem_alloc(&mem_code, code_size);
	vc4vec_mem_alloc(&mem_unif, unif_len * (32 / 8));
	vc4vec_mem_alloc(&mem_vpm, 16 * 16 * (32 / 8));
	memcpy(mem_code.cpu_addr, code, code_size);

	p = mem_unif.cpu_addr;
	*p++ = mem_vpm.gpu_addr;

	p = mem_vpm.cpu_addr;
	for (i = 0; i < 16; i ++)
		for (j = 0; j < 16; j ++)
			p[i * 16 + j] = i * 16 + j;

	launch_qpu_job_mailbox(1, 0, 10e6, mem_unif.gpu_addr, mem_code.gpu_addr);

	printf("In hex:\n");
	p = mem_vpm.cpu_addr;
	for (i = 0; i < 16; i ++) {
		printf("%2d:", i);
		for (j = 0; j < 16; j ++)
			printf(" 0x%08x", p[i * 16 + j]);
		printf("\n");
	}

	printf("In float:\n");
	pf = mem_vpm.cpu_addr;
	for (i = 0; i < 16; i ++) {
		printf("%2d:", i);
		for (j = 0; j < 16; j ++)
			printf(" %10.8f", pf[i * 16 + j]);
		printf("\n");
	}

	vc4vec_mem_free(&mem_vpm);
	vc4vec_mem_free(&mem_unif);
	vc4vec_mem_free(&mem_code);
	vc4vec_finalize();
	return 0;
}
