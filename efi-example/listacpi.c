//
//  Copyright (c) 2015  Finnbarr P. Murphy.   All rights reserved.
//  Display list of ACPI tables
//  License: BSD License
//

#include <efi.h>
#include <efilib.h>

typedef struct {
	CHAR8	Signature[8];
	UINT8	Checksum;
	UINT8	OemId[6];
	UINT8	Revision;
	UINT32	RsdtAddress;
	UINT32	Length;
	UINT64	XsdtAddress;
	UINT8	ExtendedChecksum;
	UINT8	Reserved[3];
} EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER;

// XSDT is the main System Description Table.
// There are many kinds of SDT. An SDT may be split into two parts -
// A common header and a data section which is different for each table.
typedef struct {
	CHAR8	Signature[4];
	UINT32	Length;
	UINT8	Revision;
	UINT8	Checksum;
	CHAR8	OemId[6];
	CHAR8	OemTableId[8];
	UINT32	OemRevision;
	UINT32	CreatorId;
	UINT32	CreatorRevision;
} EFI_ACPI_SDT_HEADER;

#define EFI_ACPI_TABLE_GUID \
    { 0xeb9d2d30, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d }}
#define EFI_ACPI_20_TABLE_GUID \
    { 0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81 }} 

#define EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION 0x02

UINTN myStrnCmpA( CHAR8 *s1, CHAR8 *s2, UINTN len)
{
	while (*s1 && len) {
		if (*s1 != *s2) {
			break;
		}
		s1 += 1;
		s2 += 1;
		len -= 1;
	}

	return len ? *s1 - *s2 : 0;
}

VOID Ascii2UnicodeStr(CHAR8 *String, CHAR16 *UniString, UINT8 length)
{
	int	len = length;

	while (*String != '\0' && len > 0) {
		*(UniString++) = (CHAR16) *(String++);
		len--;
	}
	*UniString = '\0';
}

static int ParseRSDP( EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *Rsdp, CHAR16* GuidStr)
{
	EFI_ACPI_SDT_HEADER	*Xsdt, *Entry;
	CHAR16			SigStr[20], OemStr[20];
	UINT32			EntryCount;
	UINT64			*EntryPtr;
	int			Index;

	Print(L"\n\nACPI GUID: %s\n", GuidStr);
	Ascii2UnicodeStr((CHAR8 *)(Rsdp->OemId), OemStr, 6);
	Print(L"\nFound RSDP. Version: %d  OEM ID: %s\n", (int)(Rsdp->Revision), OemStr);

	if (Rsdp->Revision >= EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {
		Xsdt = (EFI_ACPI_SDT_HEADER *)(Rsdp->XsdtAddress);
	} else {
		Print(L"ERROR: No XSDT table found.\n");
		return 1;
	}

	if (myStrnCmpA("XSDT", (CHAR8 *)(VOID *)(Xsdt->Signature), 4)) {
		Print(L"ERROR: Invalid XSDT table found.\n");
		return 1;
	}

	Ascii2UnicodeStr((CHAR8 *)(Xsdt->OemId), OemStr, 6);
	EntryCount = (Xsdt->Length - sizeof (EFI_ACPI_SDT_HEADER)) / sizeof(UINT64);
	Print(L"Found XSDT. OEM ID: %s  Entry Count: %d\n\n", OemStr, EntryCount);

	EntryPtr = (UINT64 *)(Xsdt + 1);
	for (Index = 0; Index < EntryCount; Index++, EntryPtr++) {
		Entry = (EFI_ACPI_SDT_HEADER *)((UINTN)(*EntryPtr));
		Ascii2UnicodeStr((CHAR8 *)(Entry->Signature), SigStr, 4);
		Ascii2UnicodeStr((CHAR8 *)(Entry->OemId), OemStr, 6);
		Print(L"Found ACPI table: %s  Version: %d  OEM ID: %s\n", SigStr, (int)(Entry->Revision), OemStr);
	}

	return 0;
}

VOID Guid2String(CHAR16 *Buffer, EFI_GUID *Guid)
{
	SPrint (Buffer, 0, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		Guid->Data1,
		Guid->Data2,
		Guid->Data3,
		Guid->Data4[0],
		Guid->Data4[1],
		Guid->Data4[2],
		Guid->Data4[3],
		Guid->Data4[4],
		Guid->Data4[5],
		Guid->Data4[6],
		Guid->Data4[7]);
}

EFI_STATUS efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab)
{
	EFI_CONFIGURATION_TABLE *ect = systab->ConfigurationTable;
	EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *Rsdp = NULL;
	EFI_GUID AcpiTableGuid = ACPI_TABLE_GUID;
	EFI_GUID Acpi2TableGuid = ACPI_20_TABLE_GUID;
	CHAR16 Str[20], GuidStr[100];
	int Index;

	InitializeLib(image_handle, systab);

	// locate RSDP (Root System Description Pointer) 
	for (Index = 0; Index < systab->NumberOfTableEntries; Index++) {
		if ((CompareGuid (&(systab->ConfigurationTable[Index].VendorGuid), &AcpiTableGuid)) ||
		(CompareGuid (&(systab->ConfigurationTable[Index].VendorGuid), &Acpi2TableGuid))) {
			if (!myStrnCmpA("RSD PTR ", (CHAR8 *)(ect->VendorTable), 8)) {
				Guid2String(GuidStr, &(systab->ConfigurationTable[Index].VendorGuid));
				Rsdp = (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *)ect->VendorTable;
				ParseRSDP(Rsdp, GuidStr); 
			}
		}
		ect++;
	}

	if (Rsdp == NULL) {
		Print(L"ERROR: Could not find a RSDP.\n");
		return 1;
	}

	return 0;
}
