#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800

#include "cubebox.h"

bool apic_checkSupport() {
	uint32_t eax, edx;
	cpuid(1, &eax, &edx);

	return edx & CPUID_FEAT_EDX_APIC;
}

void apic_setBase(uintptr_t base) {
	uint32_t edx = 0;
	uint32_t eax = (base & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;

	// edx = (apic >> 32) & 0x0f;

	setMsr(IA32_APIC_BASE_MSR, eax, edx);
}

uintptr_t apic_getBase() {
	uint32_t eax, edx;
	getMsr(IA32_APIC_BASE_MSR, &eax, &edx);

	return (eax & 0xffffff000);
}

void apic_enable() {
	/*
		TODO: Recommendation from datasheet
		=====================================
		Section 11.4.1 of 3rd volume of Intel SDM 
		recommends mapping the base address page 
		as strong uncacheable for correct 
		APIC operation.
	*/

	apic_setBase(apic_getBase());
}

void apic_init() {
	if (apic_checkSupport())
		apic_enable();
}
