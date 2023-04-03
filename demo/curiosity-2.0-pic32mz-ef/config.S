	.section .config
	#include "picoRTOS_device.h"

__config:
	.org __config + 0x40
	.global __ADEVCFG
__ADEVCFG:
	.word DEVCFG3	 /* ADEVCFG3 */
	.word DEVCFG2	 /* ADEVCFG2 */
	.word DEVCFG1	 /* ADEVCFG1 */
	.word DEVCFG0	 /* ADEVCFG0 */
	.word -1	 /* ADEVCP3 */
	.word -1	 /* ADEVCP2 */
	.word -1	 /* ADEVCP1 */
	.word DEVCP0	 /* ADEVCP0 */
	.word -1	 /* ADEVSIGN3 */
	.word -1	 /* ADEVSIGN2 */
	.word -1	 /* ADEVSIGN1 */
	.word -1	 /* ADEVSIGN0 */

	.org __config + 0xc0
	.global __DEVCFG
__DEVCFG:
	.word DEVCFG3 | USERID(0xcafe)
	.word DEVCFG2
	.word DEVCFG1 | FWDTEN|WDTPS(0)|FCKSM(3)|POSCMOD(2)|FSOSCEN|FNOSC(2)
	.word DEVCFG0 | C99_EJTAGBEN|POSCBOOST|POSCGAIN(3)|SMCLR|BOOTISA|ICESEL(3)|JTAGEN
	.word -1	 /* DEVCP3 */
	.word -1	 /* DEVCP2 */
	.word -1	 /* DEVCP1 */
	.word DEVCP0 | CP
	.word -1	 /* DEVSIGN3 */
	.word -1	 /* DEVSIGN2 */
	.word -1	 /* DEVSIGN1 */
	.word -1	 /* DEVSIGN0 */

	.org __config + 0xf0
	.global __BF1SEQ3
__BF1SEQ3:
	.word 0x0000ffff /* BF1SEQ3 */
