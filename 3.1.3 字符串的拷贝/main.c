#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
	DbgPrint("Unload\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	UNICODE_STRING dst;	// Ŀ���ַ���
	WCHAR DstBuff[256];	// �ȶ��建����
	UNICODE_STRING src = RTL_CONSTANT_STRING(L"My source string!");
	// ��Ŀ���ַ�����ʼ��Ϊӵ�л���������Ϊ256��UNICODE_STRING�մ�
	RtlInitEmptyUnicodeString(&dst, DstBuff, 256 * sizeof(WCHAR));
	RtlCopyUnicodeString(&dst, &src);
	DbgPrint("%wZ", &dst);
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}