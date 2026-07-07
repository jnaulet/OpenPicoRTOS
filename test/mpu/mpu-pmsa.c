#include <stdio.h>
#include <stdint.h>

struct mpu_desc {
  /* human-readable */
  uintptr_t bottom;
  uintptr_t top;
  /* arm */
  uint32_t RBAR;
  uint32_t SIZE;
  uint32_t SRD;
};

#define div_floor(a, b) (((a)) / (b))
#define div_ceil(a, b) (((a) + (b - 1)) / (b))

static int mpu_desc_from_addr_n(uintptr_t addr, size_t n,
				/*@out@*/ struct mpu_desc *d)
{
#define MPU_RASR_SIZE_COUNT 32
#define MPU_SRD_SIZE_MIN    256

  size_t p2;
  size_t size;
  uintptr_t top;
  uintptr_t bottom;
  uintptr_t midpoint = addr + div_ceil(n, 2u);
  
  /* closest pow2 */
  for(p2 = (size_t)8; p2 < (size_t)MPU_RASR_SIZE_COUNT; p2++){
    size_t mask = (size_t)(1u << p2) - 1u;
    /* adjust */
    size = mask + 1u;
    bottom = midpoint & ~mask;
    top = (bottom + size);
    /* out condition */
    if(bottom <= addr && size >= n)
      break;
  }

  size_t part_len = size >> 3;
  size_t before = div_floor((addr - bottom), part_len);
  size_t before_len = before * part_len;
  size_t after = div_floor(top - (addr + n), part_len);
  size_t after_len = after * part_len;

  /* underflow */
  if(after > n) after_len = 0;
  
  d->RBAR = bottom;
  d->SIZE = p2 - 1;
  d->SRD = (uint32_t)0xffu & ((uint32_t)0xff00u >> after |
			      (uint32_t)0x00ffu >> (8u - before));

  /* correct our numbers */
  d->bottom = bottom + before_len;
  d->top = top - after_len;
  
  return (int)(d->top - addr);
}

static int find_pow2(uintptr_t addr, size_t n)
{
  printf("*** 0x%08lx-0x%08lx (%zu)***\n",
	 addr, addr + n, n);

  size_t left = n;
  uintptr_t RBAR = addr;

  for(;;){
    struct mpu_desc desc;
    int s = mpu_desc_from_addr_n(RBAR, left, &desc);

    printf("-> 0x%08lx-0x%08lx (%zu), 0x%08x, %0x8, %08x\n",
	   desc.bottom, desc.top, (size_t)(desc.top - desc.bottom),
	   desc.RBAR, desc.SIZE, desc.SRD);
    
    if((size_t)s >= left)
      break;

    /* next */
    RBAR = (uint32_t)desc.top;
    left -= (uint32_t)s;
  }

  return 0;
}

int main(void)
{
  find_pow2((uintptr_t)0x20421f80, (size_t)136);
  find_pow2((uintptr_t)0x40034000, (size_t)252);
  find_pow2((uintptr_t)0x20424920, (size_t)3584);
  find_pow2((uintptr_t)0x20421a00, (size_t)1408);
  find_pow2((uintptr_t)0x20422008, (size_t)212);
  find_pow2((uintptr_t)0x20423500, (size_t)12);
  find_pow2((uintptr_t)0x4003c000, (size_t)236);
  find_pow2((uintptr_t)0x400e1600, (size_t)236);
  find_pow2((uintptr_t)0x2042350c, (size_t)12);
  find_pow2((uintptr_t)0x40064000, (size_t)236);

  printf("\n");
  
  find_pow2((uintptr_t)0xe000e000, (size_t)0x1000);
  find_pow2((uintptr_t)0x24020000, (size_t)0x2000);
  find_pow2((uintptr_t)0x24022580, (size_t)0x80);
  find_pow2((uintptr_t)0x24000000, (size_t)0x2000);
  find_pow2((uintptr_t)0x24010000, (size_t)0x10000);
  find_pow2((uintptr_t)0x24022700, (size_t)0xe0);
  find_pow2((uintptr_t)0x24023500, (size_t)0x1e0);
  find_pow2((uintptr_t)0x24023300, (size_t)0x1e0);
  find_pow2((uintptr_t)0x24023100, (size_t)0x1e0);
  find_pow2((uintptr_t)0x24022f00, (size_t)0x1e0);
  find_pow2((uintptr_t)0x24022d00, (size_t)0x1e0);
  find_pow2((uintptr_t)0x24022b00, (size_t)0x1e0);
  find_pow2((uintptr_t)0x24022900, (size_t)0x1e0);
  find_pow2((uintptr_t)0x24022858, (size_t)12);
  find_pow2((uintptr_t)0x20443620, (size_t)40);
  find_pow2((uintptr_t)0x204434f0, (size_t)1136);

  return 0;
}
